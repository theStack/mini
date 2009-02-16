#include "crypto.h"
#include "hollywood.h"
#include "utils.h"
#include "memory.h"
#include "irq.h"
#include "ipc.h"
#include "gecko.h"
#include "string.h"
#include "seeprom.h"

#define		AES_CMD_RESET	0
#define		AES_CMD_DECRYPT	0x9800

otp_t otp;
seeprom_t seeprom;

void crypto_read_otp(void)
{
	u32 *otpd = (u32*)&otp;
	int i;
	for (i=0; i< 0x20; i++) {
		write32(HW_OTPCMD,0x80000000|i);
		*otpd++ = read32(HW_OTPDATA);
	}
}

void crypto_read_seeprom(void)
{
  seeprom_read(&seeprom, 0, sizeof(seeprom) / 2);
}

void crypto_initialize()
{
	crypto_read_otp();
	crypto_read_seeprom();
	write32(AES_CMD, 0);
	while (read32(AES_CMD) != 0);
	irq_enable(IRQ_AES);
}

void crypto_ipc(volatile ipc_request *req)
{

	switch (req->req) {
		case IPC_KEYS_GETOTP:
			memcpy((void *)req->args[0], &otp, sizeof(otp));
			dc_flushrange((void *)req->args[0], sizeof(otp));
			break;
		case IPC_KEYS_GETEEP:
			memcpy((void *)req->args[0], &seeprom, sizeof(seeprom));
			dc_flushrange((void *)req->args[0], sizeof(seeprom));
			break;
		default:
			gecko_printf("IPC: unknown SLOW CRYPTO request %04x\n",
					req->req);
	}
	ipc_post(req->code, req->tag, 0);
}


static int _aes_irq = 0;

void aes_irq()
{
	_aes_irq = 1;
}

static inline void aes_command(u16 cmd, u8 iv_keep, u32 blocks)
{
	if (blocks != 0)
		blocks--;
	_aes_irq = 0;
	write32(AES_CMD, (cmd << 16) | (iv_keep ? 0x1000 : 0) | (blocks&0x7f));
	while (read32(AES_CMD) & 0x80000000);
}

void aes_reset(void)
{
	write32(AES_CMD, 0);
	while (read32(AES_CMD) != 0);
}

void aes_set_iv(u8 *iv)
{
	int i;
	for(i = 0; i < 4; i++) {
		write32(AES_IV, *(u32 *)iv);
		iv += 4;
	}
}

void aes_empty_iv()
{
	int i;
	for(i = 0; i < 4; i++)
		write32(AES_IV, 0);
}

void aes_set_key(u8 *key)
{
	int i;
	for(i = 0; i < 4; i++) {
		write32(AES_KEY, *(u32 *)key);
		key += 4;
	}
}

void aes_decrypt(u8 *src, u8 *dst, u32 blocks, u8 keep_iv)
{
	int this_blocks = 0;
	while(blocks > 0) {
		this_blocks = blocks;
		if (this_blocks > 0x80)
			this_blocks = 0x80;
	
		dc_flushrange(src, this_blocks<<4);
		dc_invalidaterange(dst, this_blocks<<4);
		write32(AES_SRC, (u32)src);
		write32(AES_DEST, (u32)dst);
		aes_command(AES_CMD_DECRYPT, keep_iv, this_blocks);

		blocks -= this_blocks;
		src += this_blocks<<4;
		dst += this_blocks<<4;
		keep_iv = 1;
	}

}

void aes_ipc(volatile ipc_request *req)
{
	switch (req->req) {
		case IPC_AES_RESET:
			aes_reset();
			break;
		case IPC_AES_SETIV:
			aes_set_iv((u8 *)req->args);
			break;
		case IPC_AES_SETKEY:
			aes_set_key((u8 *)req->args);
			break;
		case IPC_AES_DECRYPT:
			dc_invalidaterange((u8 *)req->args[0],
					(req->args[3]+1)*16);
			aes_decrypt((u8 *)req->args[0], (u8 *)req->args[1],
				    req->args[2], req->args[3]);
			dc_flushrange((u8 *)req->args[1],
					(req->args[3]+1)*16);
			break;
		default:
			gecko_printf("IPC: unknown SLOW AES request %04x\n",
					req->req);
	}
	ipc_post(req->code, req->tag, 0);
}

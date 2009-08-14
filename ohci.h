/*
	mini - a Free Software replacement for the Nintendo/BroadOn IOS.
	ohci hardware support

Copyright (C) 2009	Bernhard Urban <lewurm@gmx.net>
Copyright (C) 2009	Sebastian Falbesoner <sebastian.falbesoner@gmail.com>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#include "hollywood.h"
#include "types.h"

/* stolen from drivers/usb/host/ohci.h (linux-kernel) :) */

/* OHCI CONTROL AND STATUS REGISTER MASKS */

/*
 * HcControl (control) register masks
 */
#define OHCI_CTRL_CBSR	(3 << 0)	/* control/bulk service ratio */
#define OHCI_CTRL_PLE	(1 << 2)	/* periodic list enable */
#define OHCI_CTRL_IE	(1 << 3)	/* isochronous enable */
#define OHCI_CTRL_CLE	(1 << 4)	/* control list enable */
#define OHCI_CTRL_BLE	(1 << 5)	/* bulk list enable */
#define OHCI_CTRL_HCFS	(3 << 6)	/* host controller functional state */
#define OHCI_CTRL_IR	(1 << 8)	/* interrupt routing */
#define OHCI_CTRL_RWC	(1 << 9)	/* remote wakeup connected */
#define OHCI_CTRL_RWE	(1 << 10)	/* remote wakeup enable */

/* pre-shifted values for HCFS */
#define OHCI_USB_RESET	(0 << 6)
#define OHCI_USB_RESUME	(1 << 6)
#define OHCI_USB_OPER	(2 << 6)
#define OHCI_USB_SUSPEND	(3 << 6)

/*
 * HcCommandStatus (cmdstatus) register masks
 */
#define OHCI_HCR	(1 << 0)	/* host controller reset */
#define OHCI_CLF	(1 << 1)	/* control list filled */
#define OHCI_BLF	(1 << 2)	/* bulk list filled */
#define OHCI_OCR	(1 << 3)	/* ownership change request */
#define OHCI_SOC	(3 << 16)	/* scheduling overrun count */

/*
 * masks used with interrupt registers:
 * HcInterruptStatus (intrstatus)
 * HcInterruptEnable (intrenable)
 * HcInterruptDisable (intrdisable)
 */
#define OHCI_INTR_SO	(1 << 0)	/* scheduling overrun */
#define OHCI_INTR_WDH	(1 << 1)	/* writeback of done_head */
#define OHCI_INTR_SF	(1 << 2)	/* start frame */
#define OHCI_INTR_RD	(1 << 3)	/* resume detect */
#define OHCI_INTR_UE	(1 << 4)	/* unrecoverable error */
#define OHCI_INTR_FNO	(1 << 5)	/* frame number overflow */
#define OHCI_INTR_RHSC	(1 << 6)	/* root hub status change */
#define OHCI_INTR_OC	(1 << 30)	/* ownership change */
#define OHCI_INTR_MIE	(1 << 31)	/* master interrupt enable */


#define OHCI_CONTROL_INIT	(3 << 0)


struct ohci_hcca {
#define NUM_INITS 32
	u32 int_table[NUM_INITS]; /* periodic schedule */
	/*
	 * OHCI defines u16 frame_no, followed by u16 zero pad.
	 * Since some processors can't do 16 bit bus accesses,
	 * portable access must be a 32 bits wide.
	 */
	u32	frame_no;		/* current frame number */
	u32	done_head;		/* info returned for an interrupt */
	u8 reserved_for_hc [116];
	u8 what [4];		/* spec only identifies 252 bytes :) */
} ALIGNED(256);




void ohci_init();
void ohci0_irq();
void ohci1_irq();



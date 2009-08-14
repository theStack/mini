/*
	mini - a Free Software replacement for the Nintendo/BroadOn IOS.
	ohci hardware support

Copyright (C) 2009	Bernhard Urban <lewurm@gmx.net>
Copyright (C) 2009	Sebastian Falbesoner <sebastian.falbesoner@gmail.com>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#include "ohci.h"
#include "gecko.h"
#include "utils.h"
#include "hollywood.h"

static void dbg_op_state() {
	switch (read32(OHCI0_HC_CONTROL) & OHCI_CTRL_HCFS) {
		case OHCI_USB_SUSPEND:
			gecko_printf("ohci-- OHCI_USB_SUSPEND\n");
			break;
		case OHCI_USB_RESET:
			gecko_printf("ohci-- OHCI_USB_RESET\n");
			break;
		case OHCI_USB_OPER:
			gecko_printf("ohci-- OHCI_USB_OPER\n");
			break;
		case OHCI_USB_RESUME:
			gecko_printf("ohci-- OHCI_USB_RESUME\n");
			break;
	}
}


void ohci_init() {
	u32 i = 0;
	gecko_printf("ohci-- init\n");

	/*for(; i <= 0x200; i+=4) {
		gecko_printf("0x0d050000 + %X: %X\n", i, read32(0x0d050000+i));
		udelay(10000); //'cause usb gecko is lame
	}
	* see output in ohci.default
	*/

	/* enable interrupts of both usb host controllers */
	set32(EHCI_CTL, EHCI_CTL_OH0INTE | EHCI_CTL_OH1INTE | 0xe0000);

	dbg_op_state();
	gecko_printf("ohci-- OHCI0_HC_FM_INTERVAL: %X\n", read32(OHCI0_HC_FM_INTERVAL));
	gecko_printf("ohci-- set OHCI_HCR\n");
	set32(OHCI0_HC_COMMAND_STATUS, OHCI_HCR);
	dbg_op_state();
	gecko_printf("ohci-- OHCI0_HC_FM_INTERVAL: %X\n", read32(OHCI0_HC_FM_INTERVAL));
	udelay(12);
	gecko_printf("ohci-- OHCI0_HC_FM_INTERVAL: %X\n", read32(OHCI0_HC_FM_INTERVAL));
	dbg_op_state();


}



void ohci0_irq() {
	gecko_printf("ohci0_irq\n");
}

void ohci1_irq() {
	gecko_printf("ohci1_irq\n");
}


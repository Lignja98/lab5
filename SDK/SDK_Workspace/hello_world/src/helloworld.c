/*
 * Copyright (c) 2009-2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 *
 *
 *
 */


#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xio.h"
#include "vga_periph_mem.h"
#include "xintc.h"

XIntc Intc;

static volatile int can_continue = 0;

void vga_irq_handler(void *arg) {
	(void)arg;

	can_continue = 1;
}

int main()
{
    init_platform();
    unsigned char string_s[] = "LPRS 2\n";
    int i, j;

    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x00, 0x0);// direct mode   0
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x04, 0x3);// display_mode  1
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x08, 0x0);// show frame      2
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x0C, 0x1);// font size       3
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x22, 0x1); // Enable irq
    VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x24, 480); // Set tc
    set_foreground_color(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 0xFFFF00);// foreground 4
    set_background_color(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 0xDD0033);// background color 5
    //VGA_PERIPH_MEM_mWriteMemory(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR + 0x18, 0xFFFF00);// frame color      6

    XStatus Status = XIntc_Initialize (&Intc, XPAR_INTC_0_DEVICE_ID);
    Status = XIntc_Connect(&Intc, XPAR_AXI_INTC_0_VGA_PERIPH_MEM_0_IRQ_O_INTR, vga_irq_handler, NULL);
    Status = XIntc_Start(&Intc, XIN_REAL_MODE);
    XIntc_Enable  (&Intc, XPAR_AXI_INTC_0_VGA_PERIPH_MEM_0_IRQ_O_INTR);

	microblaze_enable_interrupts();

    print("Configuration finished\n\r");

    clear_text_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);
    clear_graphics_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);
    //draw_square(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);
    //draw_rectangle(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 256, 128);

    set_cursor(500);
    print_string(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, string_s, 6);

    while (1) {
    	int square_x = 100;
    	int direction = 1;
		for (i = 0; i < 4800; i++) {
			set_cursor(i);
			print_char(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 'Q');

			draw_square_at(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, square_x, 256);

			if (direction == 1) {
				if (square_x == 440) {
					direction = 0;
				}
				else {
					++square_x;
				}
			}
			else if (direction == 0) {
				if (square_x == 40) {
					direction = 1;
				}
				else {
					--square_x;
				}
			}

			while (!can_continue) {

			}
			can_continue = 0;

			print_char(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, ' ');
		}
    }
    set_cursor(350);
    print_char(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR, 'Q');

    //clear_screen(XPAR_VGA_PERIPH_MEM_0_S_AXI_MEM0_BASEADDR);

    return 0;
}

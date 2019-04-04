/*****************************************************************************
* Filename:          D:\work\lprs2\2013_2014\Resene_vezbe\lab56\basic_system/drivers/vga_periph_mem_v1_00_a/src/vga_periph_mem.c
* Version:           1.00.a
* Description:       vga_periph_mem Driver Source File
* Date:              Wed Mar 05 10:25:21 2014 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/

#include "vga_periph_mem.h"
Xuint32 cursor_position;
/************************** Function Definitions ***************************/

void set_cursor(Xuint32 new_value){
	cursor_position = new_value;
}

void clear_text_screen(Xuint32 BaseAddress){
	int i;
	for (i = 0; i < 4800; i++){
		VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + i*4, 0x20);
	}
}

void print_char(Xuint32 BaseAddress, unsigned char c) {
	VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + cursor_position, (c - 0x40));
}

void print_string(Xuint32 BaseAddress, unsigned char string_s[], int lenght){
	int i;
	for (i = 0; i < lenght; i++){
		VGA_PERIPH_MEM_mWriteMemory(BaseAddress + TEXT_MEM_OFF + cursor_position + i*4, (string_s[i]-0x40));
	}
}

void clear_graphics_screen(Xuint32 BaseAddress){
	int i;
	for (i = 0; i < 9600; i++){
	    VGA_PERIPH_MEM_mWriteMemory(BaseAddress + GRAPHICS_MEM_OFF + i*4, 0x0);
	}
}

void clear_screen(Xuint32 BaseAddress) {
	clear_text_screen(BaseAddress);
	clear_graphics_screen(BaseAddress);
}

void set_foreground_color(Xuint32 BaseAddress, Xuint32 color) {
    VGA_PERIPH_MEM_mWriteMemory(BaseAddress + 0x10, color);
}

void set_background_color(Xuint32 BaseAddress, Xuint32 color) {
    VGA_PERIPH_MEM_mWriteMemory(BaseAddress + 0x14, color);
}

void draw_square(Xuint32 BaseAddress){
	int i, j, k;
	for (j = 0; j < 480; j++){
		for (k = 0; k<(640/32); k++){
			i = j*(640/32) + k;
			if ((j > 200) && (j < 280) && (k > 8) && (k < 12)) {
				VGA_PERIPH_MEM_mWriteMemory(BaseAddress + GRAPHICS_MEM_OFF + i*4, 0xFFFFFFFF);
			}
			else{
				VGA_PERIPH_MEM_mWriteMemory(BaseAddress + GRAPHICS_MEM_OFF + i*4, 0x0);
			}
		}
	}
}

void draw_square_at(Xuint32 BaseAddress, Xuint32 x, Xuint32 y) {
	int i, j, k;

	y /= 32;

	for (j = 0; j < 480; j++){
		for (k = 0; k<(640/32); k++){
			i = j*(640/32) + k;
			if ((j > x - 40) && (j < x + 40) && (k > y - 2) && (k < y + 2)) {
				VGA_PERIPH_MEM_mWriteMemory(BaseAddress + GRAPHICS_MEM_OFF + i*4, 0xFFFFFFFF);
			}
			else{
				VGA_PERIPH_MEM_mWriteMemory(BaseAddress + GRAPHICS_MEM_OFF + i*4, 0x0);
			}
		}
	}
}

void draw_rectangle(Xuint32 BaseAddress, Xuint32 height, Xuint32 width){
	int i, j, k;

	width /= 32;

	for (j = 0; j < 480; j++){
		for (k = 0; k<(640/32); k++){
			i = j*(640/32) + k;
			if ((j > 240 - height / 2) && (j < 240 + height / 2) && (k > 10 - width / 2) && (k < 10 + width / 2)) {
				VGA_PERIPH_MEM_mWriteMemory(BaseAddress + GRAPHICS_MEM_OFF + i*4, 0xFFFFFFFF);
			}
			else{
				VGA_PERIPH_MEM_mWriteMemory(BaseAddress + GRAPHICS_MEM_OFF + i*4, 0x0);
			}
		}
	}
}

void get_resolution(Xuint32 *height, Xuint32 *width) {
	*height = 480;
	*width = 640;
}

/*
 * gui.h
 *
 * SDL bindings.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2022 tomaz stih
 *
 * 31.01.2022   tstih
 *
 */
#ifndef __GUI_H__
#define __GUI_H__

#include <stdint.h>
#include <stdbool.h>

extern void gui_init(args_t *args);
extern void gui_exit();
extern void gui_run(bool(*process_commands)());
extern void gui_cls();
extern void gui_set_intensity(uint8_t intensity);
extern void gui_set_paper(uint8_t paper);
extern void gui_set_pixel(int x, int y);
extern void gui_draw_line(int x0, int y0, int x1, int y1, uint8_t pattern);
extern void gui_set_blit(uint8_t blit);
extern void gui_set_wpage(int page);
extern void gui_set_dpage(int page);
extern void gui_write_raster(int x, int y, int count, uint8_t *bytes);

#endif /* __GUI_H__ */
#include <stdint.h>

extern void gui_init(int width, int height);
extern void gui_exit();
extern void gui_run();
extern void gui_cls();
extern void gui_set_intensity(uint8_t intensity);
extern void gui_set_pixel(int x, int y);
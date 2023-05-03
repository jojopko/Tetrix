/* This file is licensed under the GPL 3. 
 * For details see: www.gnu.org/licenses/gpl-3.0.txt 
 * Author: Konstantin Serezhkin (t.me/jojopko_bot) */

#include <SDL.h>
#include <SDL_ttf.h>
#include <log4c.h>

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

// Window sizes
#define WIN_W 390
#define WIN_H 600

//Global variables
extern bool _is_running;
extern SDL_Renderer * _renderer;
extern SDL_Window * _window;
extern TTF_Font * _font16;
extern SDL_Texture ** _numbers_f16;
extern log4c_category_t * logger;

int init_sdl();
int init_game();
int init_window();
int init_renderer();
int init_ttf();
int init_logger();
int load_font16();
int load_numbers_font16();
SDL_Texture * get_ttf_texture(char * text);


#endif


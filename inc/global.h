#include <SDL.h>
#include <SDL_ttf.h>
#include <log4c.h>
#include "types.h"

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

// Window sizes
#define WIN_W 390
#define WIN_H 600

//Global variables
extern SDL_Renderer * _renderer;
extern SDL_Window * _window;

struct TextureSet {
    int size;
    SDL_Texture ** textures;
};

extern struct GameAssets {
    TextureSet * digits_16pt;
    TextureSet * labels_16pt;
    TextureSet * labels_32pt;
    TextureSet * title;
} * _assets;

enum ScenesType {
    Scene_Menu,
    Scene_Menu_About,
    Scene_Gamemode1,
    Scene_Gamemode2,
    Scene_Demomode
};

extern struct GameStats {
    int score;
    int random_seed;
    bool is_stoped;
    bool is_running;
    bool gameover;
    ScenesType current_scene;
    GameField * gamefield;
    Brush * current_brush;
    Brush * next_brush;
    int * scores_10;
} * _gamestats;

enum TextureSetLabels16 {
    Label_Score,
    Label_NewGame1,
    Label_NewGame2,
    Label_Continue,
    Label_Exit,
    Label_Last_Score,
    Label_HiScore,
    Label_TopScores
};

enum TextureSetLabels32 {
    Label_Tetrix
};

int init_sdl();
int init_game();
int init_window();
int init_renderer();
int init_ttf();
int init_logger();
int load_font16();
int load_numbers_font16();
void exit_game();
int get_hight_score();
void find_largest_numbers();
void append_current_state();

#endif


#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL.h>
#include <SDL/SDL_image.h>


class Game {

public:
    Game();
    ~Game();

    void init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen);

    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() {return isRunning;};

    static SDL_Renderer *renderer;
    static SDL_Event event;
private:
    int cntr;
    bool isRunning;
    SDL_Window *window;



};

#endif  /* Game_hpp */

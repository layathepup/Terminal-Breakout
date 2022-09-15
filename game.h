//
// Created by jack on 9/9/22.
//

#ifndef UNTITLED_GAME_H
#define UNTITLED_GAME_H

#include "board_pieces.h"
#include "ncurses.h"


class Game {
public:
    Game();
    void initialize();
    void render() const { brd.render(); }

    void start();
    void end();
    void tick();


private:
    Board brd;
    int tick_length;
};


#endif //UNTITLED_GAME_H

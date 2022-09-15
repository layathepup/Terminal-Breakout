//
// Created by jack on 9/9/22.
//

#ifndef UNTITLED_BOARD_PIECES_H
#define UNTITLED_BOARD_PIECES_H

#include "ncurses.h"

#include <vector>
#include <queue>
#include <memory>


//forward declarations so base classes can have pointers members to derived classes

class Board;        // forward declarations so Entity can have a Board ptr

// To hold position of entity on board
struct Pos {
    Pos(int x, int y)
        :x{x}, y{y} {};
    int x;
    int y;
};

enum class Dim {
    x, y;
};

class Entity {
public:
    Entity(Pos position)
        : pos{ position } {};
    virtual ~Entity() =0;
    virtual void render(const Board* brd_ptr) const =0;
protected:
    Pos pos;            // location
};

class Board {
public:
    Board(int width, int height)
            :wdth{width}, hgt{height} {};

    WINDOW* get_window() const { return wnd; }
    void create() { wnd = new WINDOW; }
    void render() const
    {
        for (int i = 0; i<pieces.size(); i++)
            pieces[i]->render(this);
        wrefresh(wnd);
    }
private:
    int wdth;       // 0 is left
    int hgt;        // 0 is top
    WINDOW* wnd;
    std::vector<std::unique_ptr<Entity>> pieces;
};

class Block : public Entity {
public:
    Block(Pos position, char block_ch = '#')
        : Entity(position), blck_ch{ block_ch } {};
    void render(const Board* brd_ptr) const override { mvwaddch(brd_ptr->get_window(), pos.y+j, pos.x+i, blck_ch); }
private:
    char blck_ch;
};

class Ball : public Entity {
public:
    Ball(Pos position, std::vector<int> velocity, int ball_ch = '*', int path_len = 5)
        : Entity(position), vel{ velocity }, bll_ch{ ball_ch }
    {
        x_remainder = 0;
        y_remainder = 0;

        path.push(position);
    }

    void render(const Board* brd_ptr) const override;

    void tentative_increment(const int& tick_rate)
        // so that the game can check for a possible collision
    {
        pos.x += (x_remainder + vel[Dim::x]) / tick_rate;
        pos.y += (y_remainder + vel[Dim::y]) / tick_rate;
    }

    void increment(const int& tick_rate)
        // for real this time
    {
        pos.x += (x_remainder + vel[Dim::x]) / tick_rate;
        pos.y += (y_remainder + vel[Dim::y]) / tick_rate;
        x_remainder = vel[Dim::x] % tick_rate;
        y_remainder = vel[Dim::y] % tick_rate;

        path.push(Pos(pos.x, pos.y));
        if (path.size() > path_len)
            path.pop();
    }



    std::queue<Pos> path;
    std::vector<int> vel;
private:
    int x_remainder;
    int y_remainder;    // for remembering the remainder of the velocity vector
                        // after calculating a change in position
                        // ex. velocity = +2x,+5y, 3 ticks per time unit
                        // tick 1, move +(2/3)x, (+5/3)y = +0x, +1y
                            // then x_rem = 2-0*3, y_rem = 5-1*3
                        // tick 2, move ((2+2)/3)x, ((5+2)/3)y = +1x, +2y
                            // then...


    int trl_len;
    char bll_ch;
};

class Bar : public Entity {
    // bar is one char, many bars can be placed in a row
public:
    Bar(Pos position, int width = 1, char ch = '_');

    void render(const Board* brd_ptr) const override {
        for (int i{0}; i<wdth; i++)
            mvwaddch(brd_ptr->get_window(), pos.y, pos.x+i, br_ch);
    }

    void move(const int& delta_x, const int& delta_y)
    {
        pos.x += delta_x;
        pos.y += delta_y;
    }

private:
    int wdth;
    char br_ch;
};


#endif //UNTITLED_BOARD_PIECES_H

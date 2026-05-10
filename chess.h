#ifndef CHESS_H
#define CHESS_H
#include<iostream>
#include<string>
#include<cmath>
#include<cstdlib>
using namespace std;
class board;
class piece {
protected:
    char colour;
    char sym;
    bool moved;
public:
    piece() : colour(' '), sym(' '), moved(false) {}
    piece(char c, char s) : colour(c), sym(s), moved(false) {}
    virtual ~piece() {}
    void setcolour(char c) { colour = c; }
    void setsym(char s) { sym = s; }
    char getcolour() { return colour; }
    char getsym() { return sym; }
    bool hasmoved() { return moved; }
    void setmoved(bool m) { moved = m; }
    virtual bool isvalidmove(int fr, int fc, int tr, int tc, board* b) = 0;
};
class knight : public piece {
public:
    knight(char c) : piece(c, 'n') {}
    bool isvalidmove(int fr, int fc, int tr, int tc, board* b) {
        int dr = abs(tr - fr);
        int dc = abs(tc - fc);
        return (dr == 2 && dc == 1) || (dr == 1 && dc == 2);
    }
};
class rook : public piece {
public:
    rook(char c) : piece(c, 'r') {}
    bool isvalidmove(int fr, int fc, int tr, int tc, board* b);
};
class bishop : public piece {
public:
    bishop(char c) : piece(c, 'b') {}
    bool isvalidmove(int fr, int fc, int tr, int tc, board* b);
};
class king : public piece {
public:
    king(char c) : piece(c, 'k') {}
    bool isvalidmove(int fr, int fc, int tr, int tc, board* b);
};
class queen : public piece {
public:
    queen(char c) : piece(c, 'q') {}
    bool isvalidmove(int fr, int fc, int tr, int tc, board* b);
};
class pawn : public piece {
public:
    pawn(char c) : piece(c, 'p') {}
    bool isvalidmove(int fr, int fc, int tr, int tc, board* b);
};
class board {
private:
    piece* grid[8][8];
    board(const board& other) = delete;
    board& operator=(const board& other) = delete;
public:
    board();
    ~board();
    piece* getpiece(int r, int c) { return grid[r][c]; }
    void setpiece(int r, int c, piece* p) { grid[r][c] = p; }
    void makemove(int fr, int fc, int tr, int tc);
    void show();
    bool isrowclear(int fr, int fc, int tr, int tc);
    bool iscolumnclear(int fr, int fc, int tr, int tc);
    bool isdiagonalclear(int fr, int fc, int tr, int tc);
    bool ischeckcolour(int r, int c, char kingcolour);
    bool ischeck(char kingcolour);
    bool haslegalmove(char colour);
};
class game {
private:
    board b;
    char turn;
public:
    game() { turn = 'w'; }
    void parse(string in, int& fr, int& fc, int& tr, int& tc) {
        fc = in[0] - 'a';
        fr = in[1] - '1';
        tc = in[3] - 'a';
        tr = in[4] - '1';
    }
    void run();
};
#endif
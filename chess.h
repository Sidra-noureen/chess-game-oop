#ifndef CHESS_H
#define CHESS_H
#include<iostream>
#include<string>
#include<cmath>
using namespace std;

class board;

class piece {
protected:
    char colour;
    char sym;
public:
    piece() : colour(' '), sym(' ') {}
    piece(char c, char s) : colour(c), sym(s) {}
    void setcolour(char c) {
        \
            colour = c;
    }
    void setsym(char s) {
        sym = s;
    }
    char getcolour() {
        return colour;
    }
    char getsym() {
        return sym;
    }
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
public:
    piece* getpiece(int r, int c) {
        return grid[r][c];
    }
    void setpiece(int r, int c, piece* p) {
        grid[r][c] = p;
    }
    board() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                grid[i][j] = nullptr;
            }
        }
        grid[0][0] = new rook('w'); grid[0][7] = new rook('w');
        grid[7][0] = new rook('b'); grid[7][7] = new rook('b');
        grid[0][1] = new knight('w'); grid[0][6] = new knight('w');
        grid[7][1] = new knight('b'); grid[7][6] = new knight('b');
        grid[0][2] = new bishop('w'); grid[0][5] = new bishop('w');
        grid[7][2] = new bishop('b'); grid[7][5] = new bishop('b');
        grid[0][3] = new queen('w'); grid[7][3] = new queen('b');
        grid[0][4] = new king('w'); grid[7][4] = new king('b');
        for (int c = 0; c < 8; c++) {
            grid[1][c] = new pawn('w');
            grid[6][c] = new pawn('b');
        }
    }
    ~board() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (grid[i][j] != nullptr)
                    delete grid[i][j];
            }
        }
    }
    void makemove(int fr, int fc, int tr, int tc) {
        if (grid[tr][tc] != nullptr)
            delete grid[tr][tc];
        grid[tr][tc] = grid[fr][fc];
        grid[fr][fc] = nullptr;
        piece* moved = grid[tr][tc];
        if (moved != nullptr && moved->getsym() == 'p') {
            if ((moved->getcolour() == 'w' && tr == 7) || (moved->getcolour() == 'b' && tr == 0)) {
                delete moved;
                grid[tr][tc] = new queen(moved->getcolour());
            }
        }
    }
    void show() {
        cout << endl;
        for (int r = 7; r >= 0; r--) {
            cout << r + 1 << " | ";
            for (int c = 0; c < 8; c++) {
                if (grid[r][c] == nullptr) {
                    cout << ".  ";
                }
                else {
                    if (grid[r][c]->getcolour() == 'w') {
                        cout << (char)toupper(grid[r][c]->getsym()) << "  ";
                    }
                    else {
                        cout << (char)tolower(grid[r][c]->getsym()) << "  ";
                    }
                }
            }
            cout << endl;
        }
        cout << "    a  b  c  d  e  f  g  h" << endl;
    }
    bool isrowclear(int fr, int fc, int tr, int tc) {
        if (fr != tr)
            return false;
        int s, e;
        if (fc < tc) {
            s = fc + 1;
            e = tc - 1;
        }
        else {
            s = tc + 1;
            e = fc - 1;
        }
        for (int c = s; c <= e; c++) {
            if (grid[fr][c] != nullptr)
                return false;
        }
        return true;
    }
    bool iscolumnclear(int fr, int fc, int tr, int tc) {
        if (fc != tc)
            return false;
        int s, e;
        if (fr < tr) {
            s = fr + 1;
            e = tr - 1;
        }
        else {
            s = tr + 1;
            e = fr - 1;
        }
        for (int r = s; r <= e; r++) {
            if (grid[r][fc] != nullptr)
                return false;
        }
        return true;
    }
    bool isdiagonalclear(int fr, int fc, int tr, int tc) {
        int dr;
        if (tr > fr) {
            dr = 1;
        }
        else {
            dr = -1;
        }
        int dc;
        if (tc > fc) {
            dc = 1;
        }
        else {
            dc = -1;
        }
        int r = fr + dr;
        int c = fc + dc;
        while (r != tr && c != tc) {
            if (grid[r][c] != nullptr)
                return false;
            r = r + dr;
            c = c + dc;
        }
        return true;
    }
    bool ischeckcolour(int r, int c, char kingcolour) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                piece* p = grid[i][j];
                if (p != nullptr && p->getcolour() != kingcolour) {
                    if (p->isvalidmove(i, j, r, c, this))
                        return true;
                }
            }
        }
        return false;
    }
    bool ischeck(char kingcolour) {
        int kr = -1, kc = -1;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (grid[i][j] != nullptr && grid[i][j]->getcolour() == kingcolour && grid[i][j]->getsym() == 'k') {
                    kr = i;
                    kc = j;
                    break;
                }
            }
        }
        if (kr == -1)
            return false;
        return ischeckcolour(kr, kc, kingcolour);
    }
    bool haslegalmove(char colour) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (grid[i][j] != nullptr && grid[i][j]->getcolour() == colour) {
                    for (int ti = 0; ti < 8; ti++) {
                        for (int tj = 0; tj < 8; tj++) {
                            if (grid[i][j]->isvalidmove(i, j, ti, tj, this)) {
                                piece* target = grid[ti][tj];
                                if (target != nullptr && target->getcolour() == colour)
                                    continue;
                                piece* temp = grid[ti][tj];
                                piece* moving = grid[i][j];
                                grid[ti][tj] = moving;
                                grid[i][j] = nullptr;
                                bool safe = !ischeck(colour);
                                grid[i][j] = moving;
                                grid[ti][tj] = temp;
                                if (safe)
                                    return true;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }
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
    void run() {
        string in;
        while (true) {
            b.show();
            if (b.ischeck(turn)) cout << "check! ";
            if (turn == 'w') {
                cout << "white's turn (e2 e4): ";
            }
            else {
                cout << "black's turn (e7 e5): ";
            }
            getline(cin, in);
            if (in == "quit") break;
            if (in.length() < 5) {
                cout << "invalid\n";
                continue;
            }
            int fr, fc, tr, tc;
            parse(in, fr, fc, tr, tc);
            if (fr < 0 || fr > 7 || fc < 0 || fc > 7 || tr < 0 || tr > 7 || tc < 0 || tc > 7) {
                cout << "out of board\n";
                continue;
            }
            piece* p = b.getpiece(fr, fc);
            if (p == nullptr) {
                cout << "no piece\n";
                continue;
            }
            if (p->getcolour() != turn) {
                cout << "not your piece\n";
                continue;
            }
            piece* targ = b.getpiece(tr, tc);
            if (targ != nullptr && targ->getcolour() == turn) {
                cout << "cannot capture own\n";
                continue;
            }
            if (!p->isvalidmove(fr, fc, tr, tc, &b)) {
                cout << "illegal move\n";
                continue;
            }
            piece* captured = b.getpiece(tr, tc);
            b.makemove(fr, fc, tr, tc);
            if (p->getsym() == 'k' && abs(tc - fc) == 2) {
                if (tc == 6) {
                    b.makemove(fr, 7, fr, 5);
                }
                else if (tc == 2) {
                    b.makemove(fr, 0, fr, 3);
                }
            }
            if (b.ischeck(turn)) {
                cout << "move leaves king in check! undo\n";
                if (p->getsym() == 'k' && abs(tc - fc) == 2) {
                    if (tc == 6) {
                        b.makemove(fr, 5, fr, 7);
                    }
                    else if (tc == 2) {
                        b.makemove(fr, 3, fr, 0);
                    }
                }
                b.makemove(tr, tc, fr, fc);
                if (captured != nullptr) {
                    b.setpiece(tr, tc, captured);
                }
                else {
                    b.setpiece(tr, tc, nullptr);
                }
                continue;
            }
            if (captured != nullptr && captured->getsym() == 'k') {
                b.show();
                cout << (char)toupper(turn) << " king captured! ";
                if (turn == 'w') {
                    cout << "black wins!\n";
                }
                else {
                    cout << "white wins!\n";
                }
                break;
            }
            if (turn == 'w') {
                turn = 'b';
            }
            else {
                turn = 'w';
            }

            if (b.ischeck(turn) && !b.haslegalmove(turn)) {
                b.show();
                cout << "checkmate! ";
                if (turn == 'w') {
                    cout << "black wins!\n";
                }
                else {
                    cout << "white wins!\n";
                }
                break;
            }

            if (!b.haslegalmove(turn) && !b.ischeck(turn)) {
                b.show();
                cout << "stalemate! game drawn.\n";
                break;
            }
        }
    }
};

#endif
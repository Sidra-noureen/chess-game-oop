#include "chess.h"
board::board() {
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
board::~board() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (grid[i][j] != nullptr)
                delete grid[i][j];
        }
    }
}
void board::makemove(int fr, int fc, int tr, int tc) {
    if (grid[fr][fc] != nullptr) {
        grid[fr][fc]->setmoved(true);
    }
    if (grid[tr][tc] != nullptr)
        delete grid[tr][tc];
    grid[tr][tc] = grid[fr][fc];
    grid[fr][fc] = nullptr;

    piece* moved = grid[tr][tc];
    if (moved != nullptr && moved->getsym() == 'p') {
        if ((moved->getcolour() == 'w' && tr == 7) || (moved->getcolour() == 'b' && tr == 0)) {
            char clr = moved->getcolour();
            delete moved;
            grid[tr][tc] = new queen(clr);
        }
    }
}
void board::show() {
    cout << "   +---+---+---+---+---+---+---+---+" << endl;
    for (int r = 7; r >= 0; r--) {
        cout << " " << r + 1 << " |";
        for (int c = 0; c < 8; c++) {
            if (grid[r][c] == nullptr) {
                cout << " . |";
            }
            else {
                char ch;
                if (grid[r][c]->getcolour() == 'w')
                    ch = toupper(grid[r][c]->getsym());
                else
                    ch = tolower(grid[r][c]->getsym());
                cout << " " << ch << " |";
            }
        }
        cout << endl;
        cout << "   +---+---+---+---+---+---+---+---+" << endl;
    }
    cout << "     a   b   c   d   e   f   g   h" << endl;
}
bool board::isrowclear(int fr, int fc, int tr, int tc) {
    if (fr != tr) return false;
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
bool board::iscolumnclear(int fr, int fc, int tr, int tc) {
    if (fc != tc) return false;
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
bool board::isdiagonalclear(int fr, int fc, int tr, int tc) {
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
bool board::ischeckcolour(int r, int c, char kingcolour) {
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
bool board::ischeck(char kingcolour) {
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
    if (kr == -1) return false;
    return ischeckcolour(kr, kc, kingcolour);
}
bool board::haslegalmove(char colour) {
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
                            if (safe) return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}
bool rook::isvalidmove(int fr, int fc, int tr, int tc, board* b) {
    if (fr != tr && fc != tc) return false;
    if (fr == tr) return b->isrowclear(fr, fc, tr, tc);
    if (fc == tc) return b->iscolumnclear(fr, fc, tr, tc);
    return false;
}
bool bishop::isvalidmove(int fr, int fc, int tr, int tc, board* b) {
    int dr = abs(tr - fr);
    int dc = abs(tc - fc);
    if (dr != dc) return false;
    return b->isdiagonalclear(fr, fc, tr, tc);
}
bool queen::isvalidmove(int fr, int fc, int tr, int tc, board* b) {
    if (fr == tr) return b->isrowclear(fr, fc, tr, tc);
    if (fc == tc) return b->iscolumnclear(fr, fc, tr, tc);
    int dr = abs(tr - fr);
    int dc = abs(tc - fc);
    if (dr == dc) return b->isdiagonalclear(fr, fc, tr, tc);
    return false;
}
bool pawn::isvalidmove(int fr, int fc, int tr, int tc, board* b) {
    int dr = tr - fr;
    int dc = abs(tc - fc);
    int dir;
    if (colour == 'w') {
        dir = 1;
    }
    else {
        dir = -1;
    }
    if (dr == dir && dc == 0 && b->getpiece(tr, tc) == nullptr)
        return true;
    int startrow;
    if (colour == 'w') {
        startrow = 1;
    }
    else {
        startrow = 6;
    }
    if (fr == startrow && dr == 2 * dir && dc == 0 &&
        b->getpiece(tr, tc) == nullptr && b->getpiece(fr + dir, fc) == nullptr)
        return true;
    if (dr == dir && dc == 1 && b->getpiece(tr, tc) != nullptr &&
        b->getpiece(tr, tc)->getcolour() != colour)
        return true;
    return false;
}
bool king::isvalidmove(int fr, int fc, int tr, int tc, board* b) {
    int dr = abs(tr - fr);
    int dc = abs(tc - fc);
    if (dr <= 1 && dc <= 1 && (dr + dc) != 0)
        return true;
    if (dr == 0 && dc == 2 && !hasmoved()) {
        if (tc == 6) {
            piece* rookpiece = b->getpiece(fr, 7);
            if (rookpiece != nullptr && rookpiece->getsym() == 'r' &&
                rookpiece->getcolour() == colour && !rookpiece->hasmoved()) {
                if (b->isrowclear(fr, fc, fr, 7)) {
                    if (!b->ischeckcolour(fr, fc, colour) &&
                        !b->ischeckcolour(fr, fc + 1, colour) &&
                        !b->ischeckcolour(fr, fc + 2, colour)) {
                        return true;
                    }
                }
            }
        }
        else if (tc == 2) {
            piece* rookpiece = b->getpiece(fr, 0);
            if (rookpiece != nullptr && rookpiece->getsym() == 'r' &&
                rookpiece->getcolour() == colour && !rookpiece->hasmoved()) {
                if (b->isrowclear(fr, 0, fr, fc)) {
                    if (!b->ischeckcolour(fr, fc, colour) &&
                        !b->ischeckcolour(fr, fc - 1, colour) &&
                        !b->ischeckcolour(fr, fc - 2, colour)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
void game::run() {
    string in;
    while (true) {
        system("cls");
        b.show();
        if (b.ischeck(turn)) {
            cout << "check! ";
        }
        if (turn == 'w') {
            cout << "white's turn (e2 e4): ";
        }
        else {
            cout << "black's turn (e7 e5): ";
        }
        getline(cin, in);
        if (in == "quit")
            break;
        if (in.length() < 5) {
            cout << "invalid" << endl;
            system("pause");
            continue;
        }
        int fr, fc, tr, tc;
        parse(in, fr, fc, tr, tc);
        if (fr < 0 || fr > 7 || fc < 0 || fc > 7 || tr < 0 || tr > 7 || tc < 0 || tc > 7) {
            cout << "out of board" << endl;
            system("pause");
            continue;
        }
        piece* p = b.getpiece(fr, fc);
        if (p == nullptr) {
            cout << "no piece" << endl;
            system("pause");
            continue;
        }
        if (p->getcolour() != turn) {
            cout << "not your piece" << endl;
            system("pause");
            continue;
        }
        piece* targ = b.getpiece(tr, tc);
        if (targ != nullptr && targ->getcolour() == turn) {
            cout << "cannot capture own" << endl;
            system("pause");
            continue;
        }
        if (!p->isvalidmove(fr, fc, tr, tc, &b)) {
            cout << "illegal move" << endl;
            system("pause");
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
            cout << "move leaves king in check! undo" << endl;
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
            system("pause");
            continue;
        }
        if (turn == 'w') {
            turn = 'b';
        }
        else {
            turn = 'w';
        }
        if (b.ischeck(turn) && !b.haslegalmove(turn)) {
            system("cls");
            b.show();
            cout << "checkmate! ";
            if (turn == 'w') {
                cout << "black wins!" << endl;
            }
            else {
                cout << "white wins!" << endl;
            }
            break;
        }
        if (!b.haslegalmove(turn) && !b.ischeck(turn)) {
            system("cls");
            b.show();
            cout << "stalemate! game drawn." << endl;
            break;
        }
    }
}
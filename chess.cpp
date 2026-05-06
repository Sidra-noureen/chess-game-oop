#include "chess.h"

bool rook::isvalidmove(int fr, int fc, int tr, int tc, board* b) {
    if (fr != tr && fc != tc)
        return false;
    if (fr == tr)
        return b->isrowclear(fr, fc, tr, tc);
    if (fc == tc)
        return b->iscolumnclear(fr, fc, tr, tc);
    return false;
}

bool bishop::isvalidmove(int fr, int fc, int tr, int tc, board* b) {
    int dr = abs(tr - fr);
    int dc = abs(tc - fc);
    if (dr != dc)
        return false;
    return b->isdiagonalclear(fr, fc, tr, tc);
}

bool queen::isvalidmove(int fr, int fc, int tr, int tc, board* b) {
    if (fr == tr)
        return b->isrowclear(fr, fc, tr, tc);
    if (fc == tc)
        return b->iscolumnclear(fr, fc, tr, tc);
    int dr = abs(tr - fr);
    int dc = abs(tc - fc);
    if (dr == dc)
        return b->isdiagonalclear(fr, fc, tr, tc);
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
    if (fr == startrow && dr == 2 * dir && dc == 0 && b->getpiece(tr, tc) == nullptr && b->getpiece(fr + dir, fc) == nullptr)
        return true;
    if (dr == dir && dc == 1 && b->getpiece(tr, tc) != nullptr && b->getpiece(tr, tc)->getcolour() != colour)
        return true;
    return false;
}

bool king::isvalidmove(int fr, int fc, int tr, int tc, board* b) {
    int dr = abs(tr - fr);
    int dc = abs(tc - fc);
    if (dr <= 1 && dc <= 1 && (dr + dc) != 0)
        return true;
    if (dr == 0 && dc == 2) {
        if (tc == 6) {
            piece* rookpiece = b->getpiece(fr, 7);
            if (rookpiece != nullptr && rookpiece->getsym() == 'r' && rookpiece->getcolour() == colour) {
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
            if (rookpiece != nullptr && rookpiece->getsym() == 'r' && rookpiece->getcolour() == colour) {
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
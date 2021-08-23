/*
* Defines the TileBoard_T "class"
*/

#ifndef BOARD_INCLUDED
#define BOARD_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tile.c"

// tile data helper structure
struct tile_data {
    int type;
    int orient;
};

// tile data board helper structure
struct td_board {
    int wid;
    int len;
    struct tile_data *td;
};

// Tile Board class
typedef struct tile_board {
    int wid;
    int len;
    Tile_T *tiles;
}* TileBoard_T;

// create board and return array of tiles
TileBoard_T Board_new(struct td_board td_b) {
    int wid = td_b.wid;
    int len = td_b.len;
    struct tile_data *list = td_b.td;
    // create TileBoard_T
    TileBoard_T board = (TileBoard_T) malloc(sizeof(struct tile_board));
    Tile_T *tiles = (Tile_T *) malloc(sizeof(Tile_T) * (wid * len));
    for (int y = 0; y < len; y++) {
        for (int x = 0; x < wid; x++) {
            int i = y*wid + x;
            Tile_T t = Tile_new(list[i].type, list[i].orient);
            Tile_T north = y == 0 ? NULL : tiles[(y-1)*wid + x];
            Tile_T west = x == 0 ? NULL : tiles[y*wid + (x-1)];
            Tile_link(t, north, west);
            tiles[i] = t;
        }
    }
    board->wid = wid;
    board->len = len;
    board->tiles = tiles;
    return board;
}

void Board_draw(TileBoard_T board) {
    int wid = board->wid;
    int len = board->len;
    Tile_T *ts = board->tiles;
    for (int y = 0; y < len; y++) {
        for (int d = 0; d < 3; d++) {
            for (int x = 0; x < wid; x++) {
                int i = y*wid + x;
                if (ts[i]->type == 0) {
                    printf("           ");
                } else if (d == 0) {
                    printf("[   %d   ]  ", ts[i]->curr[0]);
                } else if (d == 1) {
                    printf("|%d, %d, %d|  ", ts[i]->curr[3], ts[i]->type, ts[i]->curr[1]);
                } else if (d == 2) {
                    printf("[   %d   ]  ", ts[i]->curr[2]);
                }
                //printf("[%d, %d, %d, %d, %d] ", ts[i]->curr[3], ts[i]->curr[0], ts[i]->type, ts[i]->curr[2], ts[i]->curr[1]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void Board_tick(TileBoard_T board) {
    Tile_T *tiles = board->tiles;
    int tot_len = board->wid * board->len;
    for (int i = 0; i < tot_len; i++) {
        Tile_tick(tiles[i]);
    }
}

struct tile_data charToStruct(char *c) {
    switch(*c) {
        case 'n':
            return (struct tile_data){NONE};
        case 'w':
            return (struct tile_data){WIRE};
        case 's': {
            return (struct tile_data){SOURCE};
        }
        case 't':
            return (struct tile_data){SWITCH};
        case 'd': {
            int top = 0;
            char buf[2] = {c[1], '\0'};
            if (atoi(buf)) {
                top = atoi(buf);
            }
            return (struct tile_data){DIODE, top};
        }
        case 'c':
            return (struct tile_data){CROSS};
        default: {
            printf("unknown character code: %c\n", *c);
            break;
        }
    }
    return (struct tile_data){};
}

struct td_board parse(char *string) {
    // example: "3,3,snntd3wwww"
    int wid = atoi(string);
    int len = atoi(strstr(string, ",")+1);
    char *seq = strstr(strstr(string, ",")+1, ",")+1;

    struct tile_data *data = (struct tile_data *)malloc(sizeof(struct tile_data) * (wid*len));
    for (int i = 0; *seq != '\0'; seq++) {
        if (*seq >= 'a' && *seq <= 'z')
            data[i++] = charToStruct(seq);
    }
    return (struct td_board){wid, len, data};
}

void Board_execute(char *com, TileBoard_T board) {
    // "7,13,1,0;0,1,4,3" <-- "x,y,type,orient;...next"
    char *off = com - 1;
    do {
        off++;
        int x = atoi(off);
        char *comma = strstr(off, ",");
        if (comma == NULL)
            return;
        int y = atoi(comma+1);
        comma = strstr(comma+1, ",");
        if (comma == NULL)
            return;
        int type = atoi(comma+1);
        comma = strstr(comma+1, ",");
        if (comma == NULL)
            return;
        int top = atoi(comma+1);
        // make changes
        Tile_T targ = board->tiles[y*(board->wid) + x];
        targ->type = type;
        targ->top = top;
        for (int i = 0; i < 4; i++) {
            targ->curr[i] = 0;
        }
    } while ((int)(off = strstr(off, ";")));
}

#endif

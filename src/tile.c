/*
* Tile object file. Decribes the Tile_T type; its methods and properties.
*/
#ifndef TILE_INCLUDED
#define TILE_INCLUDED

enum {NONE, WIRE, SOURCE, SWITCH, DIODE, CROSS};

static int global_id = 0;

typedef struct Tile {
    int id;
    int currTick;
    int type;
    int top;
    struct Tile *adj[4];
    int curr[4];
    int prev[4];
}* Tile_T;

// return new tile object (pointer to struct)
Tile_T Tile_new(int type, int orient) {
    Tile_T new_tile = (Tile_T) malloc(sizeof(struct Tile));
    if (new_tile == NULL) {
        fprintf(stderr, "ran out of memory...\n");
        return NULL;
    }
    new_tile->id = global_id++;
    new_tile->currTick = 0;
    new_tile->type = type;
    new_tile->top = orient;
    for (int i = 0; i < 4; i++) {
        new_tile->adj[i] = NULL;
        new_tile->curr[i] = 0;
        new_tile->prev[i] = 0;
    }
    return new_tile;
}

void Tile_free(Tile_T t) {
    free(t);
}

// complete doubly linked connection
void Tile_link(Tile_T t, Tile_T north, Tile_T west) {
    t->adj[0] = north;
    t->adj[3] = west;
    if (west != NULL)
        west->adj[1] = t;
    if (north != NULL)
        north->adj[2] = t;
}

void Tile_tick(Tile_T t) {
    switch(t->type) {
        case NONE: {
            // empty tile
            for (int i = 0; i < 4; i++) {
                t->prev[i] = t->curr[i];
                t->curr[i] = 0;
            }
            t->currTick++;
            break;
        }
        case WIRE: {
            // emit power if recieved on any side
            int pow = 0;
            int loc;
            for (int i = 0; i < 4; i++) {
                t->prev[i] = t->curr[i];
                if (t->adj[i] != NULL) {
                    int *nbr = t->adj[i]->currTick == t->currTick ? t->adj[i]->curr : t->adj[i]->prev;
                    if (nbr[(i+2)%4] > pow) {
                        pow = nbr[(i+2)%4];
                        loc = i;
                    }
                }
            }
            for (int i = 0; i < 4; i++) {
                t->curr[i] = pow;
                if (pow && (i == loc)) {
                    t->curr[i] = 0;
                }
            }
            t->currTick++;
            break;
        }
        case SOURCE: {
            // emit power on all sides unconditionally
            for (int i = 0; i < 4; i++) {
                t->prev[i] = t->curr[i];
                t->curr[i] = 1;
            }
            t->currTick++;
            break;
        }
        case SWITCH: {
            // power opp side if exactly 1 side is powering
            int src_count = 0;
            int src_loc;
            for (int i = 0; i < 4; i++) {
                if (t->adj[i] != NULL) {
                    int *nbr = t->adj[i]->currTick == t->currTick ? t->adj[i]->curr : t->adj[i]->prev;
                    if (nbr[(i+2)%4]) {
                        src_count++;
                        src_loc = i;
                    }
                }
                t->prev[i] = t->curr[i];
                t->curr[i] = 0;
            }
            if (src_count == 1) {
                t->curr[(src_loc+2)%4] = 1;
            }
            t->currTick++;
            break;
        }
        case DIODE: {
            // emit power to top side iff bottom side receives power
            for (int i = 0; i < 4; i++) {
                t->prev[i] = t->curr[i];
                t->curr[i] = 0;
            }
            int btm = (t->top+2)%4;
            if (t->adj[btm] != NULL) {
                int *nbr = t->adj[btm]->currTick == t->currTick ? t->adj[btm]->curr : t->adj[btm]->prev;
                if (nbr[t->top]) {
                    t->curr[t->top] = 1;
                }
            }
            t->currTick++;
            break;
        }
        /*case CROSS: {
            // emit power to sides opposite of sources
            for (int i = 0; n < 4; i++) {
                t->prev[i] = t->curr[i];
                t->curr[i] = 0;
            }
            for (int i = 0; i < 4; i++) {
                if (t->adj[i] != NULL) {
                    int *nbr = t->adj[i]->currTick == t->currTick ? t->adj[i]->curr : t->adj[i]->prev;
                    t->curr[(i+2)%4] = nbr[(i+2)%4];
                }
            }
            t->currTick++;
            break;
        }*/
        default: {
            fprintf(stderr, "unknown type\n");
            break;
        }
    }
}

#endif

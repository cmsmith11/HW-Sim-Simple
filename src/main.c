#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tile.c"
#include "board.c"
#define MAX_COMMAND 100

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: ./a.out [path to .td file]\n");
        exit(0);
    }

    printf("\nBeginning simulation of board from .td file \"%s\"\n\n", argv[1]);

    FILE *fp = fopen(argv[1], "rb");
    fseek(fp, 0, SEEK_END);
    int length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *input = (char *)malloc(sizeof(char) * (length+1));
    fread(input, 1, length, fp);
    input[length] = '\0';
    fclose(fp);

    printf("input string: %s\n", input);
    TileBoard_T board = Board_new(parse(input));

    char command[MAX_COMMAND];
    printf("Press any key to advance one tick, or enter command of the form \"x,y,type,top;x,y,type,top;...\" to alter tiles\n");
    while (fgets(command, MAX_COMMAND, stdin)) {
        Board_execute(command, board);
        Board_draw(board);
        Board_tick(board);
    }

    return 0;
}

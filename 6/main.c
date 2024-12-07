#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** lines;

int line_count = 0;

void part1() {
    int direction = 0;
    int distinct_pos = 0;
    int pos_x = 0;
    int pos_y = 0;
    //we first need to find the position of ^
    for (int i = 0; i < strlen(lines[0]); i++) {
        for (int j = 0; j < line_count; j++) {
            if (lines[j][i] == '^') {
                pos_x = i;
                pos_y = j;
            }
        }
    }
    int running = 1;
    int last_pos_x = -100;
    int last_pos_y = -100;
    while (running) {
        if (last_pos_x == pos_x && last_pos_y == pos_y) break;
        last_pos_x = pos_x;
        last_pos_y = pos_y;
        if (pos_x > strlen(lines[0])) running = 0;
        if (pos_x < 0) running = 0;
        if (pos_y >= line_count) running = 0;
        if (pos_y < 0) running = 0;
        if (lines[pos_y][pos_x] != 'X') distinct_pos++;
        lines[pos_y][pos_x] = 'X';
        if (!running) break;
        if      (direction == 0 && (pos_y - 1 >= 0)                && lines[pos_y - 1][pos_x] == '#') direction = 1;
        else if (direction == 1 && (pos_x + 1 <= strlen(lines[0])) && lines[pos_y][pos_x + 1] == '#') direction = 2;
        else if (direction == 2 && (pos_y + 1 < line_count)        && lines[pos_y + 1][pos_x] == '#') direction = 3;
        else if (direction == 3 && (pos_x - 1 >= 0)                && lines[pos_y][pos_x - 1] == '#') direction = 0;

        if      (direction == 0 && (pos_y - 1 >= 0)                && lines[pos_y - 1][pos_x] != '#') pos_y--;
        else if (direction == 1 && (pos_x + 1 <= strlen(lines[0])) && lines[pos_y][pos_x + 1] != '#') pos_x++;
        else if (direction == 2 && (pos_y + 1 < line_count)        && lines[pos_y + 1][pos_x] != '#') pos_y++;
        else if (direction == 3 && (pos_x - 1 >= 0)                && lines[pos_y][pos_x - 1] != '#') pos_x--;
    }

    printf("distinct positions: %d\n", distinct_pos);

}

void part2() {
    int direction = 0;
    int distinct_pos = 0;
    int pos_x = 0;
    int pos_y = 0;
    //we first need to find the position of ^
    for (int i = 0; i < strlen(lines[0]); i++) {
        for (int j = 0; j < line_count; j++) {
            if (lines[j][i] == '^') {
                pos_x = i;
                pos_y = j;
            }
        }
    }
    int running = 1;
    int last_pos_x = -100;
    int last_pos_y = -100;
    while (running) {
        if (last_pos_x == pos_x && last_pos_y == pos_y) break;
        last_pos_x = pos_x;
        last_pos_y = pos_y;
        if (pos_x > strlen(lines[0])) running = 0;
        if (pos_x < 0) running = 0;
        if (pos_y >= line_count) running = 0;
        if (pos_y < 0) running = 0;
        if (lines[pos_y][pos_x] != 'X') distinct_pos++;
        lines[pos_y][pos_x] = 'X';
        if (!running) break;
        if      (direction == 0 && (pos_y - 1 >= 0)                && lines[pos_y - 1][pos_x] == '#') direction = 1;
        else if (direction == 1 && (pos_x + 1 <= strlen(lines[0])) && lines[pos_y][pos_x + 1] == '#') direction = 2;
        else if (direction == 2 && (pos_y + 1 < line_count)        && lines[pos_y + 1][pos_x] == '#') direction = 3;
        else if (direction == 3 && (pos_x - 1 >= 0)                && lines[pos_y][pos_x - 1] == '#') direction = 0;

        if      (direction == 0 && (pos_y - 1 >= 0)                && lines[pos_y - 1][pos_x] != '#') pos_y--;
        else if (direction == 1 && (pos_x + 1 <= strlen(lines[0])) && lines[pos_y][pos_x + 1] != '#') pos_x++;
        else if (direction == 2 && (pos_y + 1 < line_count)        && lines[pos_y + 1][pos_x] != '#') pos_y++;
        else if (direction == 3 && (pos_x - 1 >= 0)                && lines[pos_y][pos_x - 1] != '#') pos_x--;
        printf("board state:\n");
        for (int i = 0; i < line_count; i++) {
            for (int j = 0; j < strlen(lines[0]); j++) {
                if (pos_x == j && pos_y == i) printf("\033[31m");
                printf("%c\033[0m", lines[i][j]);
            }
            printf("\n");
        }
        printf("direction: %d, at (%d, %d)\n", direction, pos_x, pos_y);

    }

    printf("distinct positions: %d\n", distinct_pos);

}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("%s file\n", argv[0]);
        exit(-1);
    }

    //get file from argv
    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("%s file\n", argv[0]);
        exit(-1);
    }

    //scan file to get newline count
    char buf[256];
    for (;;) {
        size_t res = fread(buf, 1, 256, fp);
        for (int i = 0; i < res; i++) if (buf[i] == '\n') line_count++;
        if (feof(fp)) {
            line_count++;
            break;
        }
    }
    
    rewind(fp);
    lines = malloc(sizeof(*lines) * line_count);
    for (int i = 0; i < line_count; i++) {
        char buf[1024] = {0};
        int res = fread(buf, 1, 1023, fp);
        
        for (int j = 0; j < 1024; j++) {
            if (buf[j] == '\n' || j == res) {
                lines[i] = malloc(sizeof(**lines) * (j + 1));
                memcpy(lines[i], buf, j);
                lines[i][j] = '\0';
                fseek(fp, -(res - j) + 1, SEEK_CUR);
                break;
            }
        }
    }

    part1();

    part2();

    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }
    free(lines);
    return 0;
}
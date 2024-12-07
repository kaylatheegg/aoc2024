#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** lines;

int line_count = 0;

void part1() {
    //get length of lines[0] and use that to for loop, we start from X and then go round in a circle until we find an M
    int xmas_count = 0;
    int width = strlen(lines[0]);
    for (int y = 0; y < line_count; y++) {
        for (int x = 0; x < width; x++) {
            if (lines[y][x] == 'X') {
                //we can now start Looking.
                //we look forward first
                //R, L, D, U, then DR, DL, UR, UL
                if (x + 3 < width      && lines[y][x + 1] == 'M' && lines[y][x + 2] == 'A' && lines[y][x + 3] == 'S') xmas_count++;
                if (x - 3 >= 0         && lines[y][x - 1] == 'M' && lines[y][x - 2] == 'A' && lines[y][x - 3] == 'S') xmas_count++;
                if (y + 3 < line_count && lines[y + 1][x] == 'M' && lines[y + 2][x] == 'A' && lines[y + 3][x] == 'S') xmas_count++;
                if (y - 3 >= 0         && lines[y - 1][x] == 'M' && lines[y - 2][x] == 'A' && lines[y - 3][x] == 'S') xmas_count++;
                if (x + 3 < width && y + 3 < line_count && lines[y + 1][x + 1] == 'M' && lines[y + 2][x + 2] == 'A' && lines[y + 3][x + 3] == 'S') xmas_count++;
                if (x - 3 < width && y + 3 < line_count && lines[y + 1][x - 1] == 'M' && lines[y + 2][x - 2] == 'A' && lines[y + 3][x - 3] == 'S') xmas_count++;
                if (x + 3 < width && y - 3 >= 0          && lines[y - 1][x + 1] == 'M' && lines[y - 2][x + 2] == 'A' && lines[y - 3][x + 3] == 'S') xmas_count++;
                if (x - 3 >= 0    && y - 3 >= 0          && lines[y - 1][x - 1] == 'M' && lines[y - 2][x - 2] == 'A' && lines[y - 3][x - 3] == 'S') xmas_count++;
            }
        }
    }
    printf("xmas count: %d\n", xmas_count);
}

void part2() {
    //get length of lines[0] and use that to for loop, we start from X and then go round in a circle until we find an M
    int xmas_count = 0;
    int width = strlen(lines[0]);
    for (int y = 0; y < line_count; y++) {
        for (int x = 0; x < width; x++) {
            if (lines[y][x] == 'M') {
                //we can now start Looking.
                //we look forward first
                /* we detect from the capitalised M in each
                DOWN:
                    M.m
                    .a.
                    s.s
                UP:
                    s.s
                    .a.
                    M.m
                LEFT:
                    M.s
                    .a.
                    m.s
                RIGHT:
                    s.M
                    .a.
                    s.m
                */
                if (x + 2 < width && y + 2 < line_count) { //DOWN
                    if (lines[y][x + 2] == 'M' &&
                        lines[y + 1][x + 1] == 'A' &&
                        lines[y + 2][x] == 'S' &&
                        lines[y + 2][x + 2] == 'S') xmas_count++;
                }
                if (x + 2 < width && y - 2 >= 0) { //UP
                    if (lines[y][x + 2] == 'M' &&
                        lines[y - 1][x + 1] == 'A' &&
                        lines[y - 2][x] == 'S' &&
                        lines[y - 2][x + 2] == 'S') xmas_count++;
                }
                if (x + 2 < width && y + 2 < line_count) { //LEFT
                    if (lines[y][x + 2] == 'S' &&
                        lines[y + 1][x + 1] == 'A' &&
                        lines[y + 2][x] == 'M' &&
                        lines[y + 2][x + 2] == 'S') xmas_count++;                    
                }
                if (x - 2 >= 0 && y - 2 >= 0) { //RIGHT
                    if (lines[y][x - 2] == 'S' &&
                        lines[y - 1][x - 1] == 'A' &&
                        lines[y - 2][x] == 'M' &&
                        lines[y - 2][x - 2] == 'S') xmas_count++;                    
                }
                        
            }
        }
    }
    printf("xmas count: %d\n", xmas_count);
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
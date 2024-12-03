#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** lines;

int line_count = 0;

enum {
    PARSER_START,
    PARSER_FIRST_NUM,
    PARSER_SECOND_NUM,
    PARSER_END
};

#define VERIFY if ((pos + 1) >= strlen(lines[i])) continue


void part1() {
    //we scan, grab two numbers, continue
    long running_sum = 0;
    int do_mul = 1;
    for (int i = 0; i < line_count; i++) {
        int parser_mode = PARSER_START;
        int first_num = 0;
        int second_num = 0;
        for (int pos = 0; pos < strlen(lines[i]); pos++) {
            first_num = 0;
            second_num = 0;
            if (lines[i][pos] == 'm') {
                VERIFY;
                pos++;
                if (lines[i][pos] == 'u') {
                    VERIFY;
                    pos++;
                    if (lines[i][pos] == 'l') {
                        VERIFY;
                        pos++;
                        if (lines[i][pos] == '(') {
                            VERIFY;
                            pos++;
                            for (; pos < strlen(lines[i]); pos++) {
                                if (isdigit(lines[i][pos])) {
                                    first_num *= 10;
                                    first_num += lines[i][pos] - '0';
                                    continue;
                                }
                                if (lines[i][pos] == ',') {
                                    //we can safely break
                                    pos++;
                                    break;
                                } else {
                                    pos--;
                                    goto end;
                                }
                            }
                            for (; pos < strlen(lines[i]); pos++) {
                                if (isdigit(lines[i][pos])) {
                                    second_num *= 10;
                                    second_num += lines[i][pos] - '0';
                                    continue;
                                }
                                if (lines[i][pos] == ')') {
                                    //we can safely break
                                    break;
                                } else {
                                    pos--;
                                    goto end;
                                }
                            }
                            //printf("%d * %d = %d\n", first_num, second_num, first_num * second_num);
                            if (do_mul) running_sum += first_num * second_num;
                            goto end;
                        }
                        pos--;
                        goto end;
                    }
                    pos--;
                    goto end;
                }
                pos--;
                goto end;
            }

            if (lines[i][pos] == 'd') {
                VERIFY;
                pos++;
                if (lines[i][pos] == 'o') {
                    VERIFY;
                    pos++;
                    if (lines[i][pos] == 'n') {
                        VERIFY;
                        pos++;
                        if (lines[i][pos] == '\'') {
                            VERIFY;
                            pos++;
                            if (lines[i][pos] == 't') {
                                VERIFY;
                                pos++;
                                if (lines[i][pos] == '(') {
                                    VERIFY;
                                    pos++;
                                    if (lines[i][pos] == ')') {
                                        do_mul = 0;
                                        continue;
                                    }
                                    pos--;
                                    goto end;
                                }
                                pos--;
                                goto end;
                            }
                            pos--;
                            goto end;
                        }
                        pos--;
                        goto end;
                    }
                    if (lines[i][pos] == '(') {
                        VERIFY;
                        pos++;
                        if (lines[i][pos] == ')') {
                            do_mul = 1;
                            continue;
                        }
                        pos--;
                        goto end;
                    }
                    pos--;
                    goto end;
                }
                pos--;
                goto end;
            }

            printf("%c", lines[i][pos]);
            end:
        }
        printf("\ncurrently on line %d\n", i);
    }
    printf("sum of muls: %d\n", running_sum);
}

void part2() {

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
        char buf[10240] = {0};
        int res = fread(buf, 1, 10239, fp);
        
        for (int j = 0; j < 10239; j++) {
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
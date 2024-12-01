#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** lines;

int line_count = 0;

int compare_int(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

void part1() {
    int* left_list = malloc(sizeof(*left_list) * line_count);
    int* right_list = malloc(sizeof(*right_list) * line_count);
    for (int i = 0; i < line_count; i++) sscanf(lines[i], "%d   %d", &left_list[i], &right_list[i]);
    qsort(left_list, line_count, sizeof(*left_list), compare_int);
    qsort(right_list, line_count, sizeof(*right_list), compare_int);
    //sum distances
    int total_distance = 0;
    for (int i = 0; i < line_count; i++) {
        int dist = left_list[i] - right_list[i];
        if (dist < 0) dist = -dist;
        total_distance += dist;
    }

    printf("total distance: %d\n", total_distance);
}

void part2() {
    int* left_list = malloc(sizeof(*left_list) * line_count);
    int* right_list = malloc(sizeof(*right_list) * line_count);
    for (int i = 0; i < line_count; i++) sscanf(lines[i], "%d   %d", &left_list[i], &right_list[i]);
    qsort(left_list, line_count, sizeof(*left_list), compare_int);
    qsort(right_list, line_count, sizeof(*right_list), compare_int);
    //this algorithm is going to be awful.
    
    int total_similarity = 0;
    for (int i = 0; i < line_count; i++) {
        int current_similarity = 0;
        for (int j = 0; j < line_count; j++) {
            if (left_list[i] == right_list[j]) {
                current_similarity++;
            }
        }
        current_similarity *= left_list[i];
        total_similarity += current_similarity;
    }

    printf("total similarity: %d\n", total_similarity);
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
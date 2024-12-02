#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

char** lines;

int line_count = 0;

#define iabs(x) (((x) < 0) ? -(x) : (x))

//this one was brute forced and sucked.
//augh.

void part1() {
    int safe_count = 0;
    for (int i = 0; i < line_count; i++) {
        //first, we need to extract the report info
        int* reports = malloc(sizeof(*reports));
        int reading_mode = 0;
        int curr_num = 0;
        int num_count = 0;
        for (int j = 0; j < strlen(lines[i]) + 1; j++) {
            if (lines[i][j] == ' ' || j == strlen(lines[i])) {
                num_count++;
                reports = realloc(reports, sizeof(*reports) * num_count);
                reports[num_count - 1] = curr_num;
                curr_num = 0;
                reading_mode = 0;
                continue;
            }

            if (reading_mode == 1) {
                curr_num *= 10;
                curr_num += lines[i][j] - '0';
                continue;
            }

            if (isdigit(lines[i][j])) {
                reading_mode = 1;
                curr_num = lines[i][j] - '0';
                continue;
            }
        }
        //now we have the data, we need it to obey the constraints:
        // 1 <= |report_{n+1} - report_{n}| <= 3
        // report must ascend or descend THROUGHOUT.
        
        int is_safe = 1;
        for (int j = 0; j < num_count - 1; j++) {
            //we check numerical bounds first.
            int diff = iabs(reports[j + 1] - reports[j]);
            if (!(1 <= diff && diff <= 3)) is_safe = 0;
        }
        //now, we get the directionality from the first two.
        if      (reports[0] < reports[1]) { for (int j = 0; j < num_count - 1; j++) if (reports[j] > reports[j+1]) is_safe = 0; }
        else if (reports[0] > reports[1]) { for (int j = 0; j < num_count - 1; j++) if (reports[j] < reports[j+1]) is_safe = 0; }
        else is_safe = 0;

        //printf("report %d: %s has safety %d\n", i, lines[i], is_safe);

        safe_count += is_safe;
        free(reports);
    }
    printf("number of safe reports: %d\n", safe_count);
}

int verifyReport(int* report, int length);

int* withRemoved(int* array, int length, int index) {
    int* new_array = malloc(sizeof(*new_array) * (length - 1));
    if (new_array == NULL) {
        printf("array null! %s\n", strerror(errno));
        exit(-1);
    }
    for (int i = 0; i < index; i++) {
        new_array[i] = array[i];
    }
    for (int i = index + 1; i < length; i++) {
        new_array[i - 1] = array[i];
    }
    return new_array;
}

void part2() {
    int safe_count = 0;
    for (int i = 0; i < line_count; i++) {
        //first, we need to extract the report info
        int* reports = malloc(sizeof(*reports));
        int reading_mode = 0;
        int curr_num = 0;
        int num_count = 0;
        for (int j = 0; j < strlen(lines[i]) + 1; j++) {
            if (lines[i][j] == ' ' || j == strlen(lines[i])) {
                num_count++;
                reports = realloc(reports, sizeof(*reports) * num_count);
                reports[num_count - 1] = curr_num;
                curr_num = 0;
                reading_mode = 0;
                continue;
            }

            if (reading_mode == 1) {
                curr_num *= 10;
                curr_num += lines[i][j] - '0';
                continue;
            }

            if (isdigit(lines[i][j])) {
                reading_mode = 1;
                curr_num = lines[i][j] - '0';
                continue;
            }
        }

        int is_safe = verifyReport(reports, num_count);
        if (!is_safe) {
            //brute force remove one element each. if we get a SINGLE safe one, we continue.
            for (int i = 0; i < num_count; i++) {
                int* current_report = withRemoved(reports, num_count, i);
                if (verifyReport(current_report, num_count - 1)) is_safe = 1;
            }
        }

        safe_count += is_safe;

        printf("report %d: %s has safety %d\n", i, lines[i], is_safe);
        free(reports);
    }
    printf("number of safe reports: %d\n", safe_count);
}

int verifyReport(int* reports, int num_count) {
    //now we have the data, we need it to obey the constraints:
    // 1 <= |report_{n+1} - report_{n}| <= 3
    // report must ascend or descend THROUGHOUT.
        
    int is_safe = 1;
    for (int j = 0; j < num_count - 1; j++) {
        //we check numerical bounds first.
        int diff = iabs(reports[j + 1] - reports[j]);
        if (!(1 <= diff && diff <= 3)) is_safe = 0;
    }
    //now, we get the directionality from the first two.
    if      (reports[0] < reports[1]) { for (int j = 0; j < num_count - 1; j++) if (reports[j] > reports[j+1]) is_safe = 0; }
    else if (reports[0] > reports[1]) { for (int j = 0; j < num_count - 1; j++) if (reports[j] < reports[j+1]) is_safe = 0; }
    else is_safe = 0;

    //printf("report %d: %s has safety %d\n", i, lines[i], is_safe);

    return is_safe;
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
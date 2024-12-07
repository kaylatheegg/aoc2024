#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** lines;

int line_count = 0;

typedef struct {
    int page;
    int before;
} rule;

rule* rules;
int rules_length;

rule* used_rules;

void appendRule(int page, int before) {
    if (rules_length == 0) {
        rules = malloc(sizeof(*rules));
    }
    rules = realloc(rules, sizeof(*rules) * (rules_length + 1));
    rules[rules_length] = (rule){page, before};
    rules_length++;
}

void part1() {
    //this one is gonna suck.
    for (int i = 0; i < line_count; i++) {
        if (strlen(lines[i]) <= 1) break;
        int page = 0;
        int before = 0;
        sscanf(lines[i], "%d|%d", &page, &before);
        appendRule(page, before);
    }
    int printing_sum = 0;
    //we now need to scan in the printing, and add up the middle number
    //printing string looks like:
    //aa,aa,aa,aa,aa
    //so we take the length, add one, divide by 3, multiply by 2 to obtain the number of numbers we want
    for (int i = rules_length + 1; i < line_count; i++) {
        int num_len = (strlen(lines[i]) + 1) / 3;
        int* nums = malloc(sizeof(*nums) * num_len);
        for (int j = 0; j < num_len; j++) {
            int new_num = (lines[i][j * 3 + 0] - '0') * 10;
            new_num += lines[i][j * 3 + 1] - '0';
            nums[j] = new_num;
        }
        
        int verified_good = 1;
        //now, we need to verify no rules are broken.
        for (int j = 0; j < num_len; j++) {
            for (int k = 0; k < rules_length; k++) {
                if (rules[k].page == nums[j]) {
                    for (int l = 0; l < j; l++) {
                        if (rules[k].before == nums[l]) {
                            verified_good = 0;
                        }
                    }
                }
            }
        }

        if (verified_good) {
            printing_sum += nums[(num_len - 1) / 2];
        }

        free(nums);
    }

    printf("middle page number sum: %d\n", printing_sum);
}

void part2() {
    int printing_sum = 0;
    //we now need to scan in the printing, and add up the middle number
    //printing string looks like:
    //aa,aa,aa,aa,aa
    //so we take the length, add one, divide by 3, multiply by 2 to obtain the number of numbers we want
    for (int i = rules_length + 1; i < line_count; i++) {
        int num_len = (strlen(lines[i]) + 1) / 3;
        int* nums = malloc(sizeof(*nums) * num_len);
        for (int j = 0; j < num_len; j++) {
            int new_num = (lines[i][j * 3 + 0] - '0') * 10;
            new_num += lines[i][j * 3 + 1] - '0';
            nums[j] = new_num;
        }
        
        int verified_good = 1;
        int erroring_rule_count = 0;
        //now, we need to verify no rules are broken.
        for (int j = 0; j < num_len; j++) {
            for (int k = 0; k < rules_length; k++) {
                if (rules[k].page == nums[j]) {
                    for (int l = 0; l < j; l++) {
                        if (rules[k].before == nums[l]) {
                            verified_good = 0;
                            erroring_rule_count++;
                        }
                    }
                }
            }
        }

        int was_erroring = verified_good == 0;

        while (!verified_good) {
            verified_good = 1;
            erroring_rule_count = 0;
            //scan until we hit an erroring rule, then we swap the two that are erroring.
            for (int j = 0; j < num_len; j++) {
                for (int k = 0; k < rules_length; k++) {
                    if (rules[k].page == nums[j]) {
                        for (int l = 0; l < j; l++) {
                            if (rules[k].before == nums[l]) {
                                //swap nums[j] and nums[l]
                                nums[j] ^= nums[l];
                                nums[l] ^= nums[j];
                                nums[j] ^= nums[l];
                            }
                        }
                    }
                }
            }

            for (int j = 0; j < num_len; j++) {
                for (int k = 0; k < rules_length; k++) {
                    if (rules[k].page == nums[j]) {
                        for (int l = 0; l < j; l++) {
                            if (rules[k].before == nums[l]) {
                                verified_good = 0;
                                erroring_rule_count++;
                            }
                        }
                    }
                }
            }
        }

        if (was_erroring) printing_sum += nums[(num_len - 1) / 2];

        free(nums);
    }
    printf("middle page number sum: %d\n", printing_sum);
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
/* Lfr - lfr prints the character set number times. 
   Copyright (C) 2021 by pic16f877ccs

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <getopt.h>
#include <ctype.h>
#include <errno.h>

#include "utf8.h"

#ifndef PROGRAM_NAME
#define PROGRAM_NAME "lfr"
#endif

#ifndef AUTHORS
#define AUTHORS "Karl Wulfert"
#endif

#ifndef VERSION
#define VERSION 0.1
#endif

#define LIMIT_FILL 10000000

/* 
    Options from GNU's coreutils/src/system.h */

enum {
    GETOPT_HELP_CHAR = (CHAR_MIN - 2),
    GETOPT_VERSION_CHAR = (CHAR_MIN - 3)
};
#define GETOPT_HELP_OPTION_DECL \
    "help", no_argument, NULL, GETOPT_HELP_CHAR
#define GETOPT_VERSION_OPTION_DECL \
    "version", no_argument, NULL, GETOPT_VERSION_CHAR
#define case_GETOPT_HELP_CHAR                   \
    case GETOPT_HELP_CHAR:                        \
        usage(EXIT_SUCCESS);                        \
    break;
#define case_GETOPT_VERSION_CHAR(Program_name, Version, Authors)        \
    case GETOPT_VERSION_CHAR:                                             \
        fprintf(stdout, "%s version %0.3f\nCopyright (c) 2021 by pic16f877ccs\n"   \
                "%s is free software and comes with ABSOLUTELY NO WARRANTY.\n" \
                "Distributed under the CPLv3 License.\n\nWritten by %s\n",      \
                Program_name, (double) Version, Program_name, Authors);       \
        exit(EXIT_SUCCESS);                                                   \
    break;
/* end code from system.h */

void usage(int status) {
    fputs("\nUsage: fllt [OPTION]... [FILE]\n"\
          "Prints the character set number times\n", stdout);

    fputs("\nOptions:\n\
  -f,     --fill=CHARACTER  use single CHARACTER to filling (default: \\s)\n\
  -n,     --number=INTEGER  use positive INTEGER to expand max lines (default: 0)\n\
  -z,     --zero            output lines are NUL-terminated\n\
          --help            display this help and exit\n\
          --version         output version information and exit\n\n", stdout);

    exit(status);
}

int main(int argc, char *argv[]) {

    int number_chr_start= 0, number_chr_end = 0, option = 0;
    
    char str_start[10] = {" "};    //⇛
    char str_end[10] = {" "};    //⇛
    
    bool zero_flag = false, end_flag = false;

    static struct option const long_options[] = {
        /* Options with an argument */
        {"start", required_argument, 0, 'F'},
        {"number-start", required_argument, 0, 'N'},
        {"end", required_argument, 0, 'f'},
        {"number-end", required_argument, 0, 'n'},
        {"zero", required_argument, 0, 'z'},
        {GETOPT_HELP_OPTION_DECL},
        {GETOPT_VERSION_OPTION_DECL},
        {NULL, 0, NULL, 0}
    };

    while((option = getopt_long(argc, argv, "F:N:f:n:z", long_options, NULL)) != -1) {
        switch(option) {
            case '?':
                exit(EXIT_FAILURE);

                break;
            case 'F':
                if(utf8len(optarg) > 1) {
                    fprintf(stderr, "lfr : argument to -- 'F' should be a one character\n"\
                            "Try 'lfr --help' for more information.\n");
                    exit(EXIT_FAILURE);
                }

                utf8cpy(str_start, optarg);
                break;

            case 'N': 
               for(int i = 0; i < (int)(strlen(optarg));) {
                   if((isdigit(optarg[i]) == 0) && (optarg[i] != '-')) {
                       fprintf(stderr, "lfr : argument to -- 'N' should be a integer\n"\
                               "Try 'lfr --help' for more information.\n");
                       exit(EXIT_FAILURE);
                   }

                   i++;
               }

                if((number_chr_start = abs(atoi(optarg))) > LIMIT_FILL) {
                    fprintf(stderr, "lfr : should then be a non down six digits\n"\
                            "Try 'lfr --help' for more information.\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'f':
                if(utf8len(optarg) > 1) {
                    fprintf(stderr, "lfr : argument to -- 'f' should be a one character\n"\
                            "Try 'lfr --help' for more information.\n");
                    exit(EXIT_FAILURE);
                }

                utf8cpy(str_end, optarg);
                break;

            case 'n': 
               for(int i = 0; i < (int)(strlen(optarg));) {
                   if((isdigit(optarg[i]) == 0) && (optarg[i] != '-')) {
                       fprintf(stderr, "lfr : argument to -- 'n' should be a integer\n"\
                               "Try 'lfr --help' for more information.\n");
                       exit(EXIT_FAILURE);
                   }

                   i++;
               }

                if((number_chr_end = abs(atoi(optarg))) > LIMIT_FILL) {
                    fprintf(stderr, "lfr : should then be a non down six digits\n"\
                            "Try 'lfr --help' for more information.\n");
                    exit(EXIT_FAILURE);
                }

                end_flag = true;
                break;

            case 'z':
                zero_flag = true;
                break;

            case ':':
                exit(EXIT_FAILURE);
                break;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR(PROGRAM_NAME, VERSION, AUTHORS);

            default:
                usage(EXIT_FAILURE);
        }
    }


    if(number_chr_start == 0 || argc != optind) {
        printf("Try 'lfr --help' for more information.\n");
        exit(EXIT_FAILURE);
        
    }

    for(;number_chr_start--;)
        fputs(str_start, stdout);

    if(end_flag) {
        for(;number_chr_end--;)
            fputs(str_end, stdout);
    }

    /* lines are NUL-terminated */
    if(!zero_flag)
        putchar('\n');

    exit(EXIT_SUCCESS);
}


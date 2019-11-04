/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * Copyright (c) Petr Bena 2019
 *
 * This utility will give you a "free name" for a file of your chosing
 */

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include "str_replace.h"
#include "freename.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "freename"

#define AUTHORS "Petr Bena"

/* First number to start counter with */
int opt_counter_first = 0;
/* Method used to figure out the free filename */
int opt_method = METHOD_COUNTER;

static struct option const long_opts[] =
{
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0}
};

void usage()
{
    printf("Usage: %s [OPTION] file\n", PROGRAM_NAME);
}

int is_free(char *path)
{
    return (access(path, F_OK) == -1);
}

char *construct_new_name(char *name, int counter)
{
    char strcounter[10];
    snprintf(strcounter, 10, "%d", counter);
    return str_replace(name, "$counter", strcounter);
}

char *find_name(char *path)
{
    if (is_free(path))
        return path;
    if (strstr(path, "$counter") == NULL)
        strcat(path, ".$counter");
    int current_counter = opt_counter_first;
    char *current_name = construct_new_name(path, current_counter);
    while (!is_free(current_name))
    {
        current_counter++;
        free(current_name);
        current_name = construct_new_name(path, current_counter);
    }
    return current_name;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        usage();
        return 1;
    }

    int return_code = 0;
    int c;
    while ((c = getopt_long(argc, argv, "h", long_opts, NULL)) != -1)
    {
        switch (c)
        {
            case 'h':
            case OPT_HELP:
                usage();
                return 0;
        }
    }

    printf("%s\n", find_name(argv[optind]));

    return return_code;
}

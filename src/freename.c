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
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include "str_replace.h"
#include "freename.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "freename"

#define AUTHORS "Petr Bena"
#define VERSION "1.0"

/* First number to start counter with */
int opt_counter_first = 0;
/* Method used to figure out the free filename */
int opt_method = METHOD_COUNTER;

static struct option const long_opts[] =
{
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'v'},
    {NULL, 0, NULL, 0}
};

void version()
{
    printf("%s v. %s\n", PROGRAM_NAME, VERSION);
}

void usage()
{
    printf("Usage: %s [OPTION] file\n", PROGRAM_NAME);
}

int is_free(char *path)
{
    return (access(path, F_OK) == -1);
}

char *replace_counter_with_number(char *name, int counter)
{
    char strcounter[10];
    snprintf(strcounter, 10, "%d", counter);
    return str_replace(name, "$counter", strcounter);
}

char *find_name(char *path)
{
    /* first check if $counter is present, if yes replace it with 0 and then check the file name */
    if (strstr(path, "$counter") == NULL)
    {
        if (is_free(path))
            return path;
        strcat(path, ".$counter");
    } else
    {
        char *zc = replace_counter_with_number(path, 0);
        if (is_free(zc))
            return zc;
    }
    int current_counter = opt_counter_first;
    char *current_name = replace_counter_with_number(path, current_counter);
    while (!is_free(current_name))
    {
        current_counter++;
        free(current_name);
        current_name = replace_counter_with_number(path, current_counter);
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

    int c;
    while ((c = getopt_long(argc, argv, "hv", long_opts, NULL)) != -1)
    {
        switch (c)
        {
            case 'h':
            case OPT_HELP:
                usage();
                return 0;
            case 'v':
                version();
                return 0;
            case '?':
                usage();
                return 2;
        }
    }

    printf("%s\n", find_name(argv[optind]));
    return 0;
}

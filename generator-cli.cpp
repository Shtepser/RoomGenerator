#include "yaml.h"

#include <cstdio>
#include <cstdlib>

#include "random.hpp"
#include "RoomContent.hpp"


enum ParseStatus {SUCCESS, INVALID_ARGUMENTS, INVALID_N};


void parse_arguments(int argc, char** argv,
                     unsigned int *n, ParseStatus *status)
{
    char *endptr;
    long n_argument;
    
    if (argc != 2)
    {
        *status = INVALID_ARGUMENTS;
        return;
    }
    n_argument = strtol(argv[1], &endptr, 10);
    if (endptr == argv[1] || n_argument < 0 || *endptr != '\0')
    {
        *status = INVALID_N;
        return;
    }
    *n = n_argument;
    *status = SUCCESS;
}

int main(int argc, char** argv)
{
    ParseStatus arguments_status = SUCCESS;
    unsigned int times = 0;

    parse_arguments(argc, argv, &times, &arguments_status);

    if (arguments_status == INVALID_ARGUMENTS)
    {
        printf("Usage: generator-cli N\n* N: number of rooms to generate content for\n");
        return 0;
    }
    if (arguments_status == INVALID_N)
    {
        printf("Invalid argument!\n");
        return 1;
    }
    
    init_random();
    
    for (unsigned int i = 0; i < times; i++)
    {
        RoomContent room = RoomContent();
        char* string_representation = room.toString();
        printf("%s\n", string_representation);
        delete[] string_representation;
    }

    return 0;
}


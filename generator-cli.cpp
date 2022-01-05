#include <cstdio>
#include <cstdlib>

#include "random.hpp"
#include "RoomContent.hpp"


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Usage: generator-cli N\n* N: number of rooms to generate content for\n");
        return 0;
    }

    char *endptr;
    int times_argument = strtol(argv[1], &endptr, 10);
    if (endptr == argv[1] || times_argument < 0 || *endptr != '\0')
    {
        printf("Invalid argument!\n");
        return 1;
    }

    unsigned int times = unsigned(times_argument);
    
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


#include <cstdio>
#include <cstring>

#include "RoomContent.hpp"
#include "random.hpp"

unsigned int RoomContent::createdRooms = 0;

RoomContent::RoomContent()
{
    unsigned short danger_roll = d6_roll();
    unsigned short treasure_roll = d6_roll();
    
    if (danger_roll <= 2)
    {
        hasMonsters = true;
        hasTrap = false;
    }
    else if (danger_roll == 3)
    {
        hasMonsters = false;
        hasTrap = true;
    }
    else
    {
        hasMonsters = false;
        hasTrap = false;
    }
      

    if (treasure_roll == 1 || (hasMonsters && treasure_roll <= 3))
        hasTreasure = true;
    else
        hasTreasure = false;

    createdRooms += 1;
    index = createdRooms;
}

char* RoomContent::toString() const
{
    const size_t MAX_SIZE = 128;
    char buffer[MAX_SIZE];
    const char* monsters = hasMonsters ? "has monsters" : "has no monsters"; 
    const char* trap = hasTrap ? "has trap" : "has no trap";
    const char* treasure = hasTreasure ? "has treasure" : "has no treasure";
    sprintf(buffer, "Room #%d: %s; %s; %s.",
            index, monsters, trap, treasure);
    char* result = new char[strlen(buffer)];
    strcpy(result, buffer);
    return result;
}


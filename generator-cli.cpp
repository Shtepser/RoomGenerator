#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>


void init_random()
{
    srand(time(NULL));
}


unsigned short d6_roll()
{
   return rand() % 6 + 1; 
}


class RoomContent
{
    unsigned int index;
    bool hasMonsters;
    bool hasTrap;
    bool hasTreasure;

    static unsigned int createdRooms;
    
    public:
    
        RoomContent()
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

        char* toString() const
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

        static void resetCreatedRoomsCounter()
        {
            createdRooms = 0;
        }

};


unsigned int RoomContent::createdRooms = 0;

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
}


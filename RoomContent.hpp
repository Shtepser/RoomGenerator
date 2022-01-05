#pragma once

class RoomContent
{
        unsigned int index;
        bool hasMonsters;
        bool hasTrap;
        bool hasTreasure;

        static unsigned int createdRooms;
    
    public:
    
        RoomContent();

        char* toString() const;

};


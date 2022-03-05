#pragma once

#include <stdint.h>

struct HitPointsVariant {
    uint16_t variant;

    HitPointsVariant *next;

    public:
        
        HitPointsVariant(const uint16_t variant);

};


struct Monster {
    char *name;
    uint16_t minimal_count;
    uint16_t maximal_count;
    int8_t armor_class;
    uint8_t hit_dices;
    uint8_t hit_dices_stars;
    HitPointsVariant *hit_points_variants;
    uint8_t attacks;
    char *damage;
    uint16_t movement_in_turn;
    uint16_t movement_in_round;
    char *saves_as;
    uint8_t morale;

    Monster *next;

    public:
    
        void print();
};


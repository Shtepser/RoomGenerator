#include <cstdio>

#include "Monster.hpp"


HitPointsVariant::HitPointsVariant(const uint16_t variant)
{
    this->variant = variant;
}


void Monster::print()
{
    printf("%s\n", name);
    printf("\t%d::%d\n", minimal_count, maximal_count);
    printf("\tAC %d\n", armor_class);
    printf("\tHD %d", hit_dices);
    for (int i = 0; i < hit_dices_stars; i++)
        printf("*");
    printf(" [");
    for (HitPointsVariant *v = hit_points_variants; v != NULL; v = v->next)
        printf("%d ", v->variant);
    printf("]\n");
    const char* attack_string = (attacks == 1 ? "attack" : "attacks");
    printf("\t%d %s; damage %s\n", attacks, attack_string, damage);
    printf("\tMovement %d/turn; %d/round\n", movement_in_turn, movement_in_round);
    printf("\tSaves as %s\n", saves_as);
    printf("\tMorale %d\n", morale);
}


#include <cstdlib>
#include <ctime>


void init_random()
{
    srand(time(NULL));
}


unsigned short d6_roll()
{
   return rand() % 6 + 1; 
}


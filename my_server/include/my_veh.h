#include "stdint.h"
#include "math.h"
#include <tf/transform_listener.h>

typedef struct st_vehicle
{
    /* data */
    uint8_t ui8_prio = 1;
};

int set_prio(int veh_num, int prio); // set priority of vehicle. the bigger prio thi higher prioriry 
int get_prio(int veh_num);
int is_available(tf::StampedTransform p1, tf::StampedTransform p2, float min_dist); // check distance between 2 vehicle available
// return 1 if distace > min dist. 0 if it can cause acident


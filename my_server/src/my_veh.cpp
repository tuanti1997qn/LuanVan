#include "my_prio.h"

st_vehicle my_vehicle[3];

int set_prio(int veh_num, int prio)
{
    (my_vehicle + veh_num - 1)->ui8_prio = prio;
}

int get_prio(int veh_num)
{
    return (my_vehicle + veh_num - 1)->ui8_prio;
}

int is_available(tf::StampedTransform p1, tf::StampedTransform p2, float min_dist)
{
    float sub_1, sub_2, dist;

    sub_1 = p1.getOrigin().x() - p2.getOrigin().x();
    sub_2 = p1.getOrigin().y() - p2.getOrigin().y();

    sub_1 *= sub_1;
    sub_2 *= sub_2;

    dist = sub_1 + sub_2;
    min_dist *= min_dist;

    return (dist > min_dist);
}


#include "math.h"
#include <tf/transform_listener.h>

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

int set_prio( int num, int prio )
{
    
}
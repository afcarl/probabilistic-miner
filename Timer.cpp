#include "Timer.h"

/// Constructor
Timer :: Timer ()
{
 microsec_ = 1000000.0;
}


/// Start timer
void Timer:: start_timer ()
{
 gettimeofday (&start_time_, (struct timezone *) 0);
 st_sec_ = start_time_.tv_sec + (start_time_.tv_usec/microsec_) ;
}


/// End timer
void Timer:: end_timer ()
{
 gettimeofday (&stop_time_, (struct timezone *) 0);
 end_sec_ = stop_time_.tv_sec + (stop_time_.tv_usec/microsec_) ;
}


/// Get the elapsed time
double Timer:: get_time_in_secs ()
{
 return (end_sec_ - st_sec_);
}

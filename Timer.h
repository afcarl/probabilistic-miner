#ifndef TIMER_H
#define	TIMER_H

#include <sys/time.h>
#include <unistd.h>


class Timer{
public:
 /// constructor
 Timer ();

 /// Start the timer
 void start_timer ();

 /// End the timer
 void end_timer ();

 /// Get the elapsed time in secs from end to start
 double get_time_in_secs ();
 
private: 
 /// Start time
 struct timeval start_time_;

 /// End time
 struct timeval stop_time_;

 /// Elapsed sec up to start point
 double st_sec_;

 /// Elapsed sec up to end point
 double end_sec_;

 /// Microsec value 
 double microsec_;
};

#endif	/* TIMER_H */


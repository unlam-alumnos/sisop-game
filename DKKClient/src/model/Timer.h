/*
 * Timer.h
 *
 *  Created on: Nov 1, 2014
 *      Author: cristianmiranda
 */

#ifndef MODEL_TIMER_H_
#define MODEL_TIMER_H_

#include "../core/Core.h"
#include "../common/Constants.h"

//The timer
class Timer
{
    private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

    public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};

#endif /* MODEL_TIMER_H_ */

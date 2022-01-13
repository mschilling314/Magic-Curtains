#include "algorithm.h"
#include "nrf_delay.h"

bool downTest(struct machine_State* sta){
    // manual override
    if (sta->butA == 0){
        sta->priority = 2;
        return true;
    }

    // test both time and temperature
    if (!(sta->hour == 8 && sta->minute < 15) && sta->priority < 2){
        if (sta->temp > 27 || sta->light > 3.1) {
            sta->priority = 1;
            return true;
        }
        if (sta->hour >= 23 || sta->hour < 12) return true;
    }

    // if none of the above, then no state change needed
    return false;
}

bool upTest(struct machine_State* sta){
    // manual override
    if (sta->butB == 0) {
        sta->priority = 2;
        return true;
    }

    // test to see if the hour is ripe, and that downTest is false
    if (sta->priority == 0 && 12 <= sta->hour && sta->hour < 23 && !downTest(sta)) return true;
    if (sta->priority == 0 && 8 == sta->hour && 15 > sta->minute) return true;
    // if none of the above, then no state change needed
    return false;
}

void trans_set(struct machine_State* sta, motor_direction mo){
    light_turn_off(Green);
    // first calculate when the transistion should end
    // currently assume transition takes 30 seconds
    sta->endTransSec = (sta->second + STATE_SHIFT) % 60;
    if ((sta->second + STATE_SHIFT) / 60 == 1) sta->endTransMin = (sta->minute + 1) % 60;
    else sta->endTransMin = sta->minute;

    // additionally turn on a color for troubleshooting
    // red for up, blue for down
    // and also set dest
    if (mo == Forward) {
        light_turn_on(Blue);
        sta->dest = Down;
    }
    else if (mo == Reverse) {
        light_turn_on(Red);
        sta->dest = Up;
    }

    // lastly, update state to indicate the curtains are in trans.
    sta->cur = Transing;
}

void print_state(struct machine_State* state){
    // print out the state of the machine
    printf("Button A: %u\n", !state->butA);
    printf("Button B: %u\n", !state->butB);
    printf("Light Sensor State: %f \n",state->light);
    printf("Temperature: %f \n", state->temp);
    //printf("Curtain State: %u\n", state->cur);
    //printf("Dest State: %u\n", state->dest);
    printf("Time: %u:%u:%u\n", state->hour,
                                      state->minute,
                                      state->second);
    printf("Priority: %u\n", state->priority);
    //printf("Last End Time: %u:%u", state->endTransMin,
    //                                      state->endTransSec);
    printf("\n");  // for the lols
}

void algo(struct machine_State* state){
    if (state->priority == 2 && (!state->butA || !state->butB)) state->priority = 0;
    if (state->priority == 1 && state->temp < 30 && state->light < 3) state->priority = 0;

    // first, branch based on current state
    if (state->cur != Transing){
        // this is reached if the curtains aren't transitioning
        // meaning, if the curtains are up, eval if they need to
        // go down and vice versa
        if (state->cur == Up){
            // this will test to see if the curtain should go down
            if (downTest(state)){
                // update state to reflect curtain going down
                trans_set(state, Forward);
            }
        }
        else if (state->cur == Down){
            // this will test if the curtain should go up
            if (upTest(state)){
                // update state to reflect curtain going up
                trans_set(state, Reverse);
            }
        }
    }
    else{
        // indicates states are transitioning
        // need to track when the transition should be complete
        // if at/past that time, stop transitioning
        if (state->second >= state->endTransSec && state->minute >= state->endTransMin){
            state->cur = state->dest;
            light_turn_off(Red);
            light_turn_off(Blue);
            light_turn_on(Green);
        }
    }
    print_state(state);
}
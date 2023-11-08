#include "Rover.h"

void ModeZK5AD::setRoverMovement(RoverMovement movement) {
    const char* message = nullptr;
    switch (movement) {
    case RoverMovement::MoveForward:
        message = "MOVE FORWARD";
        ap_relay->on(0);
        ap_relay->off(1);
        ap_relay->on(2);
        ap_relay->off(3);
        break;
    case RoverMovement::MoveBack:
        message = "MOVE BACK";
        ap_relay->off(0);
        ap_relay->on(1);
        ap_relay->off(2);
        ap_relay->on(3);
        break;
    case RoverMovement::TurnRight:
        message = "TURN RIGHT";
        ap_relay->off(0);
        ap_relay->on(1);
        ap_relay->on(2);
        ap_relay->off(3);
        break;
    case RoverMovement::TurnLeft:
        message = "TURN LEFT";
        ap_relay->on(0);
        ap_relay->off(1);
        ap_relay->off(2);
        ap_relay->on(3);
        break;
    case RoverMovement::Stop:
        message = "STOP";
        ap_relay->off(0);
        ap_relay->off(1);
        ap_relay->off(2);
        ap_relay->off(3);
        break;
    }
    if (message) {
        GCS_SEND_TEXT(MAV_SEVERITY_INFO, "%s", message);
    }

}
void ModeZK5AD::update() {

    // Exit immediately if no relay is setup
    if (!ap_relay) {
        return;
    }
    int16_t roll_out = RC_Channels::rc_channel(0)->get_radio_in();
    int16_t pitch_out = RC_Channels::rc_channel(1)->get_radio_in();


    // Make movement decisions based on pitch and roll
    if (pitch_out > CENTER_PWM + _TRIM) {
        setRoverMovement(RoverMovement::MoveForward);
    } else if (pitch_out < CENTER_PWM - _TRIM) {
        setRoverMovement(RoverMovement::MoveBack);
    } else if (abs(pitch_out - CENTER_PWM) <= _TRIM) {
        if (roll_out > CENTER_PWM + _TRIM) {
            setRoverMovement(RoverMovement::TurnRight);
        } else if (roll_out < CENTER_PWM - _TRIM) {
            setRoverMovement(RoverMovement::TurnLeft);
        } else {
            setRoverMovement(RoverMovement::Stop);
        }
    }
}


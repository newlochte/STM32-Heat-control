#include "pid_config.h"

PID_TypeDef pid = {
    .state = PID_AUTO,
    .dt = 1.0,
    .u = 0.0,
    .kp = 1.2,
    .ki = 0.02,
    .kd = 0.0,
    .integral = 0.0,
    .prev_error = 0.0,
    .abs_max_signal = 1.0};
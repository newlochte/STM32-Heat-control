#include "pid_config.h"

PID_TypeDef pid = {
    .state = PID_AUTO,
    .dt = 0.2,
    .u = 0.0,
    .kp = 12,
    .ki = 0.2,
    .kd = 0.0,
    .integral = 0.0,
    .prev_error = 0.0,
    .abs_max_signal = 10.0};
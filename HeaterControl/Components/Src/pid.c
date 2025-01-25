#include "pid.h"
// #include <math.h>

float PID_calulate_signal(PID_TypeDef *controller, float error)
{
  float proportional = controller->kp * error;
  float derivative = controller->kd * (error - controller->prev_error) / controller->dt;
  // if (fabs(controller->u) < controller->abs_max_signal)                            // anty wind-up
  //   controller->integral += (error + controller->prev_error) * controller->dt / 2; // metoda trapezów
  float integral = controller->ki * controller->integral;

  float signal = proportional + derivative + integral;

  // Saturacja
  if (signal > controller->abs_max_signal)
    return controller->abs_max_signal;
  if (signal < -controller->abs_max_signal)
    return -controller->abs_max_signal;
  else
    return signal;
}

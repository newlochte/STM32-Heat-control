#ifndef INC_PID_H_
#define INC_PID_H_
// #include <math.h>

enum PID_state
{
  PID_MANUAL,
  PID_AUTO
};

typedef struct
{
  enum PID_state state;
  float dt;             // czas probkowania Tp=1 s
  float u;              // sygnal pid (przed nasyceniem)
  float kp;             // parametr PID Kp
  float ki;             // parametr PID Ki
  float kd;             // parametr PID Kd
  float integral;       // akumulator całki
  float prev_error;     // sygnał w poprzedniej próbce
  float abs_max_signal; // wartość saturacji
} PID_TypeDef;

float PID_calulate_signal(PID_TypeDef *controller, float error);

#endif
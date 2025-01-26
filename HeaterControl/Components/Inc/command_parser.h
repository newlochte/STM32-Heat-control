#ifndef INC_CMD_PARSER_H_
#define INC_CMD_PARSER_H_

#include <stdlib.h>
#include <stdint.h>

#define COMMAND_OPERAND_POSITION 0
#define COMMAND_COMMAND_POSITION 2
#define COMMAND_LENGTH 4

enum Command_type
{
  BAD_ARG = -1,
  READ_TEMPERATURE = 0,
  READ_FAN_PWM,
  READ_RESISTOR_PWM,
  READ_SETPOINT,
  WRITE_FAN_PWM,
  WRITE_RESISTOR_PWM,
  WRITE_SETPOINT,
  WRITE_PID_STATE,
};

typedef struct
{
  enum Command_type type;
  float value;
} Command_TypeDef;

Command_TypeDef parse_commmand(uint8_t *command, size_t length);

#endif
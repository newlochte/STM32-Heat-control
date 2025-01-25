#include "command_parser.h"
// #include <stdint.h>
Command_TypeDef parse_commmand(uint8_t *command, size_t length)
{
  Command_TypeDef cmd;
  if (length < COMMAND_LENGTH)
  {
    cmd.type = BAD_ARG;
    return cmd;
  }
  switch (command[COMMAND_OPERAND_POSITION])
  {
  case 'r':
  case 'R':
  {
    switch (command[COMMAND_COMMAND_POSITION])
    {
    case 't':
    case 'T':
      cmd.type = READ_TEMPERATURE;
      break;
    case 'f':
    case 'F':
      cmd.type = READ_FAN_PWM;
      break;
    case 'r':
    case 'R':
      cmd.type = READ_RESISTOR_PWM;
      break;
    case 's':
    case 'S':
      cmd.type = READ_SETPOINT;
      break;
    default:
      cmd.type = BAD_ARG;
      return cmd;
    }
  }
  break;
  case 'w':
  case 'W':
  {
    switch (command[COMMAND_COMMAND_POSITION])
    {
    case 'f':
    case 'F':
      cmd.type = WRITE_FAN_PWM;
      break;
    case 'r':
    case 'R':
      cmd.type = WRITE_RESISTOR_PWM;
      break;
    case 's':
    case 'S':
      cmd.type = WRITE_SETPOINT;
      break;
    default:
      cmd.type = BAD_ARG;
      return cmd;
    }
  }
  break;
  default:
    cmd.type = BAD_ARG;
    return cmd;
  }
  cmd.value = atof((char *)(command + COMMAND_LENGTH));
  return cmd;
}
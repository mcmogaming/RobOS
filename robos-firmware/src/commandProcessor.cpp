#include "commandProcess.h"

void processCommand(char cmd[MAX_NUM_CMDS][MAX_CMD_LENGTH]){

    if(strcmp(cmd[0], "motor") == 0){
       Serial.printf("Command Motor %d %d %s %s %s\n", cmd, &cmd[1][0] ,&cmd[0][0], &cmd[1][0], &cmd[2][0]);
       setMotorPower(String(cmd[1]).toInt(),String(cmd[2]).toInt());
    }

}
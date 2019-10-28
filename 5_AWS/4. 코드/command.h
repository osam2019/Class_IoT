#include"make.h"

void processingOP(const char* , const char* , SHELF** , char* , char* , char* , int* , int* , int* , const int );
void commandInput(const char* , SHELF** , char* , char* , char* , int* , int* , int* );
void commandSet(const char* , char* , char* , int* , int* );
void commandFind(const char* , char* , char* , int* , int* , const int );
void commandOutting(const char* );
void commandShow(const char* );

// 명령어 처리 메인
void commandProcessing(char *command, SHELF **shelves, const int num) {
  char opcode[10] = {0}; 
  char value1[10] = {0};
  char value2[10] = {0};
  char value3[100] = {0};
  char temp = 0;
  int index = 0;
  int index_op = 0;
  int index_v1 = 0;
  int index_v2 = 0;
  int index_v3 = 0;
  while(command[index] != ' ') {
    if(command[index] == 0) break;
    opcode[index_op] = command[index];
    index++;
    index_op++;
  }
  index++;
  while(command[index] != ' ') {
    if(command[index] == 0) break;
    value1[index_v1] = command[index];
    index++;
    index_v1++;
  }
  index++;
  processingOP(opcode, command, shelves, value1, value2, value3, &index, &index_v2, &index_v3, num);
}


// OPCODE 처리
void processingOP(const char *opcode, const char *command, SHELF **shelves, char *value1, char *value2, char *value3, int *index, int *index_v2, int *index_v3, const int num){
  if(strcmp(opcode, "INPUT") == 0) 
    commandInput(command, shelves, value1, value2, value3, index, index_v2, index_v3);
  else if(strcmp(opcode, "SET") == 0)
        commandSet(command, value1, value2, index, index_v2);
    else if(strcmp(opcode, "FIND") == 0)
        commandFind(command, value1, value2, index, index_v2, num);
    else if(strcmp(opcode, "OUTTING") == 0)
        commandOutting(value1);
    else if(strcmp(opcode, "SHOW") == 0)
        commandShow(value1);
    else {
    bluetooth.write("op code error");
  }
}

// 입력 명령 처리
void commandInput(const char *command, SHELF **shelves, char *value1, char *value2, char *value3, int *index, int *index_v2, int *index_v3){
    while(command[*index] != ' ') {
      if(command[*index] == 0) break;
      value2[*index_v2] = command[*index];
      (*index)++;
      (*index_v2)++;
    }
    (*index)++;
    while(command[*index] != ' ') {
      if(command[*index] == 0) break;
      value3[*index_v3] = command[*index];
      (*index)++;
      (*index_v3)++;
    }
    if(strcmp(value1, "NAME") == 0) {
      if(strcmp(value2, "shelf0") == 0) strcpy(shelves[0]->name, value3);
      if(strcmp(value2, "shelf1") == 0) strcpy(shelves[1]->name, value3);
    } else if(strcmp(value1, "SIZE") == 0) {
      if(strcmp(value2, "shelf0") == 0) shelves[0]->size = atoi(value3);
      if(strcmp(value2, "shelf1") == 0) shelves[1]->size = atoi(value3);
    }
}

// 설정 명령 처리
void commandSet(const char *command, char *value1, char *value2, int *index, int *index_v2){
  while(command[*index] != ' ') {
    if(command[*index] == 0) break;
    value2[*index_v2] = command[*index];
    (*index)++;
    (*index_v2)++;
  }
  if(strcmp(value1, "TMP") == 0) limit_temperature = atoi(value2); else if(strcmp(value1, "CYCLE") == 0) cycle = atoi(value2);
}

// 찾기 명령 처리
void commandFind(const char *command, char *value1, char *value2, int *index, int *index_v2, const int num){
    if(strcmp(value1, "NAME") == 0) {
    SHELF *find_shelf = NULL;
    while(command[*index] != '\n') {
      if(command[*index] == 0) break;
      value2[*index_v2] = command[*index];
      (*index)++;
      (*index_v2)++;
    }
    find_shelf = find_name(shelves, num, value2);
    if(find_shelf != NULL) {
      show_info(find_shelf);
      for (i = 0; i < 5; i++) {
        setLED(find_shelf->led, 255, 255);
        delay(500);
        setLED(find_shelf->led, 0, 0);
        delay(500);
      }
    } else {
      bluetooth.write("not find");
    }
    } else if(strcmp(value1, "SPACE") == 0) {
      findspace(shelves, num);
    }
}

// 외출(방범) 명령 처리
void commandOutting(const char *value1){
  if(strcmp(value1, "ON") == 0) {
    outting = 1;
    bluetooth.write("outting on\n");
  } else if(strcmp(value1, "OFF") == 0) {
    outting = 0;
    bluetooth.write("outting off\n");
  }
}

// 현재 온도 표시 명령 처리
void commandShow(const char* value1){
  if(strcmp(value1, "TMP") == 0) {
    sprintf(temps, "Temperature : %d°C \n", temperature);
    bluetooth.write(temps);
  }
}

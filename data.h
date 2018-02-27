// Header File Information /////////////////////////////////////
/**
* @file data.h
*
* @brief used for storing structs
*
* @version 1.00
* Sim01
*
*/

#ifndef DATA_H
#define DATA_H

#define MONITOR_LOG 1
#define FILE_LOG 2
#define BOTH_LOG 3

//structure for storing the configInfo
struct configInfo
{
  int versNum;
  char filePath[255];
  char cpuScheduleCode[10];
  int quantTime;
  int memory;
  int procCycle;
  int ioCycle;
  int logLocation;
  char logPath[255];
};

struct process
{
  char compLetter;
  char operation[18];
  int cycleTime;
  int memory;

  struct process *nextProcess;
};

#endif

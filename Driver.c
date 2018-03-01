// Program Information ////////////////////////////////////////////////
/**
 * @file Driver.c
*
* @brief Main driver
*
*
* @version 1.00
* Sim01
*
* @note Requires Util.h, data.h, simtimer.h, FileOps.h
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "simtimer.h"
#include "data.h"
#include "FileOps.h"
#include "Util.h"

int main( int arg, const char *args[])
{
    
  char buffer[10];
  printf("Time: 0.000000, System start \n");
  //timer used from Michaels example, slightly modified to work
  accessTimer( ZERO_TIMER, buffer );

  FILE *config, *mdf;

  //creates new config struct to pass in and store data
  struct configInfo *configData = malloc(sizeof(struct configInfo));

  struct process *processHead = malloc(sizeof(struct process));

  //struct queueNode *queueHead = malloc(sizeof(struct queueNode));

  int configReturn, mdfReturn;

  //checking for correct number of args
  if( arg != 2 )
  {
    printf("Invalid Number of args, input a config file \n");

    return 0;
  }
  
  double curTime = accessTimer( LAP_TIMER, buffer );
  printf("Time: %f, OS: Begin PCB Creation \n", curTime);
  
  //open args[1] since args[0] is the executable name
  //r for read
  config = fopen(args[1], "r");

  configReturn = configRead( config, configData );

  switch( configReturn )
  {
    case CONFIG_FILE_ERROR:
      printf("Config file does not exist\n");
      fclose( config );
      return 0;

    case MALFORMED_CONFIG_FILE:
      printf( "Malformed Configuration File \n" );
      fclose( config );
      return 0;

    case INVALID_VERSION_NUMBER:
      printf("Version number invalid\n");
      fclose( config );
      return 0;

    case INVALID_CPU_SCHEDULE:
      printf("Invalid CPU schedule code\n");
      fclose( config );
      return 0;

    case INVALID_QUANT_TIME:
      printf("Invalid Quantum Time\n");
      fclose( config );
      return 0;

    case INVALID_MEMORY_SIZE:
      printf("Invalid Memory Amount\n");
      fclose( config );
      return 0;

    case INVALID_PROCESS_TIME:
      printf("Invalid Process Time\n");
      fclose( config );
      return 0;

    case INVALID_IO_TIME:
      printf("Invalid I/O Time\n");
      fclose( config );
      return 0;

  }

  mdf = fopen(configData->filePath, "r");

  mdfReturn = metaDataRead( mdf, processHead );

  switch( mdfReturn )
  {
    case MALFORMED:
      printf( "Malformed Meta Data File \n");
      return 0;
  }

  curTime = accessTimer( LAP_TIMER, buffer );
  printf("Time: %f, OS: All processes initialized in New state \n", curTime);
  
  
  curTime = accessTimer( LAP_TIMER, buffer );
  printf("Time: %f, OS: All processes initialized in Ready state \n", curTime);
  
  curTime = accessTimer( LAP_TIMER, buffer );
  printf("Time: %f, OS: ", curTime);
  printf("%s Strategy selects Process 0 with time: ", configData -> cpuScheduleCode);
  printf("%f \n", curTime);
  
  struct process *curPro = processHead;
  
  int proCycle = configData -> procCycle;
         
  while(curPro != NULL)
  {
    switch(curPro -> compLetter)
    {
      case 'S':
           runTimer(curPro -> cycleTime * proCycle);
           curTime = accessTimer( LAP_TIMER, buffer );
           printf("Time: %f, OS: task completed \n", curTime);
           //OSTask(curPro, queueHead);
           break;
           
      case 'A':
           runTimer(curPro -> cycleTime * proCycle);
           curTime = accessTimer( LAP_TIMER, buffer );
           printf("Time: %f, Process 0: application \n", curTime);
           //AppTask(curPro, queueHead);
           break;
           
      case 'P':
           runTimer(curPro -> cycleTime * proCycle);
           curTime = accessTimer( LAP_TIMER, buffer );
           printf("Time: %f, Process 0: process \n", curTime);
           //ProTask(curPro, queueHead);
           break;
           
      case 'M':
           runTimer(20);
           curTime = accessTimer( LAP_TIMER, buffer );
           printf("Time: %f, Process 0: Memory \n", curTime);
           //MemTask(curPro, queueHead);
           break;
           
      case 'I':
           printf(" ");
           pthread_t threadI;
           pthread_attr_t threadAttribI;
           pthread_attr_init(&threadAttribI);
           
           struct queueNode *newNodeI = createQueueNode("IO: process", configData -> ioCycle, curPro);
           
           pthread_create(&threadI, &threadAttribI, IORunner, &newNodeI);
           pthread_join(threadI, NULL);
           
           break;
           
      case 'O':
           printf(" ");
           pthread_t threadO;
           pthread_attr_t threadAttribO;
           pthread_attr_init(&threadAttribO);
           
           struct queueNode *newNodeO = createQueueNode("IO: process", configData -> ioCycle, curPro);
           
           pthread_create(&threadO, &threadAttribO, IORunner, &newNodeO);
           pthread_join(threadI, NULL);
           
           break;
    }
    
    curPro = curPro -> nextProcess;
  }
  
  free( configData );

  deleteProcessList( processHead );
  
  printf("test");
  
  return 0;
}

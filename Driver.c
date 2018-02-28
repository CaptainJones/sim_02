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
  
  struct queueNode *queueHead = NULL;
         
  while(curPro != NULL)
  {
    switch(curPro -> compLetter)
    {
      case 'S':
           OSTask(curPro, queueHead);
           break;
      case 'A':
           AppTask(curPro, queueHead);
           break;
      case 'P':
           ProTask(curPro, queueHead);
           break;
      case 'M':
           MemTask(curPro, queueHead);
           break;
      case 'I':
           InTask(curPro, queueHead);
      case 'O':
           OutTask(curPro, queueHead);
           break;
    }
    
    outPut(queueHead);
    curPro = curPro -> nextProcess;
  }
  
  free( configData );

  deleteProcessList( processHead );
  
  return 0;
}

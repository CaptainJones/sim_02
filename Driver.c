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
  FILE *config, *mdf;

  char buffer[10];

  double configTime = 0, processTime = 0;

  //creates new config struct to pass in and store data
  struct configInfo *configData = malloc(sizeof(struct configInfo));

  struct process *processHead = malloc(sizeof(struct process));

  int configReturn, mdfReturn;

  //checking for correct number of args
  if( arg != 2 )
  {
    printf("Invalid Number of args, input a config file \n");

    return 0;
  }

  //timer used from Michaels example, slightly modified to work
  accessTimer( ZERO_TIMER, buffer );


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

  configTime = accessTimer( LAP_TIMER, buffer );
  accessTimer( ZERO_TIMER, buffer );


  //accessTimer( LAP_TIMER, buffer );

  mdf = fopen(configData->filePath, "r");

  mdfReturn = metaDataRead( mdf, processHead );

  switch( mdfReturn )
  {
    case MALFORMED:
      printf( "Malformed Meta Data File \n");
      return 0;
  }

  processTime = accessTimer( LAP_TIMER, buffer );

  logFunc( configData, processHead, configTime, processTime );

  free( configData );

  deleteProcessList( processHead );

  return 0;
}

// Program Information ////////////////////////////////////////////////
/**
 * @file Util.c
*
* @brief Utilites for other functions
*
*
* @version 1.00
* Sim01
*
* @note Requires Util.h, data.h
*/


#include <stdio.h>
#include <stdlib.h>
#include "Util.h"
#include "data.h"

const char CFGDELIMITER = ':';

const char MDFDELIMITER = ';';

//extracts the setting from config file line, regardless of which line it is
//returns the data via the output pointer
void extractInfo( char *input, char *output )
{
  int increment = 0;
  int outputLen = 0;

  int stringlen = strlength( input );

  //finds where the delimiter is (:)
  while( input[ increment ] != CFGDELIMITER )
  {
    increment++;
  }

  //add two to skip space and delimiter
  increment += 2;

  //printf("outputLen: %d  increment %d \n", outputLen, increment );

  while( increment < stringlen )
  {
    output[ outputLen++ ] = input[ increment++ ];
  }

  //format value back to string
  output[ outputLen ] = '\0';

}

int extractProcess(char *line, int startOfProcess, int endOfProcess, struct process *node)
{
  int increment, startIncrement, closeBracketIdx, parsed;

  const char openBracket = '(', closeBracket = ')',
       semiColon = ';', period = '.';

  char strInt[12];

  node->compLetter = line[ startOfProcess ];

  //check for opening bracket
  if( line[startOfProcess + 1] != openBracket )
  {
    return MALFORMED;
  }

  increment = startOfProcess;

  //finds closing bracket
  while( line[ increment ] != semiColon )
  {
    if( line[ increment ] == closeBracket || line[ increment ] == period )
    {
      closeBracketIdx = increment;

      break;
    }
    if( increment >= endOfProcess )
    {
      return MALFORMED;
    }
    increment++;
  }

  //gets number at end of process and parses
  startIncrement = 0;

  for( increment = closeBracketIdx + 1; increment < endOfProcess; increment++ )
  {
    //check if end is actually number
    if( line[ increment ] < '0' || line[ increment ] > '9' )
    {
      return MALFORMED;
    }

    strInt[startIncrement++ ] = line[ increment ];
  }

  parsed = parseInt( strInt );

  startIncrement = 0;

  for( increment = startOfProcess + 2; increment < closeBracketIdx; increment++ )
  {
    node->operation[ startIncrement++ ] = line[ increment ];
  }

  //end of string
  node->operation[ startIncrement ] = '\0';

  switch( node->compLetter )
  {
    case 'S':
      if( strCompare( node->operation, "end" ) == 0 )
      {
        node->cycleTime = parsed;
        return END_OF_METADATA;
      }
    case 'M':
      node->memory = parsed;
      break;
  }

  node->cycleTime = parsed;

  return CORRECT;
}

int checkMalformedConfig( FILE* config )
{
  char inputBuffer[255];

  int error = 0;

  //first line ignored
  fgets( inputBuffer, sizeof( inputBuffer ), config );

  //Version Check
  error += checkMalformedConfigHelper( "Version/Phase:", config );

  //FilePath check
  error += checkMalformedConfigHelper( "File Path:", config );

  //CPU Schedule check
  error += checkMalformedConfigHelper( "CPU Scheduling Code:", config );

  //Quantum check
  error += checkMalformedConfigHelper( "Quantum Time (cycles):", config );

  //Memory check
  error += checkMalformedConfigHelper( "Memory Available (KB):", config );

  //Processor Cycle time check
  error += checkMalformedConfigHelper( "Processor Cycle Time (msec):", config );

  //I/O Cycle Time check
  error += checkMalformedConfigHelper( "I/O Cycle Time (msec):", config );

  //log to check
  error += checkMalformedConfigHelper( "Log To:", config );

  //Log File Path check
  error += checkMalformedConfigHelper( "Log File Path:", config );

  // makes fgets start from beginning
  rewind( config );

  if( error == 0 )
  {
    return CORRECT;
  }
  return MALFORMED;
}

int checkMalformedConfigHelper( char *lineToCheck, FILE* config )
{
  char inputBuffer[255];

  fgets( inputBuffer, sizeof( inputBuffer ), config );

  if( strCompareToDelim( inputBuffer, lineToCheck, CFGDELIMITER ) != 0 )
  {
    return MALFORMED;
  }

  return CORRECT;
}

//finds length of string, until \n
int strlength( char input[] )
{
  int length = 0;

  while( input[length] != '\n' )
  {
    length++;
  }

  return length;
}

void strCopy ( char *input, char *output )
{
  int increment;

  int size = sizeof( input );

  for( increment = 0; increment < size; increment++ )
  {
    output[ increment ] = input[ increment ];
  }

  //show end of string
  output[ increment ] = '\0';

}

//found from https://stackoverflow.com/questions/14232990/comparing-two-strings-in-c
int strCompare( char *oneStr, char *otherStr )
{
  int increment = 0;

  while( oneStr[ increment ] == otherStr [ increment ] )
  {
    if( oneStr [ increment ] == '\0' || otherStr[ increment ] == '\0' )
    {
      break;
    }
    increment++;
  }

  if( oneStr[ increment ] == '\0' && otherStr[ increment ] == '\0')
  {
    return 0;
  }

  return 1;
}

//found from https://stackoverflow.com/questions/14232990/comparing-two-strings-in-c
int strCompareToDelim( char *oneStr, char *otherStr, char DELIMITER )
{
  int increment = 0;

  while( oneStr[ increment ] == otherStr [ increment ] )
  {
    if( oneStr [ increment ] == DELIMITER || otherStr[ increment ] == DELIMITER )
    {
      break;
    }
    increment++;
  }

  if( oneStr[ increment ] == DELIMITER && otherStr[ increment ] == DELIMITER )
  {
    return 0;
  }

  return 1;
}

int parseInt( char *str )
{
  //increment for loop
  int increment;
  //finds length of
  int lenOfStrParse = lenOfIntStr( str );
  //used for a power function, parsing int sort of using scientific notation
  //as in for 56, 5 is multiplied by 10, then add 6 to it
  int currentPower = lenOfStrParse - 1;

  int output = 0;

  for( increment = 0; increment < lenOfStrParse; increment++ )
  {
    output = output + (str[ increment ] - '0') * powerOf( 10, currentPower--);
  }

  return output;
}

int lenOfIntStr( char *str )
{
  int increment = 0;

  while( str[ increment ] >= '0' && str[ increment ] <= '9' )
  {
    increment++;
  }

  return increment;
}

int powerOf( int base, int power )
{
  if( power == 0 )
  {
    return 1;
  }
  return base * powerOf( base, power - 1 );
}

int cpuCodeCheck( char *cpuCode, struct configInfo *configStruct )
{
int cpuError = CPUSCHEDULEERROR;

  //CPU code checks
  if( strCompare( cpuCode, "NONE") == 0 )
  {
    cpuError = CPUSCHEDULECORRECT;

    strCopy("FCFS-N", configStruct->cpuScheduleCode);
  }

  if( strCompare( cpuCode, "FCFS-N") == 0 )
  {
    cpuError = CPUSCHEDULECORRECT;

    strCopy("FCFS-N", configStruct->cpuScheduleCode);
  }

  //TODO Error here?
  if( strCompare( cpuCode, "SJF-N") == 0 )
  {
    cpuError = CPUSCHEDULECORRECT;

    strCopy("SJF-N", configStruct->cpuScheduleCode);
  }

  if( strCompare( cpuCode, "SRTF-P") == 0 )
  {
    cpuError = CPUSCHEDULECORRECT;

    strCopy("SRTF-P", configStruct->cpuScheduleCode);
  }

  if( strCompare( cpuCode, "FCFS-P") == 0 )
  {
    cpuError = CPUSCHEDULECORRECT;

    strCopy("FCFS-P", configStruct->cpuScheduleCode);
  }

  if( strCompare( cpuCode, "RR-P") == 0 )
  {
    cpuError = CPUSCHEDULECORRECT;

    strCopy("RR-P", configStruct->cpuScheduleCode);
  }

  return cpuError;
}

void strConcat( char *str, char *concatStr )
{
  int strLen = 0, concatStrLen = 0;

  //goes to end of first string
  while( str[ strLen ] != '\0')
  {
    strLen++;
  }

  while( concatStr[ concatStrLen ] != '\0' )
  {
    str[ strLen++ ] = concatStr[ concatStrLen++ ];
  }
}

void logFunc( struct configInfo *config, struct process *processList,
              double configTime, double processTime)
{
  FILE *logFile;

  if( config->logLocation == FILE_LOG || config->logLocation == BOTH_LOG )
  {
    //w+ to overwrite file if it exists
    logFile = fopen(config->logPath, "w+");
  }

  configLog( config, logFile, configTime );

  processLog( config, processList, logFile, processTime );
}

void configLog( struct configInfo *config, FILE *logFile, double configTime )
{
  char buffer[280];
  char logLocation[10];


  switch( config->logLocation )
  {
    case FILE_LOG:
      strCopy( "FILE", logLocation );
      break;
    case MONITOR_LOG:
      strCopy( "MONITOR", logLocation );
      break;
    case BOTH_LOG:
      strCopy( "BOTH", logLocation );
      break;
  }

  logLine( "Config File Dump\n", logFile, config );
  logLine( "================\n", logFile, config );

  sprintf( buffer, "Version            : %d\n", config->versNum );
  logLine( buffer, logFile, config );

  sprintf( buffer, "Program file name  : %s\n", config->filePath);
  logLine( buffer, logFile, config );

  sprintf( buffer, "CPU schedule code  : %s\n", config->cpuScheduleCode);
  logLine( buffer, logFile, config );

  sprintf( buffer, "Quantum time       : %d\n", config->quantTime);
  logLine( buffer, logFile, config );

  sprintf( buffer, "Memory size        : %d\n", (config->memory)/1000);
  logLine( buffer, logFile, config );

  sprintf( buffer, "Process cycle rate : %d\n", config->procCycle);
  logLine( buffer, logFile, config );

  sprintf( buffer, "I/O cycle rate     : %d\n", config->ioCycle);
  logLine( buffer, logFile, config );

  sprintf( buffer, "Log to code        : %s\n", logLocation);
  logLine( buffer, logFile, config );

  sprintf( buffer, "Log file name      : %s\n\n", config->logPath);
  logLine( buffer, logFile, config );

  sprintf( buffer, "Time for configuration upload: %f\n\n", configTime);
  logLine( buffer, logFile, config );

}

void processLog( struct configInfo *config, struct process *processList,
   FILE *logFile, double processTime )
{
  struct process *current = processList;

  char buffer[300];

  logLine( "Meta-Data File Dump\n", logFile, config );
  logLine( "===================\n", logFile, config );

  while( current != NULL )
  {
    /**
    The data item component letter is: S
    The data item operation string is: start
    The data item cycle time is : 0
**/
    sprintf( buffer, "The data item component letter is: %c\n", current->compLetter );
    logLine( buffer, logFile, config );

    sprintf( buffer, "The data item operation string is: %s\n", current->operation );
    logLine( buffer, logFile, config );

    sprintf( buffer, "The data item cycle time is : %d\n\n", current->cycleTime );
    logLine( buffer, logFile, config );

    current = current->nextProcess;
  }


  sprintf( buffer, "Time for configuration upload: %f\n", processTime);
  logLine( buffer, logFile, config );
}

void logLine( char * str, FILE *logFile, struct configInfo *config )
{
  if( config->logLocation == MONITOR_LOG || config->logLocation == BOTH_LOG )
  {
    printf("%s", str);
  }
  if( config->logLocation == FILE_LOG || config->logLocation == BOTH_LOG )
  {
    fputs( str, logFile );
  }

}

void deleteProcessList( struct process *processList )
{
  struct process *current = processList, *temp;

  while( current != NULL )
  {
    temp = current;

    free( current );

    current = temp->nextProcess;
  }
}

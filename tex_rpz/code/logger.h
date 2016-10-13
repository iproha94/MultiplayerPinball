/* 
 * File:   logger.h
 * Author: ilyapc-ubuntu
 */

#ifndef LOGGER_H
#define	LOGGER_H

#include <stdio.h>
#include <string.h>
#define MAX_LENGTH_STRING_TIME 14

class Logger {
public:
  Logger() {
    char str[MAX_LENGTH_STRING_TIME + 12] = "logger/";
    strcat(strcat(str, timeToString()), ".txt");
    filelog = fopen(str, "w");
  }
  
  ~Logger() {
    fclose(filelog);
  }
  
  FILE *filelog;

  char *timeToString() {
    time_t t = time(NULL);
    static char *str = new char[MAX_LENGTH_STRING_TIME];
    tm* aTm = localtime(&t);
    sprintf(str, "%02d_%02d_%02d:%02d:%02d", aTm->tm_mday, aTm->tm_mon + 1, aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
    return str;
  }
  
  void printlog(const char *str) {
    fprintf(filelog, "%s - ", timeToString());
    fprintf(filelog, str);
    fprintf(filelog, "\n");
    fflush(filelog);
  }
};

#endif	/* LOGGER_H */


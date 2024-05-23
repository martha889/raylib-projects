#ifndef SYSTEM_C
#define SYSTEM_C

#include <pthread.h>
#include <stdlib.h>

#include "globals.c"

typedef struct {
  pthread_t* threads;
  int count_threads;
} SystemInfo;


pthread_t* init_threads() {
  pthread_t* threads = malloc(sizeof(pthread_t) * MAX_THREADS);
  for (int i=0; i<MAX_THREADS; i++) {
    threads[i] = 0;
  }
  return threads;
}

SystemInfo* init_system_info() {
  SystemInfo* system_info = malloc(sizeof(SystemInfo));
  system_info->threads = init_threads();
  system_info->count_threads = 0;
  return system_info;
}

#endif

//
// Created by FionaLee on 2/18/19.
//

#include <stdio.h>
#include <pthread.h>
#include "TOF_API.h"

int main(int argc, char **argv) {
  printf("[MAIN] ready to init STMcom.\n");
  int status = stmcom_init();
  printf("status: %d\n", status);
  if (status < 0) {
    printf("failed to init com library, check connection.\n");
    return -1;
  }
  pthread_t comm_thread;
  printf("[MAIN] readu to create a thread.\n");
  pthread_create(&comm_thread, NULL, read_and_unpack_thread, NULL);
  printf("[MAIN] thread started.\n");


  while (1) {
    for (int i = 0; i < 6; i++) {
      printf("in main: %d,%d,\n", get_tof_idx(i),get_range_mm(i));
    // printf("in main: %d,%d,%d,%e,%e\n", i, get_range_mm(i), get_range_status(i), get_signal_rate(i), get_ambient_rate(i));
    }

  }


  pthread_join(comm_thread, NULL);

}
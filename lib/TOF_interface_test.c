//
// Created by FionaLee on 2/18/19.
//

#include <stdio.h>
#include "TOF_API.h"

int main(int argc, char **argv) {
  printf("In mian.\n");
  int status = stmcom_init();
  printf("status: %d\n", status);
  if (status < 0) {
    printf("failed to init com library, check connection.\n");
    return -1;
  }
//  read_and_unpack_thread(NULL);
  printf("%d\n",get_range_mm(1));
  printf("%e\n",get_signal_rate(1));
  printf("%e\n",get_ambient_rate(1));
  printf("%d\n",get_range_status(1));

}
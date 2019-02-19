//
// Created by FionaLee on 2/18/19.
//

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <stdlib.h>
#include <err.h>

#ifndef STMCOM_TTY
#define STMCOM_TTY "test.txt"
//#define STMCOM_TTY "/dev/ttyUSB1" // default tty
#endif

#define UART_BUFF_SIZE 500


typedef struct {
int TOF_INDEX;
int RANGE_STATUS;
int RANGE_MM;
float SIGNAL_RATE;
float AMBIENT_RATE;
} TOF_sensor_data;


static FILE *tty_file = 0;
static int num_of_TOF_sensors = 6;
static TOF_sensor_data* tof_sensor_data;

/* UART receive buffer */
static char computer_rx_buf[UART_BUFF_SIZE];

static int set_interface_attribs (int speed, int parity) {
  struct termios tty;
  memset(&tty, 0, sizeof tty);
//  if (tcgetattr(fileno(tty_file), &tty) != 0) {
//    err(1, "error from tcgetattr");
//    return errno;
//  }

  cfsetospeed(&tty, speed);
  cfsetispeed(&tty, speed);

  tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
  // disable IGNBRK for mismatched speed tests; otherwise receive break
  // as \000 chars
  tty.c_iflag &= ~IGNBRK;         // disable break processing
  tty.c_lflag = 0;                // no signaling chars, no echo,
  // no canonical processing
  tty.c_oflag = 0;                // no remapping, no delays
  tty.c_cc[VMIN]  = 0;            // read doesn't block
  tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

  tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
  // enable reading
  tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
  tty.c_cflag |= parity;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CRTSCTS;

//  if (tcsetattr(fileno(tty_file), TCSANOW, &tty) != 0) {
//    err(1, "error from tcsetattr");
//    return -1;
//  }
  return 0;
}

static void set_blocking(int should_block) {
  struct termios tty;
  memset(&tty, 0, sizeof tty);
//  if (tcgetattr(fileno(tty_file), &tty) != 0) {
//    err(1, "error from tggetattr");
//    return;
//  }

  tty.c_cc[VMIN]  = should_block ? 1 : 0;
  tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

//  if (tcsetattr(fileno(tty_file), TCSANOW, &tty) != 0) {
//    err(1, "error setting term attributes");
//  }
}

void update_TOF_reading(TOF_sensor_data *tof) {
  tof_sensor_data[tof->TOF_INDEX].RANGE_STATUS = tof->RANGE_STATUS;
  tof_sensor_data[tof->TOF_INDEX].RANGE_MM = tof->RANGE_MM;
  tof_sensor_data[tof->TOF_INDEX].SIGNAL_RATE = tof->SIGNAL_RATE;
  tof_sensor_data[tof->TOF_INDEX].AMBIENT_RATE = tof->AMBIENT_RATE;
}


void *read_and_unpack_thread(void *argu) {

  TOF_sensor_data tof;

  while (fgets(computer_rx_buf, UART_BUFF_SIZE, tty_file)) {
    sscanf(computer_rx_buf, "%d, %d, %d, %e, %e",
           &tof.TOF_INDEX, &tof.RANGE_STATUS, &tof.RANGE_MM,
           &tof.SIGNAL_RATE, &tof.AMBIENT_RATE);
    update_TOF_reading(&tof);
  }
  return NULL;
}

int stmcom_init(void) {
  tof_sensor_data = malloc(sizeof(tof_sensor_data) * num_of_TOF_sensors);
  tof_sensor_data[1].RANGE_STATUS = 213;
  tty_file = fopen (STMCOM_TTY, "r");
  if (tty_file == NULL) {
    err(1, "error opening usbtty");
  }
  set_interface_attribs (B115200, 0);  // set speed to 921600 bps, 8n1 (no parity)
  set_blocking (0);                // set no blocking
  read_and_unpack_thread(NULL);

  return 1;
}


int get_range_status(int tof_idx) {
  return tof_sensor_data[tof_idx].RANGE_STATUS;
}

int get_range_mm(int tof_idx) {
  return tof_sensor_data[tof_idx].RANGE_MM;
}

float get_signal_rate(int tof_idx) {
  return tof_sensor_data[tof_idx].SIGNAL_RATE;
}

float get_ambient_rate(int tof_idx) {
  return tof_sensor_data[tof_idx].AMBIENT_RATE;
}

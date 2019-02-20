//
// Created by FionaLee on 2/18/19.
//

#ifndef STM_COM_TOF_API_H
#define STM_COM_TOF_API_H

int stmcom_init(void);

int get_range_status(int tof_idx);

int get_range_mm(int tof_idx);

float get_signal_rate(int tof_idx);

float get_ambient_rate(int tof_idx);

void *read_and_unpack_thread(void *argu);
#endif //STM_COM_TOF_API_H

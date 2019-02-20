import ctypes
import threading

_STM_com = ctypes.CDLL('STM_com.so')


def test():
    global _STM_com
    init_result = _STM_com.stmcom_init()

    t = threading.Thread(target=_STM_com.read_and_unpack_thread)
    t.start()
    while(1):
       for i in range(6):
           print(_STM_com.get_tof_idx(i))
           print(_STM_com.get_range_mm(i))
           # print(_STM_com.get_signal_rate(i))
           print("\n")
    return init_result

print(test())

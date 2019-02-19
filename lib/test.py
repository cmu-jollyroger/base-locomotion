import ctypes

_STM_com = ctypes.CDLL('STM_com.so')

def test():
    global _STM_com
    init_result = _STM_com.stmcom_init()
    print(_STM_com.get_range_mm(1))
    print(_STM_com.get_signal_rate(1))
    print(_STM_com.get_ambient_rate(1))
    print(_STM_com.get_range_status(1))
    return init_result

print(test())
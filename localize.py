import matplotlib.pyplot as plt
import numpy as np 
import math as mth
import ctypes

_STM_com = ctypes.CDLL('lib/STM_com.so')
num_sen = 6
# def test():
#     global _STM_com
#     init_result = _STM_com.stmcom_init()
#     print(_STM_com.get_range_mm(1))






#     print(_STM_com.get_signal_rate(1))
#     print(_STM_com.get_ambient_rate(1))
#     print(_STM_com.get_range_status(1))
#     return init_result

 
# print(test())
# global variable
bed_l = 1524
bed_w = 914.4
chassis_l = 2*152.4
chassis_w = 2*152.4
chassis_diag = mth.sqrt(chassis_l**2 + chassis_w**2)
center_offset = np.array([chassis_l, chassis_diag, chassis_w, chassis_diag, chassis_w, chassis_diag ])
center_offset = np.true_divide(center_offset,2)
tol = 1e-6



x_range = range(-400,400,1)

# test data
t_0 = np.deg2rad(90)
t_1 = np.deg2rad(135)
t_2 = np.deg2rad(180)
t_3 = np.deg2rad(225)
t_4 = np.deg2rad(0)
t_5 = np.deg2rad(45)
t = np.array([t_0, t_1, t_2, t_3, t_4, t_5])
# print(t_1,t_2, t_3)
# d_0 = 50             #mm 
# d_1 = 50*mth.sqrt(2) #mm 
# d_2 = 50             #mm
# d_3 = 50*mth.sqrt(2) #mm 
# d_4 = 4000           #mm
# d_5 = 50*mth.sqrt(2) #mm
# d = np.array([d_0, d_1, d_2, d_3, d_4, d_5])
#print(t,d)



def inLines(line, lines_): 
    for l in lines_: 
        if len(l) == 1 and len(line)==1: 
            if(abs(line[0]-l[0]) <=tol): return True
        elif len(line) == 1 : continue 
        elif (abs(line[0]-l[0]) <=tol and abs(line[1]-l[1]) <=tol): return True 
    return False

def init(t): 
	global _STM_com
	_STM_com.stmcom_init()
	localize(t)


def localize(t): 
	global _STM_com
	# in frame of robot (x,y) = (0,0)
	x_y_anchor =[]  
	for sensor in range(num_sen):
		dist_off = _STM_com.get_range_mm(sensor) + center_offset[sensor]
		x,y = dist_off*mth.cos(t[sensor]) , dist_off*mth.sin(t[sensor])
		if(x> bed_w or y>bed_l): x,y = (0,0)
		x_y_anchor.append([x,y])

	#print(x_y_anchor)

	# plot the data captured in robot frame 
	plt.figure(10)
	plt.plot([0],[0], 'ro')
	for i in x_y_anchor:
		if not( i[0] == 0 and i[1] == 0): 
  			plt.plot(i[0], i[1], 'bo')
	plt.show()

	# find out the lines of between points taking two at a time 
	lines_ = []
	for sen_i in range(len(x_y_anchor)-2): 
	    x_0, y_0 = x_y_anchor[sen_i]
	    if not(x_0 == 0 and y_0 == 0): 
	        for sen_k in range(sen_i+1,len(x_y_anchor)):
	            if not(sen_i == 2 and sen_k == 4) and not(sen_i ==3 and sen_k ==5) : 
	                #print(sen_i,sen_k)
	                x_1, y_1 = x_y_anchor[sen_k]
	                if not(x_1 == 0 and y_1 == 0): 
	                    if (abs(x_1- x_0)<=tol ) : 
	                        line = [x_1]
	                    else: 
	                        slope = (y_1-y_0)/(x_1-x_0)
	                        intercept = y_1 - slope*x_1
	                        line = [slope, intercept]
	                    if line not in lines_ and not inLines(line,lines_):
	                        lines_.append(line)
	#print(lines_, len(lines_))

	# plot the lines
	plt.figure(20)
	for l in lines_: 
	    if len(l) == 1: plt.axvline(l[0])
	    else: 
	        y_range = l[0]*x_range + l[1]
	        plt.plot(x_range,y_range)
	plt.show()

	# find the perpendicular lines
	perp_lines = []

	for lin in range(len(lines_)-1): 
	    for o_lin in range(lin+1, len(lines_)): 
	        l_on,l_comp = lines_[lin], lines_[o_lin]
	        mslope = l_on[0]*l_comp[0]
	        if  ((len(l_on)==1 and l_comp[0] == 0) 
	            or (len(l_comp)==1 and l_on[0] == 0) 
	            or abs(mslope+1)<=tol):
	            perp_lines.append([l_on,l_comp])

	#print(perp_lines)

	corners = []

	for csec in perp_lines: 
	    x_corner = 0
	    y_corner = 0
	    if(len(csec[0]) == 1): 
	        x_corner = csec[0][0]
	        y_corner = csec[1][1]
	        
	    elif (len(csec[1]) == 1): 
	        x_corner = csec[1][0]
	        y_corner = csec[0][1]
	        
	    else: 
	        slope_0,intercept_0 = csec[0]
	        slope_1,intercept_1 = csec[1]
	        x_corner = (intercept_1 - intercept_0)/(slope_0-slope_1)
	        y_corner = slope_1*x_corner + intercept_1

	    corners.append([x_corner, y_corner])
	    
	#print(corners)
	target_corner = [0,0]

	translation = [] 
	for corner in np.array(corners): 
	    print(corner)
	    translation.append([-1*corner[0],corner[1]])


init(t)











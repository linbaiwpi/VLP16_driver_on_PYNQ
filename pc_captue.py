import ctypes
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

so = ctypes.CDLL('./libvelodynedriver.so')
so.velodyne_display("hello")
p=ctypes.create_string_buffer(29184*4*4)
so.velodyne_run(p)


p_np = np.frombuffer(p,dtype=np.float32)
#print(p_np)
p_np = p_np.reshape((29184,4))
#np.savetxt('pc_test.txt', p_np, fmt='%f')

fig = plt.figure()
ax = plt.axes(projection='3d')
ax.scatter(p_np[:,0],p_np[:,1],p_np[:,2],s=1)
plt.show()

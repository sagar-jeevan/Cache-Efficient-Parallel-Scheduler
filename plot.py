import matplotlib.pyplot as plt
import numpy as np

Size=[7, 8, 9, 10, 11]

M_4   = [0.14193, 2.03238, 15.1056, 22.2891, 178.2173]
M_8   = [0.04617, 1.58794, 12.3481, 17.0967, 136.1678]
M_16  = [0.035617, 1.50127, 11.9192, 15.7817, 96.1831]
M_32  = [0.03205, 1.46738, 12.0379, 15.3304, 94.03223]
M_64  = [0.03173, 1.48553, 11.9391, 15.0136, 94.8186]
M_128 = [0.02886, 1.56052, 11.7056, 15.4494, 93.6775]

plt.figure(figsize = (20,10))
plt.title('Run Time for Different Base Cases (M)', fontsize = 14)
plt.ylabel('Run Time (Sec)', fontsize = 14)
plt.xlabel('Input Size (raised to 2)', fontsize = 14)

plt.plot(Size, M_4, color = 'orange', label = 'M = 4')
plt.plot(Size, M_8, color = 'blue', label = 'M = 8')
plt.plot(Size, M_16, color = 'red', label = 'M = 16')
plt.plot(Size, M_32, color = 'green', label = 'M = 32')
plt.plot(Size, M_64, color = 'yellow', label = 'M = 64')
plt.plot(Size, M_128, color = 'brown', label = 'M = 128')

plt.legend(loc = 'upper left', fontsize = 20)
plt.show()

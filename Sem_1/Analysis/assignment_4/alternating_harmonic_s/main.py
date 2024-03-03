import math

import numpy as np
import matplotlib.pyplot as plt
# MARA IELCIU


t = np.arange(1, 50, 1)
series = (-1) ** (t + 1) / t
ampli = np.cumsum(series)

plt.plot(t, ampli)
plt.grid()
plt.show()

sum_even = 0
sum_odd = 0
for i in range(1,50):
    if i%2 == 0:
        sum_even = sum_even + 1/i
    else:
        sum_odd = sum_odd + 1/i
total_sum = sum_odd - sum_even
print(total_sum-math.log(2))













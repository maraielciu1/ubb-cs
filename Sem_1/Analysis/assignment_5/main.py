"""
Let f:R→R be differentiable. To minimize f, consider the gradient descent method x_n+1=x_n−eta f′(x_n),
where x_1∈R and eta >0 (learning rate).  Use Python (numerics or graphics) for the following:
(a) Take a convex f and show that for small eta the method converges to the minimum of f.
(b) Show that by increasing eta the method can converge faster (in fewer steps).
(c) Show that taking eta too large might lead to the divergence of the method.
(d) Take a nonconvex f and show that the method can get stuck in a local minimum.


Let f:R->R , f(x) = x^2
"""
import math

import numpy as np
import matplotlib.pyplot as plt

def f1_prime(x):
    return 2*x  # 2x

def f2_nonconvex_prime(x):
    return 3*x**2   # 3x^2
def find_new_element(x, learning_rate):
    return x - learning_rate * f1_prime(x)

series = []
series.append(3)
max_number_of_iterations = 1000
precision = 0.00000001
x_1 = 3
learning_rate = 0.2
previous_precision = 1
iteration_counter = 0
while previous_precision > precision and iteration_counter < max_number_of_iterations:
    iteration_counter += 1
    previous_precision = abs(x_1 - find_new_element(x_1, learning_rate))
    x_1 = find_new_element(x_1, learning_rate)
    series.append(x_1)
    print(x_1)

print("The local minimum occurs at", x_1)
plt.plot(series)
plt.grid()
plt.show()

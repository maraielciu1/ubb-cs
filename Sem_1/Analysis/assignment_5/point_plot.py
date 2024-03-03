"""
(c) Show that taking eta too large might lead to the divergence of the method.

"""
import matplotlib.pyplot as plt
def f1_prime(x):
    return 2*x  # 2x

def f2_nonconvex_prime(x):
    return 3*x**2   # 3x^2
def find_new_element(x, learning_rate):
    return x - learning_rate * f1_prime(x)

x=[]
y=[]

max_number_of_iterations = 1000
precision = 0.00000001
x_1 = 3
x.append(x_1)
learning_rate = 1
previous_precision = 1
iteration_counter = 0
y.append(iteration_counter)
while previous_precision > precision and iteration_counter < max_number_of_iterations:
    iteration_counter += 1
    previous_precision = abs(x_1 - find_new_element(x_1, learning_rate))
    x_1 = find_new_element(x_1, learning_rate)
    x.append(x_1)
    y.append(iteration_counter)
plt.plot(y,x, color='none', linestyle='dashed', linewidth = 3,marker='o', markerfacecolor='blue')
plt.grid()
plt.show()
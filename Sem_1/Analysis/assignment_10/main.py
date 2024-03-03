import numpy as np
import matplotlib.pyplot as plt


# Define the objective function (in this case, a simple quadratic function)
def objective_function(x, y):
    return x ** 2 + b * y ** 2


# Define the gradient of the objective function
def gradient(x, y):
    grad_x = 2 * x
    grad_y = 2 * b * y
    return np.array([grad_x, grad_y])


# Gradient Descent function
def gradient_descent(initial_point, learning_rate, iterations):
    history = [initial_point]
    current_point = np.array(initial_point)

    for i in range(iterations):
        current_point = current_point - learning_rate * gradient(*current_point)
        history.append(current_point)

    return np.array(history), i + 1  # Return the history and the number of iterations


# Values of b
b_values = [1, 1 / 2, 1 / 5, 1 / 10]

# Plotting
fig, axes = plt.subplots(nrows=2, ncols=2, figsize=(10, 8))
fig.suptitle('Gradient Descent for Different b Values')

for i, b in enumerate(b_values):
    # Contour plot
    x_vals = np.linspace(-3, 3, 100)
    y_vals = np.linspace(-3, 3, 100)
    X, Y = np.meshgrid(x_vals, y_vals)
    Z = objective_function(X, Y)

    contours = axes[i // 2, i % 2].contour(X, Y, Z, levels=20, cmap='viridis')
    axes[i // 2, i % 2].clabel(contours, inline=True, fontsize=8)

    # Gradient descent iterations
    initial_point = [-2, 2]
    iterations = 30
    learning_rate = 0.1
    history, num_steps = gradient_descent(initial_point, learning_rate, iterations)

    axes[i // 2, i % 2].plot(history[:, 0], history[:, 1], marker='o', label=f'b = {b}')
    axes[i // 2, i % 2].set_title(f'b = {b}')
    axes[i // 2, i % 2].legend()

    print(f"For b = {b}, the number of steps is: {num_steps}")

plt.show()

import numpy as np


def f(x):
    return x[0] ** 2 + 3 * x[1] ** 2


def gradient_f(x):
    return np.array([2 * x[0], 6 * x[1]])


def hessian_f(x):
    return np.array([[2, 0], [0, 6]])


def newton_method(initial_value, tolerance=1e-6, max_iterations=100):
    x = np.array(initial_value)

    for iteration in range(max_iterations):
        grad = gradient_f(x)
        hessian_inv = np.linalg.inv(hessian_f(x))
        update = -hessian_inv @ grad
        x = x + update

        if np.linalg.norm(update) < tolerance:
            return x, iteration + 1  # Converged

    return x, max_iterations  # Return the current estimate if max_iterations is reached


# Set an initial value
initial_value = [1.0, 1.0]

# Run Newton's method
result, iterations = newton_method(initial_value)

print(f"Minimum at: {result}")
print(f"Number of iterations: {iterations}")

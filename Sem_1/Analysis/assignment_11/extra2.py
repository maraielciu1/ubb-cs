import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import minimize

# Define the objective function
def objective_function(x):
    return 100 * (x[1] - x[0]**2)**2 + (1 - x[0])**2

# Set initial point
initial_point = [-1, 1]

# Run optimization using L-BFGS-B
result = minimize(objective_function, initial_point, method='L-BFGS-B', tol=1e-6)

# Extract the optimization result
minimized_point = result.x

# Plotting
x_vals = np.linspace(-2, 2, 100)
y_vals = np.linspace(-1, 3, 100)
X, Y = np.meshgrid(x_vals, y_vals)
Z = 100 * (Y - X**2)**2 + (1 - X)**2

plt.contour(X, Y, Z, levels=20, cmap='viridis', alpha=0.6)
plt.scatter(minimized_point[0], minimized_point[1], color='red', marker='*', label='Minimum (1,1)')
plt.title('Optimization using L-BFGS-B')
plt.legend()
plt.xlabel('x')
plt.ylabel('y')
plt.xlim([-2, 2])
plt.ylim([-1, 3])
plt.show()

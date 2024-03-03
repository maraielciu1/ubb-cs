# IELCIU  MARA

import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm
from scipy.integrate import quad

# Gaussian curve function
gaussian_curve = lambda x: np.exp(-x**2)

# Numerical integration using quad
numerical_integral, _ = quad(gaussian_curve, -np.inf, np.inf)

# Exact value of sqrt(pi)
exact_value = np.sqrt(np.pi)

# Set x values for plotting
x_values = np.linspace(-2.5, 2.5, 1000)

# Generate 1000 values for a between 1 and 5
a_values = np.linspace(1, 5, 1000)

# Plot the Gaussian curve
plt.figure(figsize=(10, 6))
plt.plot(x_values, gaussian_curve(x_values), label='Gaussian Curve', color='blue')

# Fill the area below the curve using cumulative distribution function (CDF)
plt.fill_between(x_values, 0, gaussian_curve(x_values), where=(x_values >= -5) & (x_values <= 5), color='skyblue', alpha=0.4, label='Area below Curve')

# Highlight below the curve
for i, a in enumerate(a_values):
    color = plt.colormaps.get_cmap('tab20')(i / len(a_values))
    plt.fill_between(x_values, 0, gaussian_curve(x_values), where=(x_values >= -a) & (x_values <= a), color=color, alpha=0.5)

last_computed_value, _ = quad(gaussian_curve, -a_values[len(a_values)-1], a_values[len(a_values)-1])

# Enhancements
plt.title('Area below the Gaussian Curve $e^{-x^2}$ ')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid(True)
plt.show()

# Print numerical result and exact value for comparison
print(f'Last Computed Value: {last_computed_value}')
print(f'Numerical Integral: {numerical_integral}')
print(f'Exact Value (sqrt(pi)): {exact_value}')

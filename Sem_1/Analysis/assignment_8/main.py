import numpy as np
import matplotlib.pyplot as plt
# IELCIU MARA


def sample_unit_ball_p_norm(p, num_points=1000):
    # Generate random points in the unit ball for the p-norm
    r = np.random.uniform(0, 1, num_points) ** (1 / p)
    theta = np.random.uniform(0, 2 * np.pi, num_points)

    # Convert to Cartesian coordinates
    x = r * np.cos(theta)
    y = r * np.sin(theta)

    return x, y


# Values of p
p_values = 1.25

# Plotting
plt.figure(figsize=(12, 8))


x, y = sample_unit_ball_p_norm(p)
plt.scatter(x, y, label=f'p = {p}')

# Set equal aspect ratio for better visualization
plt.gca().set_aspect('equal', adjustable='box')

# Add labels and legend
plt.title('Random Sampling in Unit Ball for Various p-norms')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid(True)
plt.show()

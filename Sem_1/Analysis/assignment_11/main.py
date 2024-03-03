"""
Let A be a 2×2 matrix and let the quadratic function f:R2→R,
  f(x) =1/2x^TAx.
  (a)  Give a matrix A such that f has a unique minimum.
  (b)  Give a matrix A such that f has a unique maximum.
  (c)  Give a matrix A such that f has a unique saddle point.
In each case plot the 3d surface, three contour lines and the gradient at three different points.
"""
import numpy as np
import matplotlib.pyplot as plt


# Define the quadratic function and its gradient
def quadratic_function(x, A):
    return 0.5 * x.T @ A @ x


def gradient(x, A):
    return A @ x


# Create a grid of points
x_vals = np.linspace(-5, 5, 100)
y_vals = np.linspace(-5, 5, 100)
x, y = np.meshgrid(x_vals, y_vals)
points = np.vstack([x.ravel(), y.ravel()])

# Plotting for each case
for A_matrix, plot_title in [([[2, 0], [0, 2]], "Unique Minimum"),
                             ([[-2, 0], [0, -2]], "Unique Maximum"),
                             ([[2, 0], [0, -2]], "Unique Saddle Point")]:
    A = np.array(A_matrix)

    # Calculate the function values
    z = np.array([quadratic_function(point, A) for point in points.T])
    z = z.reshape(x.shape)

    # Plot 3D surface
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.plot_surface(x, y, z, cmap='viridis', alpha=0.8)
    ax.set_title(plot_title)

    # Plot contour lines
    contour_levels = np.linspace(z.min(), z.max(), 3)
    ax.contour(x, y, z, contour_levels, colors='k', linestyles='solid')

    # Plot gradient at three different points
    for point in [[-4, -4], [0, 0], [4, 4]]:
        grad = gradient(np.array(point), A)
        ax.quiver(point[0], point[1], quadratic_function(np.array(point), A), grad[0], grad[1],
                  quadratic_function(np.array(point), A), color='r', length=1, normalize=True)

        # Highlight the point in red
        ax.scatter(point[0], point[1], quadratic_function(np.array(point), A), color='r', s=20)

plt.show()



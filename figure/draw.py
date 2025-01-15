import pandas as pd
import matplotlib.pyplot as plt

# Read data from the file
file_path = 'data.csv'  # Replace with your file's path
data = pd.read_csv(file_path)

# Extract x and y values
a = data['Instance']
x = data['x']
y = data['y']
z = data['z']
#t = data['t']
#u = data['u']
#v = data['v']
#
# Create the scatter plot
#plt.figure(figsize=(8, 6))
#plt.scatter(a, x, color='blue', label='1500 10')
#plt.scatter(a, y, color='red', label='1500 20')
#plt.scatter(a, z, color='green', label='1500 30')
#plt.scatter(a, t, color='cyan', label='2000 10')
#plt.scatter(a, u, color='yellow', label='2000 20')
#plt.scatter(a, v, color='purple', label='2000 30')
#
#plt.title('Scatter Plot from File Data')
#plt.xlabel('X-axis')
#plt.ylabel('Y-axis')
#plt.legend()
#plt.grid(True)

# Show the plot
fig, axes = plt.subplots(1, 2, figsize=(12, 6))

# First scatter plot
axes[0].scatter(a, z, color='blue', label='Hybrid')
axes[0].scatter(a, y, color='red', label='NSGA II')
#axes[0].set_title('Scatter Plot 1')
axes[0].set_xlabel('Instances')
axes[0].set_ylabel('Hypervolume')
axes[0].legend()
axes[0].grid(True, linestyle='--', alpha=0.6)

# Second scatter plot
axes[1].scatter(a, z, color='blue', label='Hybrid')
axes[1].scatter(a, x, color='green', label='Tabu')
#axes[1].set_title('Scatter Plot 2')
axes[1].set_xlabel('Instances')
axes[1].set_ylabel('Hypervolume')
axes[1].legend()
axes[1].grid(True, linestyle='--', alpha=0.6)
plt.show()

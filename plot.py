import pandas as pd
import matplotlib.pyplot as plt

# Load CSV files
def load_csv(filepath):
    df = pd.read_csv(filepath)
    df.columns = df.columns.str.strip()  # Remove any extra spaces in column names
    return df

df_cache_misses = load_csv('cache_misses_vs_threads.csv')
df_cache_references = load_csv('cache_references_vs_threads.csv')
df_time = load_csv('time_vs_threads.csv')

# Print column names to verify correct names
print("Cache Misses Columns:", df_cache_misses.columns)
print("Cache References Columns:", df_cache_references.columns)
print("Time Columns:", df_time.columns)

# Drop unnecessary 'Type' column if present
df_cache_misses = df_cache_misses.drop(columns=['Type'], errors='ignore')
df_cache_references = df_cache_references.drop(columns=['Type'], errors='ignore')
df_time = df_time.drop(columns=['Type'], errors='ignore')

# Use correct column names
df_cache_misses = df_cache_misses[['Threads', 'Cache_Misses']]
df_cache_references = df_cache_references[['Threads', 'Cache_References']]
df_time = df_time[['Threads', 'Time']]

# Plot 1: Cache Misses vs Threads
plt.figure(figsize=(8, 5))
plt.plot(df_cache_misses['Threads'], df_cache_misses['Cache_Misses'], marker='o', linestyle='-', label='Cache Misses')
plt.xlabel('Threads')
plt.ylabel('Cache Misses')
plt.title('Cache Misses vs Threads')
plt.legend()
plt.grid()
plt.savefig("cache_misses_vs_threads.png")

# Plot 2: Cache References vs Threads
plt.figure(figsize=(8, 5))
plt.plot(df_cache_references['Threads'], df_cache_references['Cache_References'], marker='s', linestyle='-', label='Cache References', color='r')
plt.xlabel('Threads')
plt.ylabel('Cache References')
plt.title('Cache References vs Threads')
plt.legend()
plt.grid()
plt.savefig("cache_references_vs_threads.png")

# Plot 3: Time vs Threads
plt.figure(figsize=(8, 5))
plt.plot(df_time['Threads'], df_time['Time'], marker='^', linestyle='-', label='Execution Time', color='g')
plt.xlabel('Threads')
plt.ylabel('Time (seconds)')
plt.title('Execution Time vs Threads')
plt.legend()
plt.grid()
plt.savefig("time_vs_threads.png")

import numpy as np
import pandas as pd
import yaml
from datetime import datetime, timedelta

with open('config.yaml', 'r') as file:
    config = yaml.safe_load(file)

users = config['num_users']
products = config['num_products']
size1 = config['day1_size']
size2 = config['day2_size']

day1 = datetime(2023, 4, 1)
day2 = day1 + timedelta(days=1)

users_day1 = np.random.randint(1, users + 1, size=size1)
products_day1 = np.random.randint(1, products + 1, size=size1)
timestamps_day1 = [day1 + timedelta(seconds=np.random.randint(0, 86400)) for _ in range(size1)]

users_day2 = np.random.randint(1, users + 1, size=size2)
products_day2 = np.random.randint(1, products + 1, size=size2)
timestamps_day2 = [day2 + timedelta(seconds=np.random.randint(0, 86400)) for _ in range(size2)]

day1 = {'user_id': users_day1, 'product_id': products_day1, 'timestamp': timestamps_day1}
day2 = {'user_id': users_day2, 'product_id': products_day2, 'timestamp': timestamps_day2}
df1 = pd.DataFrame(day1)
df2 = pd.DataFrame(day2)

df1.to_csv('day1.csv', index=False)
df2.to_csv('day2.csv', index=False)

print("Sample of day1:")
print(df1.head(10))
print("\nSample of day2:")
print(df2.head(10))


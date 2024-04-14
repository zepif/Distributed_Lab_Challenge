import pandas as pd
from collections import defaultdict

day1 = pd.read_csv('day1.csv')
day2 = pd.read_csv('day2.csv')

common_users = set(day1['user_id']).intersection(set(day2['user_id']))

print("Common users:")
print(list(common_users))

user_products = defaultdict(lambda: [set(), set()])

for _, row in day1.iterrows():
    user_products[row['user_id']][0].add(row['product_id'])

for _, row in day2.iterrows():
    user_products[row['user_id']][1].add(row['product_id'])

result = [user for user, (day1_products, day2_products) in user_products.items()
          if day2_products - day1_products]

print("Users who visited new products on the second day:")
print(result)

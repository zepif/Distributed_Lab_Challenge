import random

def generate_data(num_rows):
    data = ["tx_id,tx_size,tx_fee"]

    for _ in range(num_rows):
        tx_id = f"{random.randint(1, 99999):05}"
        tx_size = random.randint(1, 1000)
        tx_fee = random.randint(1, 10000)
        data.append(f"{tx_id},{tx_size},{tx_fee}")

    return data

rows = 10000
data = generate_data(rows)

with open("data.csv", "w") as f:
    for line in data:
        f.write(line + "\n")

print(f"All data is saved in data.csv")

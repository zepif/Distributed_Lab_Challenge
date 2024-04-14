import asyncio
import csv
from typing import List

async def run_path_finder(max_hops: int) -> List[str] or None:
    import main
    start_url = 'https://en.wikipedia.org/wiki/Special:Random'
    result, _, path = await main.parse_pages(start_url, max_hops=max_hops, False)
    #print(f"Result is {result}")
    if result:
        print(f"Path length: {len(path)}")
        return path
    else:
        print(f"susy link : {start_url}")
        return None

async def find_long_path() -> List[str]:
    max_hops = 6
    path = await run_path_finder(max_hops)
    if path and len(path) <= max_hops:
        return path
    else:
        return None

async def main():
    existing_paths = set()
    try:
        with open("paths.csv", "r") as csvfile:
            reader = csv.reader(csvfile)
            for row in reader:
                path_tuple = tuple(row)
                existing_paths.add(path_tuple)
    except FileNotFoundError:
        pass

    while True:
        path = await find_long_path()
        if path:
            print(f"We find a path of length : {len(path) - 1}:")
            print(" -> ".join(path))
            path_tuple = tuple(path[1:])
            if len(path) >= 3 and path_tuple not in existing_paths:
                with open("paths.csv", "a", newline="") as csvfile:
                    writer = csv.writer(csvfile)
                    writer.writerow(path[1:])
        else:
            print("Failed to find a way to Adolf Hitler")
            exit()

if __name__ == "__main__":
    asyncio.run(main())

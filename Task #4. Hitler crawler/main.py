import asyncio
import aiohttp
import re
from bs4 import BeautifulSoup
from urllib.parse import urljoin, unquote_plus
from asyncio import Queue
import threading
import time
import os
import gzip
import csv

MAX_THREADS = os.cpu_count() * 2
db = set()
path_db = {}
visited = set()
path_found = False
path_lock = threading.Lock()
prev_request_time = time.time()
#start_url = 'https://en.wikipedia.org/wiki/Special:RandomPage'
#start_url = 'https://en.wikipedia.org/wiki/Road_Atlanta'
start_url = input("Enter a Wikipedia page URL: ")
requests_per_second = 0

async def fetch_page(session, url):
    global prev_request_time, requests_per_second
    current_time = time.time()
    if current_time - prev_request_time >= 1:
        #print(requests_per_second)
        requests_per_second = 0
        prev_request_time = current_time
    requests_per_second += 1
    try:
        headers = {'Accept-Encoding': 'gzip'}
        async with session.get(url, headers=headers) as response:
            if response.headers.get('Content-Encoding') == 'gzip':
                data = await response.read()
                if data[:2] == b'\x1f\x8b':
                    return gzip.decompress(data).decode('utf-8')
            return await response.text()
    except (aiohttp.ClientError, asyncio.TimeoutError):
        return None

def get_article_URLs(html, path_length, check_db=True):
    global check_bd
    articleList = []
    soup = BeautifulSoup(html, 'lxml')
    content_div = soup.find('div', {'id': 'bodyContent'})
    if content_div:
        links = content_div.find_all('a', href=re.compile(r'^/wiki/(?!Wiki|Wikipedia|Special|Help|File|Talk|Category|Portal|Draft|Module|Template:|Template_talk:)'))
        for link in links:
            href = link.get('href')
            if href:
                href_parts = href.split('#', 1)
                decoded_href = unquote_plus(href_parts[0])
                wiki_part = decoded_href.split('/wiki/', 1)
                if len(wiki_part) > 1:
                    filtered_href = re.sub(r'\\\\(.*?\\\\)', '', wiki_part[1])
                    if filtered_href == 'Adolf_Hitler':
                        articleList.clear()
                        articleList.append((f'https://en.wikipedia.org{href}', path_length))
                        visited.clear()
                        break
                    elif filtered_href in db and check_db:
                        articleList.clear()
                        articleList.append(('#', -1))
                        articleList.append((f'https://en.wikipedia.org{href}', path_length - 1))
                        for i, url in enumerate(path_db[filtered_href]):
                            articleList.append((f'https://en.wikipedia.org/wiki/{url}', path_length + i))
                        #print(articleList)
                        visited.clear()
                        break
                    elif filtered_href not in visited:
                        visited.add(filtered_href)
                        articleList.append((f'https://en.wikipedia.org{href}', path_length))

    #print(f'Number of gained links : {len(articleList)}')
    with open('log.txt', 'a', encoding='utf-8') as log_file:
        for link, path_length in articleList:
            log_file.write(f"{link}\n")

    return articleList

async def parse_pages(start_url, max_hops=6, session=None):
    global path_found
    path_found = False
    check_bd = False
    start_time = time.time()
    queue = asyncio.Queue()
    await queue.put((start_url, 1, []))
    path = [(start_url, 1)]
    visited.add(start_url)
    async with aiohttp.ClientSession() as session:
        while not queue.empty():
            current_url, path_length, path = await queue.get()
            path.append((current_url))
            html = await fetch_page(session, current_url)
            if html:
                urls = get_article_URLs(html, path_length + 1)
                if len(urls) > 0 and urls[0] == ('#', -1):
                        path_found = True
                        end_time = time.time()
                        for i, url in enumerate(urls[1:]):
                            path.append(url)
                        return path, end_time - start_time, path
                for link, link_path_length in urls:
                    if link.endswith('Adolf_Hitler'):
                        with path_lock:
                            if not path_found:
                                path_found = True
                                end_time = time.time()
                                path.append(link)
                                return path, end_time - start_time, path
                    elif link_path_length <= max_hops:
                        await queue.put((link, link_path_length, path.copy()))
    end_time = time.time()
    return None, end_time - start_time, []

async def worker():
    result, elapsed_time, path = await parse_pages(start_url, 6)
    if result:
        print("Path to Hitler found:")
        print(" -> ".join(f"{link} (Path length: {i})\n" for i, link in enumerate(path)))
        print(f"Time taken: {elapsed_time:.2f} seconds")
    else:
        print(f"Hitler not found from {start_url}")
        print(f"Time taken: {elapsed_time:.2f} seconds")    

async def main():
    with open('paths.csv', 'r') as csvfile:
        reader = csv.reader(csvfile)
        for row in reader:
            for i, url in enumerate(row):
                href_parts = url.split('#', 1)
                decoded_href = unquote_plus(href_parts[0])
                filtered_href = decoded_href.split('/wiki/', 1)
                row[i] = filtered_href[1]
            #print(row)
            db.add(row[0])
            path_db[row[0]] = row[1:]
    #exit()

    start_time = time.time()
    print(f'starting from the page : {start_url}')

    with open('log.txt', 'w') as log_file:
        log_file.truncate(0)

    await worker()
    #tasks = [asyncio.create_task(worker()) for _ in range(MAX_THREADS)]
    #await asyncio.gather(*tasks)

    print(f"Total time taken: {time.time() - start_time:.2f} seconds")

if __name__ == '__main__':
    asyncio.run(main())

with open('log.txt', 'r') as file:
    log_links = []
    for link in file.readlines():
        link_parts = link.strip().split('/')
        if len(link_parts) > 1:
            log_links.append(link_parts[-1].split('#')[0])

with open('urls.txt', 'r') as file:
    urls_links = []
    for link in file.readlines():
        link_parts = link.strip().split('/')
        if len(link_parts) > 1:
            urls_links.append(link_parts[-1].split('#')[0])

log_only_links = [link for link in log_links if link not in urls_links]
urls_only_links = [link for link in urls_links if link not in log_links]

with open('unique_links.txt', 'w') as file:
    file.write("Links unique only in log.txt:\n")
    for link in log_only_links:
        file.write(link + '\n')
    file.write("\nLinks unique only in urls.txt:\n")
    for link in urls_only_links:
        file.write(link + '\n')

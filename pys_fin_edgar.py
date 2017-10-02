# Generate the list of index files archived in EDGAR since start_year (earliest: 1993) until the most recent quarter
import datetime
import requests
current_year = datetime.date.today().year
current_quarter = (datetime.date.today().month - 1) // 3 + 1
start_year = 1993
years = list(range(start_year, current_year))
quarters = ['QTR1', 'QTR2', 'QTR3', 'QTR4']
history = [(y, q) for y in years for q in quarters]
for i in range(1, current_quarter + 1):
    history.append((current_year, 'QTR%d' % i))
urls = ['https://www.sec.gov/Archives/edgar/full-index/%d/%s/master.idx' % (x[0], x[1]) for x in history]
urls.sort()
print(urls)
lines = requests.get(urls[0]).text.splitlines()
print(lines)
records = [tuple(line.split('|')) for line in lines[11:]]
print(records[2])
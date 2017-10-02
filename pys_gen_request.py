import csv, requests
from urllib import request
req = requests.post('https://en.wikipedia.org/w/index.php', data = {'search':'Nanotechnology'})

req.raise_for_status()
with open('Nanotechnology.html', 'wb') as fd:
    for chunk in req.iter_content(chunk_size=50000):
        fd.write(chunk)

url='http://wildfire.alberta.ca/reports/activedd.csv'
#url='http://winterolympicmedals.com/medals.csv'
r=request.urlopen(url).read()
#decoded_content = r.read().decode('utf-8')
#text = decoded_content.iter_lines()
#reader=csv.reader(text, delimiter=',')
#for row in reader:
#    print(row)
#rd=str(r, 'utf-8')
reader=csv.reader(r, delimiter=',')
print(reader)
with open('testdn.csv', 'wb') as f:
    f.write(r)
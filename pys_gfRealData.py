import goferlib.goferWeb as gw
from bs4 import BeautifulSoup
from bs4 import re
from urllib.request import urlopen
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.common.exceptions import TimeoutException
import time
url='https://finance.google.com/finance?q=intc'
browser='chrome'
dInit=getattr(gw, 'init_'+browser+'_driver')
driver=dInit()
time.sleep(5)
driver.get(url)
html = driver.page_source
soup = BeautifulSoup(html, "html.parser")
# print(html)
#print(soup)
nameList = soup.findAll("span", {"class":"pr"})
for name in nameList:
    print(name.get_text())
nameList = soup.findAll("td", {"class":"key"})

for name in nameList:
    sibling = name.find_next_sibling()
    #print(name.get_text()[:-1]+' = '+sibling.get_text())
nameList = soup.findAll("td", {"class":"ctsymbol"})
# nameList = soup.select('td[class="ctsymbol"]')
for e1 in nameList:
   print(e1.text)



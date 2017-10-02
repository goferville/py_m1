from bs4 import BeautifulSoup
from bs4 import re
from urllib.request import urlopen
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.common.exceptions import TimeoutException
import time
url='https://finance.yahoo.com/quote/SPY/history?p=SPY'
html=urlopen(url)
print(html)
soup = BeautifulSoup(html, "html.parser")
print(soup)
nameList = soup.findAll("span", {"text":"Time Period"})
for name in nameList:
    print(name.get_text())

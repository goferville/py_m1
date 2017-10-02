from bs4 import BeautifulSoup
from bs4 import re
from urllib.request import urlopen
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.common.exceptions import TimeoutException
import time

def init_firefox_driver():

    driver = webdriver.Firefox(executable_path=r'C:\Users\qiangli3\PycharmProjects\Python_goferbase\geckodriver.exe')
    driver.wait = WebDriverWait(driver, 5)
    return driver


def get_soup_obj(url):
    # init a selenium driver & get url's html
    # start a Firefox driver
    # driver = init_firefox_driver()
    # driver.get(url)
    # get html from driver
    html = urlopen(url)
    # driver.quit()  # remove this line to leave the browser open
    # print(html)   #test only
    # pass driver to bs
    soup = BeautifulSoup(html, "html.parser")
    return soup


def get_soup_obj_ff(url):
    global ffDriver
    ffDriver.get(url)
    html = ffDriver.page_source
    ffDriver.wait = WebDriverWait(ffDriver, 5)
    soup = BeautifulSoup(html, "html.parser")
    return soup

def soup_test(soup):
    for link in soup.find_all('a'):
        print(link.get('href', None))


def basic_test():
    url = 'http://www.google.com'
    soup = get_soup_obj(url)
    # further parsing: do something with bs object
    soup_test(soup)


def text_parse_test():
    print("1. === basic scrapping ===")
    # Book "web scraping with Python", P. 14
    url = 'http://www.pythonscraping.com/pages/warandpeace.html'
    soup = get_soup_obj(url)
    #nameList = soup.find_all("span",{"class":"green"})
    #for name in nameList:
    #    print(name.get_text())
    # following should return both red and green tag, but only last was returned
    # cannot be use dlike this according to bs4 document
    combinedList = soup.find_all("span", {"class": "green", "class": "red"})
    for cName in combinedList:
        print(cName.get_text())
    # Qs???: problem with "the Prince"
    # also see how I convert integer to string
    pList = soup.find_all(text="the prince")
    print("the Prince ="+len(pList).__str__())


    # test .children
    print("2. === test .children ===")
    url = 'http://www.pythonscraping.com/pages/page3.html'
    soup = get_soup_obj(url)
    for child in soup.find("table", {"id": "giftList"}).children:
        print(child)


def test_reg_expression():
    url='http://www.pythonscraping.com/pages/page3.html'
    soup=get_soup_obj(url)
    # search <img src="../img/gifts/img3.jpg">
    images=soup.find_all("img", {"src": re.compile("\.\./img/gifts/img.\.jpg")})
    for image in images:
        print(image["src"])
    gifts=soup.find_all("tr",{"id": re.compile("gift.")})
    for gift in gifts:
        print(gift["id"])
    # test Lambda expressions
    # retrieves all tags that have exactly two attributes
    tags = soup.find_all(lambda tag: len(tag.attrs) == 2)
    for tag in tags:
        print(tag.attrs)


def crawl_wiki():
    url='http://en.wikipedia.org/wiki/Kevin_Bacon'
    # url = 'http://www.baidu.com'
    soup=get_soup_obj(url)
    links=soup.find_all("a")
    i=1
    j=1
    # find all href links
    for link in links:
        if 'href' in link.attrs:
            print('link #' + i.__str__() + '=' + link.attrs['href'])
            i = i + 1
            # find all href with wiki in it
            if 'wiki' in link.attrs['href']:
                print('wiki link #' + j.__str__() + '=' + link.attrs['href'])
                j=j+1


# text_parse_test()
# test_reg_expression()
def get_links(url):
    global pages, pages_mit
    soup=get_soup_obj('http://www.pythonscraping.com/'+url)
    # search href, start with '/' means internal link
    # links=soup.find_all("a",{'href': re.compile("^(/node/|/blog|/)")})
    links = soup.find_all("a", {'href': re.compile("^(/)")})
    for link in links:
        # print(link.attrs['href'])
        if 'href' in link.attrs:
            if link.attrs['href'] not in pages:
                newPage=link.attrs['href']
                pages.add(newPage)
                print(newPage)
                get_links(newPage)

def search_mitAuto(url):
    global pages_mit
    print('http://www.mitbbs.com'+url)
    soup=get_soup_obj('http://www.mitbbs.com/bbsdoc1/Automobile_1_0.html')
    url='http://www.mitbbs.com'
    soup = get_soup_obj(url)


def search_wxc_cooking(url):
        global pages_wxc
        global getPage
        soup=getPage('http://bbs.wenxuecity.com'+url)
        # print('ref stop='+'http://bbs.wenxuecity.com'+url)

        foodLinks=soup.find_all("a",href=re.compile("^(./1)"))
        for foodLink in foodLinks:
            if 'title' in foodLink.attrs:
                # print(foodLink.attrs['title'])
                # print(foodLink.text)
                if "泡菜" in foodLink.attrs['title']:
                    print(foodLink.attrs['title'])
                    print('Author='+foodLink.parent.find("a", {"class": "b"}).text)
                    #print('Author=' + foodLink.parent.find("a", id=re.compile("^(n_cooking_)")).__str__())
                    #for child in foodLink.parent.find(""):
                        #if 'text' in child.attrs:
                            #print(child.__str__())

        pageLinks = soup.find_all("a", href=re.compile("^(/cooking/\?page)"))
        for pageLink in pageLinks:
            # print(pageLink.attrs['href'])
            if pageLink.attrs['href'] not in pages_wxc:
                pages_wxc.add(pageLink.attrs['href'])
                print('New page found = '+pageLink.attrs['href'])
                search_wxc_cooking(pageLink.attrs['href'])

# region place holder for globals
pages_wxc=set()
# firefox driver
ffDriver=None
#ffDriver = init_firefox_driver()
getPage=get_soup_obj
#_bkm_: assign function name as a var
# urllib driver
# getPage=get_soup_obj

# endregion place holder for globals


# get_links("")
# search_mitAuto('/bbsdoc1/Automobile_1_0.html')
#url='http://www.wenxuecity.com/news/'
#html = urlopen(url)
#print(html)
url='/cooking/?page=1'
pages_wxc.add(url)
print(pages_wxc)
search_wxc_cooking(url)


# kill all script and style elements
# for script in soup(["script", "style"]):
    #script.extract()    # rip it out

# get text
#text = soup.get_text()
import urllib
from urllib.request import OpenerDirector
from urllib.request import getproxies
# url = 'https://www.google.com'

# === to detect system proxy
print(getproxies())

# html = OpenerDirector.open(fullurl='https://www.google.com')


# urllib will auto-detect your proxy settings and use those. This is through the ProxyHandler, which is part of the
# normal handler chain when a proxy setting is detected. Normally that’s a good thing, but there are occasions when it
# may not be helpful 5. One way to do this is to setup our own ProxyHandler, with no proxies defined. This is done
# using similar steps to setting up a Basic Authentication handler:
proxy_support = urllib.request.ProxyHandler({})
opener = urllib.request.build_opener(proxy_support)
urllib.request.install_opener(opener)

# y = urllib.request.urlopen('https://www.google.com')
y = opener.open('https://www.google.com')
# print(y.read())

from selenium import webdriver


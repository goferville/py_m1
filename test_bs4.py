from bs4 import BeautifulSoup
mytxt="""<td class="ctsymbol"><a href="/finance?q=' \
       'NASDAQ:TXN&amp;ei=tkTLWcDhHpaS2AaIn5bADQ" ' \
       'title="Texas Instruments..." id="rc-0">TXN</a></td>"""
s1 = BeautifulSoup(mytxt, 'lxml')
print(s1.find("td").attrs)
print(s1.find("td").text)
print(s1.find("td").attrs['class'])
print(s1.find("td").has_attr('class'))
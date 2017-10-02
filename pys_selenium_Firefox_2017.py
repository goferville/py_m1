from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.common.exceptions import TimeoutException
import time


def init_firefox_driver():
    driver = webdriver.Firefox(executable_path=r'C:\Users\qiangli3\PycharmProjects\jpy_2017\geckodriver.exe')
    driver.wait = WebDriverWait(driver, 5)
    return driver


def google_test():
    driver = init_Firefox_driver()
    driver.get('http://www.google.com')
    time.sleep(5)

    # =================================================================
    # BY.XPATH option is really useful
    # XPath is the language used for locating nodes in an XML document.
    # As HTML can be an implementation of XML (XHTML), Selenium users
    # can leverage this powerful language to target elements in their web applications.
    # XPath extends beyond (as well as supporting) the simple methods of locating
    # by id or name attributes, and opens up all sorts of new possibilities
    # such as locating the third checkbox on the page.
    #
    # see all examples below, from www.googl.com and test the usage
    # btn_show=driver.wait.until(EC.presence_of_element_located(
    #                           (By.XPATH, "//a[@class='gb_0c gb_md']")))
    # btn_show = driver.wait.until(EC.presence_of_element_located(
    #     (By.XPATH, "//a[@href='https://www.google.com/intl/en/options/']")))

    #This one goes to Gmail
    btn_show = driver.wait.until(EC.presence_of_element_located(
        (By.XPATH, "//a[@class='gb_P']")))
    btn_show.click()
    # =================================================================

    # =================================================================
    #following test is directly after www.google.com, without above click
    box = driver.wait.until(EC.presence_of_element_located(
        (By.NAME, "q")))

    button = driver.wait.until(EC.presence_of_element_located(
        (By.NAME, "btnK")))
    box.send_keys("selenium")
    button.click()
    btn_show = driver.wait.until(EC.presence_of_element_located(
                                  (By.CLASS_NAME, "gb_wb gb_Fa gb_7c gb_6c")))
    # =================================================================


def wait_test():

    # To test WebDriverWait function

    driver = init_Firefox_driver()
    driver.get('http://www.google.com')
    try:
        # (By.XPATH, "//a[@class='gb_P']"), must be inside ()
        btn_show = WebDriverWait(driver, 5).until(EC.presence_of_element_located((By.XPATH, "//a[@class='gb_P']")))
        btn_show.click()
    finally:
        print('completed!')

# google_test()
wait_test()

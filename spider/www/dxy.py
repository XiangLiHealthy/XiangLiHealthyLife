import downloader
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.wait import WebDriverWait
import bs4

from bs4 import BeautifulSoup
from time import sleep
import data_store
import downloader

path = '/home/xiangbaosong/work'

class SpiderDisesea:
    def __init__(self):
        options = webdriver.ChromeOptions()
        #add_argument('--headless')
        #self.browser = webdriver.Chrome(options=options)

        # 禁止图片和css加载
        prefs = {"profile.managed_default_content_settings.images": 2, 'permissions.default.stylesheet': 2}
        options.add_experimental_option("prefs", prefs)

        self.browser = webdriver.Chrome(options=options)
        self.browser.implicitly_wait(5)
    def login(self):
        log_url = 'https://auth.dxy.cn/accounts/login?logoDqId=21523&method=2&qr=false&service=https%3a%2f%2fdxy.com%2fauth%2flogin%2fdxy%3fredirect_uri%3daHR0cHM6Ly9keHkuY29tLw=='
        self.browser.get(log_url)
        log_title = self.browser.find_element_by_xpath("//h3[@class = 'login__head_log J-login-method']")
        log_title.click()

        username = self.browser.find_element_by_name('username')
        password = self.browser.find_element_by_name('password')
        submit = self.browser.find_element_by_xpath("//button[@type='submit']")

        username.send_keys('13883372441')
        password.send_keys('Xl2016xl')
        submit.click()
        sleep(10)

        return

    def reconnet_chrome(self):
        self.browser.close()
        self.browser = webdriver.Chrome()
        self.browser.implicitly_wait(5)

    def get_items(self):
        try:
            #get main page
            self.browser.get("https://dxy.com/vaccines")

            #get aid_card
            soup = BeautifulSoup(self.browser.page_source)
            aid_cards = soup.find_all('div', attrs = {'class' : 'vaccine-card-container'})

            surgrys = []
            for card in aid_cards :
                type = card.find('div', attrs = {'class' : 'vaccine-card-top-info-type'})
                cash = card.find('div', attrs = {'class' : 'vaccine-card-top-info-desc'})

                links = card.find_all('div', attrs = {'class' : 'link-line'})
                for link in links :
                    name = link.get_text()
                    href = link.contents[0].attrs['href']

                    item = {}
                    item['name'] = name
                    item['url'] = href
                    item['type'] = type.get_text()
                    item['money'] = cash.get_text()
                    surgrys.append(item)

            #get head1, head2, items
            return surgrys
        except Exception as e :
            print (e)

        return None

    def get_content(self, url):
        try:
            #opend web
            self.browser.get(url)
            soup = BeautifulSoup(self.browser.page_source)

            text = ''
            content = soup.find('div', attrs = {'class' : 'disease-list'})
            text = ''
            ul = content.find('ul')
            paragrahs = ul.contents
            for div in paragrahs :
                if 'div' == div.name :
                    title_tag = div.find_all('div')
                    for title in title_tag :
                        if '简介' == title.get_text() :
                            break
                        text += title.get_text() + '\n'
                else :
                    li = div.find_all(['h2', 'li', 'p'])
                    for p in li :
                        text += p.get_text() + '\n'

            return text
        except Exception as e :
            print (e)
            return ''

    def save(self, first_aid, content):
        try:
            headers = ['名称', '种类', '费用', '描述']
            file = path + '/丁香医生-查疫苗.csv'

            datas = [first_aid['name'], first_aid['type'], first_aid['money'], content]

            data_store.write_content(file, headers, datas)
        except Exception as e :
            print (e)

        return

    def perform(self) :
        try:
            #self.login()

            classifies = self.get_items()
            print ('all classify count:{}'.format(len(classifies)))
            #self.reconnet_chrome()

            index = 0
            for classify in classifies :
                content = self.get_content(classify['url'])
                self.save(classify, content)
                print ('get count : {}'.format(index))
                index += 1
                sleep(1)
        finally:
            self.browser.close()

        return

if __name__ == '__main__' :
    tool = SpiderDisesea()
    urls = tool.perform()
    print (urls)
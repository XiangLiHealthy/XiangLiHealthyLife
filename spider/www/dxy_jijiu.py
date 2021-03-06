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
            self.browser.get("https://dxy.com/firstaids")

            #get aid_card
            soup = BeautifulSoup(self.browser.page_source)
            aid_cards = soup.find_all('div', attrs = {'class' : 'first-aid-card'})

            first_aids = []
            for card in aid_cards :
                main_category = card.find('div', attrs = {'class' : 'header-title'})
                body_items = card.find_all('div', attrs = {'class' : 'body-item'})
                for body_item in body_items :
                    child_category = body_item.find('div', attrs = {'class' : 'common-tag-content'})
                    name_items = body_item.find_all('div', attrs = {'class' : 'link-line'})
                    for name in name_items :
                        first_aid = {}
                        first_aid['name'] = name.get_text()
                        first_aid['url'] = name.contents[0].attrs['href']
                        first_aid['main_category'] = main_category.get_text()
                        first_aid['child_category'] = child_category.get_text()
                        first_aids.append(first_aid)

            #get head1, head2, items
            return first_aids
        except Exception as e :
            print (e)

        return None

    def get_content(self, name, url):
        try:
            #opend web
            self.browser.get(url)
            soup = BeautifulSoup(self.browser.page_source)

            text = ''
            middle = soup.find('div', attrs = {'class' : 'article-detail-content'})
            text = middle.find('h1').get_text() + '\n'
            paragrahs = middle.find_all('div', attrs = {'class' : 'html-parse continuous-img-no-margin article-html'})
            index = 0
            for div in paragrahs :
                tags = div.find_all(['p', 'span', 'img', 'video'])
                for tag in tags :
                    if 'img' == tag.name or 'video' == tag.name :
                        url = tag.attrs['src']
                        pos = url.rfind('.')
                        prefix = url[pos:]
                        file = path + '/jijiu/{}-{}{}'.format(name, index, prefix)
                        index += 1
                        downloader.get_img(url, file)
                        text += '\n如图：{}\n'.format(file)
                    text += tag.get_text()

            return text
        except Exception as e :
            print (e)
            return ''

    def save(self, first_aid, content):
        try:
            headers = ['名称', '内容', '主分类', '二级分类']
            file = path + '/丁香医生-急救常识.csv'

            datas = [first_aid['name'], content, first_aid['main_category'], first_aid['child_category']]

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
                content = self.get_content(classify['name'], classify['url'])
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
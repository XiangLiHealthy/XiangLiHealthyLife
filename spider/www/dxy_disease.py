#import app_spider.downloader
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.wait import WebDriverWait
import bs4

from bs4 import BeautifulSoup
from time import sleep
import data_store

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

    def get_disease_urls(self) :
        try:
            self.browser.get('https://dxy.com/diseases/6948')
            soup = BeautifulSoup(self.browser.page_source, 'html.parser')
            div = soup.find('div', attrs={'class' : 'section-filter-box-common section-filter-box-border'})
            a = div.find_all('a')

            urls = []
            for item in a :
                href = item.attrs['href']
                text = item.find('span').get_text()

                tmp = {}
                tmp['url'] = href
                tmp['name'] = text

                urls.append(tmp)

            return urls
        except Exception as e:
            print (e)
            return None
    def parser_disease_html(self, html, tag_name):
        try:
            soup = BeautifulSoup(html, 'html.parser')
            elements = soup.find_all('a', attrs={'class':'section-card common-card-link'})

            diseases = []
            for element in elements :
                disease = {}
                disease['ch'] = tag_name
                disease['url'] = element.attrs['href']
                disease['name'] = element.contents[0].get_text()
                disease['brief'] = element.contents[1].get_text()
                diseases.append(disease)
        except Exception as e :
            print (e)
            return None

        return diseases

    def get_diseases(self, url) :
        try:
            #open web
            self.browser.get(url)
            buttons = self.browser.find_elements_by_xpath("//div[@class='section-filter-box-common']/div[2]/div")

            urls = []
            #get all charector diease
            for button in buttons :
                button.click()
                sleep(0.1)

                ch = self.parser_disease_html(self.browser.page_source, button.text)

                urls += ch

            #get all diseases by charector
            return urls
        except Exception as e :
            print (e)

        return None

    def get_content(self, url):
        try:
            #opend web
            self.browser.get(url)
            soup = BeautifulSoup(self.browser.page_source)

            disease_list = soup.find('div', attrs = {'class' : 'disease-list'})
            ul = disease_list.find('ul')

            divs = ul.find('div', attrs={'class':'tag-content'})
            li = ul.find_all('li')

            contents = ''
            #title
            for div in divs.contents :
                contents += div.get_text() + '\n'

            for tag in li :
                for tag1 in tag.contents :
                    if isinstance(tag1, bs4.Tag) :
                        for tag2 in tag1.contents :
                            if isinstance(tag2, bs4.Tag) :
                                contents += tag2.get_text() + '\n'
                            else :
                                contents += tag2 + '\n'
                        continue

                    if isinstance(tag1, bs4.Tag) :
                        contents += tag1.get_text() + '\n'
                    else :
                        contents += tag1

        except Exception as e :
            print (e)
            return ''

        return contents

    def save(self, department, disease):
        try:
            headers = ['科室', '字母', '疾病', '简介', '详细内容']
            file = path + '/丁香医生-疾病查询.csv'

            datas = [department, disease['ch'], disease['name'], disease['brief'], disease['detail']]

            data_store.write_content(file, headers, datas)
        except Exception as e :
            print (e)

        return

    def perform(self) :
        try:
            #self.login()

            urls = self.get_disease_urls()

            count = 0
            for url in urls :

                diseases = self.get_diseases(url['url'])
                print ('name:{}, count:{}'.format(url['name'], len(diseases)))
                if None == diseases :
                    continue

                for disease in diseases :

                    content = self.get_content(disease['url'])
                    count = count + 1
                    print ('get count:{}'.format(count))

                    disease['detail'] = content
                    self.save(url['name'], disease)
                    sleep(1)
        finally:
            self.browser.close()

        return

if __name__ == '__main__' :
    tool = SpiderDisesea()
    urls = tool.perform()
    print (urls)
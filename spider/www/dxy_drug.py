import app_spider.downloader
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
        #options = webdriver.ChromeOptions().add_argument('--headless')
        #self.browser = webdriver.Chrome(options=options)
        self.browser = webdriver.Chrome()
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

        return

    def reconnet_chrome(self):
        self.browser.close()
        self.browser = webdriver.Chrome()
        self.browser.implicitly_wait(5)

    def get_classify(self):
        classifies = []

        try:
            #get main classify
            self.browser.get('http://drugs.dxy.cn/')
            buttons = self.browser.find_elements_by_xpath("//div[@class='ant-tabs-nav-list']/div")
            for button in buttons :
                try:
                    sleep(0.1)
                    button.click()
                except Exception as e :
                    print (e)

            soup = BeautifulSoup(self.browser.page_source)
            divs = soup.find('div', attrs={'class' : 'ant-tabs-nav-list'})
            for div in divs.contents :
                id = div.contents[0].attrs['id']
                child_classify = soup.find('div', attrs={'aria-labelledby' : id})
                li = child_classify.find_all('li')

                for element in li :
                    a = element.find('a')
                    name = a.get_text()
                    href = a.attrs['href']

                    drug = {}
                    drug['main_classify'] = div.get_text()
                    drug['child_classify'] = name
                    drug['url'] = 'http://drugs.dxy.cn' + href

                    classifies.append(drug)
        except Exception as e :
            print (e)
            return classifies

        return classifies
    def get_pages_count(self, url):
        try:
            self.browser.get(url)
            soup = BeautifulSoup(self.browser.page_source, 'html.parser')
            paginations = soup.find_all('li', attrs={'class' : 'ant-pagination-item'})

            forward = ''
            for pagination in paginations :
                title = pagination.attrs['title']
                if 'Next Page' == title :
                    return forward

                forward = title
            return forward

        except Exception as e :
            print (e)

        return -1

    def get_drugs(self, url):
        try:
            drugs = []
            self.browser.get(url)
            soup = BeautifulSoup(self.browser.page_source)
            drugs_item = soup.find_all('div', attrs={'class' : 'drugs-item__3GYt'})
            for drug_item in drugs_item :
                h3 = drug_item.find('h3')
                p = drug_item.find('p')

                name = h3.contents[0].get_text()
                url = h3.contents[0].attrs['href']

                contents = p.find_all(['b', 'span'])
                text = ''
                for content in contents :
                    text += content.get_text() + '\n'

                drug = {}
                drug['name'] = name
                drug['url'] = 'http://drugs.dxy.cn' + url
                drug['brief'] = text

                drugs.append(drug)

        except Exception as e :
            print (e)

        return drugs

    def get_content(self, name, url):
        try:
            #opend web
            self.browser.get(url)
            soup = BeautifulSoup(self.browser.page_source)

            text = ''
            middle = soup.find('div', attrs = {'class' : 'container-middle'})
            divs = middle.contents[0].contents
            for div in divs :
                for p in div.contents :
                    if not isinstance (p, bs4.NavigableString) :
                        if 'img' == p.contents[0].name :
                            file = path + '/' + name + '.png'
                            data_store.get_img(p.contents[0].attrs['src'], file)
                        text += p.get_text()
                    text += '\n'

            return text
        except Exception as e :
            print (e)
            return ''

    def save(self, classify, drug, content):
        try:
            headers = ['名称', '简介', '内容', '主分类', '子分类']
            file = path + '/丁香医生-药品查询.csv'

            datas = [drug['name'], drug['brief'], content, classify['main_classify'], classify['child_classify']]

            data_store.write_content(file, headers, datas)
        except Exception as e :
            print (e)

        return

    def perform(self) :
        try:
            self.login()

            classifies = self.get_classify()
            print ('all classify count:{}'.format(len(classifies)))
            #self.reconnet_chrome()

            for classify in classifies :
                count = self.get_pages_count(classify['url'])
                for num in range(1, int(count)) :
                    url = 'http://drugs.dxy.cn/category/6FO9JRKee7oGjg4ueqcElw==?page={}'.format(num)
                    drugs = self.get_drugs(url)
                    for drug in drugs :
                        content = self.get_content(drug['name'], drug['url'])
                        self.save(classify, drug, content)
                        sleep(1)
        finally:
            self.browser.close()

        return

if __name__ == '__main__' :
    tool = SpiderDisesea()
    urls = tool.perform()
    print (urls)
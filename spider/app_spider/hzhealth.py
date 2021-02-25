#
import csv
import json

import requests
import chardet
import urllib.parse
from bs4 import BeautifulSoup

import urllib.request
import ssl
import os

ssl._create_default_https_context = ssl._create_stdlib_context

class HZHealth :
    def __init__(self):
        self.classification_api = 'https://muzi.heletech.cn/htalk1/api/action/getCategoryList'
        self.category_api = 'https://muzi.heletech.cn/htalk1/api/action/getActionList?categoryId='
        self.detail_api = 'https://muzi.heletech.cn/htalk1/api/action/getActionById?id='

        self.canDo = {}
        self.canDo['1'] = '能做'
        self.canDo['2'] = '注意'
        self.canDo['3'] = '禁止'

    def perform_http(self, url, varStream = False):
        result = ''
        user_agent = "Mozilla/4.0 (compatible;MSIE 5.5; Windows NT)"
        # 伪装成浏览器
        headers = {"User-Agent": user_agent}

        # 获取页面
        r = requests.get(url, headers=headers)
        # 如果返回的响应码为200,即可成功连接服务器
        if r.status_code == 200:
            if True == varStream :
                return r.content

            # 自动辨别网页的编码格式
            r.encoding = chardet.detect(r.content)["encoding"]
            return r.text

        return None

    def get_classification(self):
        #perform http
        res = self.perform_http(self.classification_api)


        classification_list = []
        soup = json.loads(res)

        return soup['data']
    def get_category_list(self, category_id):
        url = '{}{}'.format(self.category_api, category_id)
        res = self.perform_http(url)
        js = json.loads(res)
        return js['data']

    def get_detail_list(self, action_id):
        detail_list = []
        url = '{}{}'.format(self.detail_api, action_id['id'])

        res = self.perform_http(url)
        soup = json.loads(res)

        return soup['data']
    def write_content(self, detail, type):
        #create file name
        file_name = 'D:\jianhai\spider\data\杭州健康通-能不能做.csv'
        mode = ''
        if not os.path.exists(file_name):
            header = ['name', 'title', 'type', 'description', 'canDo']
            with open(file_name, 'w', encoding='utf-8-sig', newline='') as f:
                writer = csv.writer(f)
                writer.writerow(header)

        with open(file_name, 'a+', encoding='utf-8-sig', newline='') as f:
            try:
                data = []
                data.append(detail['name'])
                data.append(detail['title'])
                data.append(type)
                data.append(detail['description'])

                canDo = '{}'.format(detail['canDo'])
                data.append(self.canDo[canDo])
                datas = [data]

                writer = csv.writer(f)
                writer.writerows(datas)
            except Exception as e:
                print (e)

        return

    def performSpider(self):
        #get classifation list
        classification_list = self.get_classification()
        print ('get classification count:{}'.format(len(classification_list)))

        #get detail list
        classification_idx = 0
        for action in classification_list :
            detail_list = self.get_category_list(action['id'])
            print ('classification :{} action count:{}'.format(action['name'], len(detail_list)))

            # write into csv
            for detail in detail_list :
                classification_idx = classification_idx + 1
                print ('get detail idx:{}'.format(classification_idx))
                self.write_content(detail, action['name'])

        print ('get total count:{}'.format(classification_idx))

        return


if __name__ == '__main__' :
    spider = HZHealth()
    spider.performSpider()

    print ('complete spider')
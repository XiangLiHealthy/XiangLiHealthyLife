import csv
import json

import requests
import chardet
import urllib.parse
from bs4 import BeautifulSoup

import urllib.request
import ssl
import os
import downloader

ssl._create_default_https_context = ssl._create_stdlib_context
g_header = {}
g_header['user-agent'] = 'Mozilla/5.0 (Linux; Android 6.0.1; DUK-AL20 Build/V417IR; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/52.0.2743.100 Mobile Safari/537.36 MMWEBID/1448 MicroMessenger/7.0.22.1820(0x27001636) Process/toolsmp WeChat/arm32 Weixin NetType/WIFI Language/zh_CN ABI/arm64'
g_header['x-requested-with'] = 'XMLHttpRequest'
g_header['accept'] = '*/*'
g_header['referer'] = 'https://mp.weixin.qq.com/mp/profile_ext?action=home&__biz=MzI5OTM2NDA0Nw==&subscene=0&devicetype=android-23&version=27001636&lang=zh_CN&nettype=WIFI&a8scene=7&session_us=gh_8fef98253d05&pass_ticket=BhCq5K14wgYX6oIXCat%2FDZqYqw%2FN34Up5ixDijO%2FX5Ng%2Bn2iCNtGrZh0a72MqLei&wx_header=1'
g_header['accept-coding'] = 'gzip, deflate'
g_header['accept-language'] = 'zh-CN,en-US;q=0.8'
g_header['cookie'] = 'rewardsn='
g_header['cookie'] = 'wxuin=941412330'
g_header['cookie'] = 'devicetype=android-23'
g_header['cookie'] = 'version=27001636'
g_header['cookie'] = 'lang=zh_CN'
g_header['cookie'] = 'pass_ticket=BhCq5K14wgYX6oIXCat/DZqYqw/N34Up5ixDijO/X5Ng+n2iCNtGrZh0a72MqLei'
g_header['cookie'] = 'wap_sid2=COqf88ADEooBeV9ISmhKRW5pYkdueThsbVozNlhDLXpaMHVQREl2b3lWWHJyZjY2WWJLQ1M5V1kzOTBwMzZmdDFtNjBVSUk4WmxvcnJXcWNGdTV5VFlaZERCMjRoVFZlb0FrMUcxcTBDVjVfeHh1bXJ6Y3JJVXA3QjFTa0ZBZ2ZfdzQwMVpVbW9NeFBDZ1NBQUF+ML/R7oAGOA1AAQ=='
g_header['cookie'] = 'wxtokenkey=777'

class WXAiGanLianMeng :
    def __init__(self):
        self.downloader = downloader.Downloader()
        self.header = {}


    def get_article_list(self, offset):
        #create url
        url = 'https://mpweixin.qq.com/mp/profile_ext?action=getmsg&__biz=MzI5OTM2NDA0Nw==&f=json&offset={}&count=10&is_ok=1&scene=&uin=777&key=777&pass_ticket=BhCq5K14wgYX6oIXCat%2FDZqYqw%2FN34Up5ixDijO%2FX5Ng%2Bn2iCNtGrZh0a72MqLei&wxtoken=&appmsg_token=1099_NdImYU3mc25oEumQNB5DwFC72Cfu3kQ4mkZa2g~~&x5=0&f=json'.format(offset)

        #perform https
        res = self.downloader.get(url, g_header)

        #parser url
        js = json.loads(res)

        return

    def get_article(self, url):
        #perform https

        #parser article

        return

    def save_text(self, title, contents):
        #create file

        #write context

        return

    def get_articles(self):
        #initialize offset
        offset = 0
        articles = []

        # get all list
        while offset < 530 :
            articles_tmp = self.get_article_list(offset)
            articles += articles_tmp
            offset += len(articles_tmp)

        return articles

    def perform_spider(self):
        #get article list
        articles = self.get_articles()
        print ('get article count:{}'.format(len(articles)))

        for article in articles :
            #get article
            context = self.get_article(article['url'])

            #write to text
            self.save_text(context['title'])

        return

if __name__ == '__main__' :
    wx = WXAiGanLianMeng()
    wx.perform_spider()
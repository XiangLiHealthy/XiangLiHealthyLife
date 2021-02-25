import requests
import chardet
import urllib.parse
from bs4 import BeautifulSoup

import urllib.request
import ssl
import os

ssl._create_default_https_context = ssl._create_stdlib_context

class Downloader:
    def get(self, url, headers, var_stream = False):
        result = ''

        proxy = {'http': '33.33.33.10:8118'}

        # 获取页面
        r = requests.get(url, headers=headers, proxies=proxy)
        # 如果返回的响应码为200,即可成功连接服务器
        if r.status_code == 200:
            if True == var_stream :
                return r.content

            # 自动辨别网页的编码格式
            r.encoding = chardet.detect(r.content)["encoding"]
            return r.text

        return None
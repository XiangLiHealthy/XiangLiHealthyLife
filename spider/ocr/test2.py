import requests
import os
from requests.exceptions import ReadTimeout
import chardet
from bs4 import BeautifulSoup
import re
import json
import math
from PIL import Image
#import pdfkit
#from GetPpt import GetPpt
import time
import ssl
import urllib.request

ssl._create_default_https_context = ssl._create_stdlib_context

def test_spider(url) :
    headers = {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) '
                             'Chrome/51.0.2704.63 Safari/537.36'}
    req = urllib.request.Request(url=url, headers=headers)

    res = urllib.request.urlopen(req)

    data = res.read()
    soup = BeautifulSoup(data, 'html.parser')

    return

if __name__ == '__main__':
 # 若存储路径为空，则在当前文件夹生成
 test_spider('http://www.a-hospital.com/w/%E6%80%A5%E6%95%91%E5%B8%B8%E8%AF%86')




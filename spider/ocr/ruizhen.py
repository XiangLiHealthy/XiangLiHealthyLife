#! /usr/bin/env python3
# -*- coding:utf-8 -*-

import requests
import chardet

def updateImage(filepath):
    upload_url = " http://trial.web.glority.cn:8000/classify?type=20"
    header = {"Content-type": "application/octet-stream"}
    # 此处是重点！我们操作文件上传的时候，
    # 把目标文件以open打开，然后存储到变量upfile里面存到一个字典里，注意这里的upfile不是随便起的，要看后台的接收数据的时候，使用的接收文件信息的key是如何定义的。一定要按照服务端的key来
    files = {'image_file': open(filepath, 'rb')
             }


    ##此处是重点！我们操作文件上传的时候，接口请求参数直接存到upload_data变量里面，
    # 在请求的时候，直接作为数据传递过去
    r = requests.post(upload_url,
                               files=files,
                               headers=header)
    if r.status_code == 200 :
        r.encoding = chardet.detect(r.content)["encoding"]
        return r.text

    return None


if __name__ == '__main__':
    r = updateImage('/home/xiangbaosong/下载/血常规/血常规/A1.jpg')
    print (r)
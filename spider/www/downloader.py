import urllib
import chardet
import requests

headers = {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) '
                         'Chrome/51.0.2704.63 Safari/537.36'}
s = requests.session()
s.headers.clear()
s.headers.update(
    headers
)

def get_byte(url) :

    req = urllib.request.Request(url=url, headers=headers)

    res = urllib.request.urlopen(req)

    data = res.read()


    return data

def get_text(url) :
    try:
        #req = urllib.request.Request(url=url, headers=headers)

        #r = urllib.request.urlopen(req, timeout = 5)
        r = s.get(url, timeout = 5)
        if r.status_code == 200:
            # 自动辨别网页的编码格式
            r.encoding = chardet.detect(r.content)["encoding"]
            return r.text

    except Exception as e :
        print (e)

    return None


def get_img(url, file_name) :
    data = get_byte(url)

    try:
        with open(file_name, "wb") as f:
            f.write(data)

    except Exception as e:
        print(e)

    return
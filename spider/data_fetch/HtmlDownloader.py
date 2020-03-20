import requests
import chardet

class HtmlDownloader:
    def download(self, url):
        if url is None:
            return None

        user_agent = "Mozilla/4.0 (compatible;MSIE 5.5; Windows NT)"
        # 伪装成浏览器
        headers = {"User-Agent" : user_agent}
        # 获取页面
        r = requests.get(url, headers = headers)
        # 如果返回的响应码为200,即可成功连接服务器
        if r.status_code == 200:
            # 自动辨别网页的编码格式
            r.encoding = chardet.detect(r.content)["encoding"]
            return r.text

        return None

if __name__ == "__main__":
    url = "http://www.woidm.com/"

    htmld = HtmlDownloader()
    htmlText = htmld.download(url)

    print(type(htmlText))
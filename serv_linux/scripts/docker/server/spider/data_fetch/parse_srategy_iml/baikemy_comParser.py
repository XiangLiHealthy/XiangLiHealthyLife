from HtmlParserStrategy import HtmlParserStrategy
import urllib.parse
from bs4 import BeautifulSoup


class baikemy_comParser(HtmlParserStrategy):
    def __init__(self):
        HtmlParserStrategy.__init__(self)
        #super(baikemy_comParser, self).__init__(self)

        self.url = "https://www.baikemy.com/disease/list/0/0?diseaseContentType=A"
        self.station = 'www.baikemy.com'

    def parser_station(self, root_url, html):
        if root_url is None or html is None:
            return

        soup = BeautifulSoup(html, "html.parser", from_encoding="utf-8")
        new_urls = []
        # 抽取符合要求的a 标记
        links = soup.find_all("div", "typeInfo_List")
        for link in links:

            items = link.find_all("li", "typeInfo_Li")

            for item in items:

                href = item.find("a")
                disease = href.contents[0]
                url = href['href']
                new_url = urllib.parse.urljoin(root_url, url)

                data = {}
                data['disease'] = disease
                data['url'] = new_url

                new_urls.append(data)

        return new_urls

    def parser_content(self, html):
        soup = BeautifulSoup(html, "html.parser", from_encoding="utf-8")
        data = {}

        #data['url'] = page_url

        # get detail name
        #detail_name_tag = soup.find_all("div", "detail_name")
        #data['detail_name'] = detail_name_tag[0].string;

        # get pupulatorVersion
        data['elements'] = []
        p_directory = soup.find_all("div", id="popularVersion")
        data['elements'] = self._get_contents(p_directory)

        # get specialVersion
        desc_tags = soup.find_all("div", id="specialityVersion")
        data['elements'] += self._get_contents(desc_tags)

        return data

    def _get_contents(self, directory):
        p_datas = []

        for p_directory in directory:
            divs = p_directory.find_all("div", recursive=False)

            for div in divs:
                h1 = div.find_all("h1")
                ps = div.find_all("p")

                contents = ''
                for p in ps:
                    content = ''
                    content = self._merge_contents(p)
                    contents += content

                h_data = h1.pop()
                item = {}
                item['head'] = h_data.string
                item['content'] = contents

                p_datas.append(item)

        return p_datas

    def _merge_contents(self, contents):
        content = ''
        list = contents.contents;
        for item in list:
            if item.string is not None:
                content = content + item.string

        return content

if __name__ == "__main__":
    test = baikemy_comParser;
    print(test)

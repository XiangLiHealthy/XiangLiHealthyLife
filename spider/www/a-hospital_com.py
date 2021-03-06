
import downloader
import bs4
import data_store
import os

path = '/home/xiangbaosong/work'

class SpiderCommonSense :
    def get_diseases(self):
        urls = []
        father_url = 'http://www.a-hospital.com/w/%E6%80%A5%E6%95%91%E5%B8%B8%E8%AF%86'

        #requests html
        html = downloader.get_text(father_url)

        #get tag li
        soup = bs4.BeautifulSoup(html, 'html.parser')

        #parser url
        li_tags = soup.find_all(['h2', 'li'])

        h2 = ''
        for item in li_tags :
            try:
                if item.name == 'li' and len(item.attrs) != 0:
                    continue

                if item.name == 'h2' and  item.contents[0].attrs['class'][0] != 'mw-headline':
                    continue

                if 'h2' == item.name :
                    h2 = item.get_text()
                    continue

                node = {}
                node['type'] = h2
                node['name'] = item.get_text()
                node['url'] = 'http://www.a-hospital.com' + item.contents[0].attrs['href']
                if '急救的任务和次序' == h2 :
                    break

                urls.append(node)
            except Exception as e:
                print (e)
                continue

        return urls

    def get_content(self, type, name, disease_url):
        contents = ''

        html = downloader.get_text(disease_url)
        soup = bs4.BeautifulSoup(html, 'html.parser')

        divide = '更多医学百科条目 目前暂无留言 站外链接 参看 目录 视频'
        li = soup.find_all(['h2', 'p'])
        for text in li :
            try:
                tmp = text.get_text()
                idx = divide.find(tmp)
                if tmp == '目录' or tmp == '更多医学百科条目' or tmp == '目前暂无留言\n' or tmp == '站外链接' '更多医学百度条目' == tmp:
                    continue

                contents += '\n'
                if '' == tmp and text.contents[0].name != 'img':
                    continue

                if text.contents[0].name == 'img':
                    src = text.contents[0].attrs['src']
                    file_name = path + '/' + type + "-" + name + '-' + tmp + '.jpg'
                    downloader.get_img(src, file_name)

                contents += tmp
            except Exception as e :
                print(e)

        return contents

    def save(self, obj):
        datas = []
        datas.append(obj['type'])
        datas.append(obj['name'])
        datas.append(obj['content'])

        file_name = path + '/疾病百科.csv'
        headers = ['类型', '名称', '内容']

        data_store.write_content(file_name, headers, datas)

        return

    def perform(self):
        try:
            diseases = self.get_diseases()

            for obj in diseases :
                obj['content'] = self.get_content(obj['type'], obj['name'], obj['url'])
                self.save(obj)

        except Exception as e :
            print (e)

        return

class SpiderBaike:
    def get_charectors(self):
        urls = []
        for charector in range(71, 91) :
            url = 'http://www.a-hospital.com/w/%E7%96%BE%E7%97%85%E6%9D%A1%E7%9B%AE%E7%B4%A2%E5%BC%95-' + chr(charector)

            item = {}
            item['name'] = chr(charector)
            item['url'] = url
            urls.append(item)
        return urls

    def get_partments(self, url):
        #download html
        html = downloader.get_text(url)

        #parser
        soup = bs4.BeautifulSoup(html, 'html.parser')

        h4s = soup.find_all(['h3', 'h4'])

        res = []
        for title in h4s :
            text = title.get_text()
            h_url = 'http://www.a-hospital.com' + title.contents[0].attrs['href']

            item = {}
            item['name'] = text
            item['url'] = h_url

            res.append(item)

        return res

    def get_diseases(self, url):
        # download html
        html = downloader.get_text(url)

        # parser
        soup = bs4.BeautifulSoup(html, 'html.parser')

        div = soup.find('div', attrs = {'id': 'bodyContent'})
        lis = div.find_all('li')

        urls = []
        for tag in lis :
            if tag.contents[0].name != 'a':
                break

            text = tag.get_text()
            href = tag.contents[0].attrs['href']

            url = {}
            url['name'] = text
            url['url'] = 'http://www.a-hospital.com' + href
            urls.append(url)

        return urls

    def get_content(self, url):
        # download html
        html = downloader.get_text(url)

        # parser
        soup = bs4.BeautifulSoup(html, 'html.parser')

        div = soup.find('div', attrs={'id': 'content'})

        contents = div.find_all(['h1', 'h2', 'h3', 'p'])
        content = ''
        for tag in contents :
            text = tag.get_text()
            if '参看' == text or '目前暂无留言' == text:
                break

            content += text + '\n'

        return content

    def save_content(self, department, name, content):
        headers = ['字母', '名称', '内容']

        columns = []
        columns.append(department)
        columns.append(name)
        columns.append(content)

        file = path + '/疾病百科.csv'
        data_store.write_content(file, headers, columns)

        return

    def perform(self):
        try:
            #1.get parrment
            departments = self.get_charectors()

            #2.get disease list
            count = 0
            for department in departments :
                diseases = self.get_diseases(department['url'])
                print ('char {} count:{}'.format(department['name'], len(diseases)))

                for disease in diseases :
                    try:
                        content = self.get_content(disease['url'])
                        self.save_content(department['name'], disease['name'], content)
                        count = count + 1
                        print ('get cunnt :{}'.format(count))
                    except Exception as e :
                        print (e)
            #3.get content

            #4.get save

        except Exception as e :
            print (e)

        return

class SpiderTestsheet :
    def get_items(self, url):
        # download html
        html = downloader.get_text(url)

        # parser
        soup = bs4.BeautifulSoup(html, 'html.parser')

        tags = soup.find_all(['p', 'li'])
        items = []
        title = ''
        for tag in tags :
            if len(tag.attrs) > 0 :
                continue

            try:
                item = {}
                if 'p' == tag.name:
                    title  = tag.get_text()
                    p_title = tag.contents[0].attrs['title']
                    if p_title.find('尚未撰写') == -1:
                        p_href  = tag.contents[0].attrs['href']
                        item['name'] = title
                        item['type'] = title
                        item['url'] = 'http://www.a-hospital.com' + p_href
                        items.append(item)
                        continue

                li_text = tag.get_text()
                if li_text.find('登录') != -1 :
                    break

                li_href = tag.contents[0].attrs['href']
                item['name'] = li_text
                item['type'] = title
                item['url'] = 'http://www.a-hospital.com' + li_href
                items.append(item)
            except Exception as e :
                print(e)

        return items

    def get_contents(self, url):
        # download html
        html = downloader.get_text(url)

        # parser
        soup = bs4.BeautifulSoup(html, 'html.parser')

        div = soup.find('div', attrs = {'id' : 'bodyContent'})
        tags = div.find_all(['p', 'h2', 'table', 'li'])
        content = ''
        for tag in tags :
            text = tag.get_text()
            if '参看' ==  text or '怎样看化验单专题' == text:
                break

            if 'table' == tag.name :
                for row in tag.contents :
                    if isinstance(row , bs4.NavigableString) :
                        content += '\n'
                        continue

                    row_text =  ''
                    for col in row.contents :
                        if isinstance(col, bs4.NavigableString) :
                            row_text += '    '
                            continue
                        row_text += col.get_text()

                    content += row_text

                continue

            content += text + '\n'

        return content

    def save(self, name, type, content, url):
        headers = ['名称', '类型', '内容', 'url']
        file = path + '/怎么看化验单.csv'

        datas = []
        datas.append(name)
        datas.append(type)
        datas.append(content)
        datas.append(url)

        data_store.write_content(file, headers, datas)

        return

    def perform(self):
        try:
            #1.get all itmes
            items = self.get_items('http://www.a-hospital.com/w/%E5%B8%B8%E8%A7%81%E5%8C%96%E9%AA%8C%E6%8C%87%E6%A0%87%E5%8F%8A%E6%84%8F%E4%B9%89')

            #2.get content
            index = 0
            for item in items :
                try:
                    content = self.get_contents(item['url'])
                    self.save(item['name'], item['type'], content, item['url'])
                    index = index + 1
                    print ('get count:{}/{}'.format(index, len(items)))
                except Exception as e :
                    print ('error:{},url:{}'.format(e, item['url']))
            #3. save content
        except Exception as e :
            print (e)
        return



if __name__ == '__main__' :
    #disease = SpiderCommonSense()
    #disease = SpiderBaike()
    disease = SpiderTestsheet()
    disease.perform()

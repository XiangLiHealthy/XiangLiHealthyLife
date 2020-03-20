

class HtmlParserStrategy:
    def __init__(self):
        self.station = ''
        self.url = ''

    def get_station(self):
        return self.station

    def get_url(self):
        return self.url

    def parser_station(self, root_url, html):
        pass

    def parser_content(self, html):
        pass


from DataStore import DataStore
from UrlManager import UrlManager
from HtmlDownloader import HtmlDownloader
from ParserStrategyContainer import ParserStrategyContainer


class SpiderMan:
    def __init__(self):
        self.data_store = DataStore()
        self.url_manager = UrlManager(self.data_store)
        self.strategy_container = ParserStrategyContainer()
        self.downloader = HtmlDownloader()

    def start(self):
        self.register_station()

        while self.has_task():
            try:
                self.handle_station()

                self.handle_content()

            except Exception as e:
                print(e)

    def has_task(self):
        station_count = self.url_manager.get_stations_size()
        contents_count = self.url_manager.get_contents_size()

        all_count = station_count + contents_count

        return all_count > 0

    def register_station(self):
        strategys = self.strategy_container.get_strategys()
        for key, value in strategys.items():
            self.data_store.add_station_url(key, value.get_url())

    def handle_station(self):
        data = self.url_manager.get_station_url()
        if data is None:
            return

        strategy = self.strategy_container.get_srategy(data['station'])

        # get all urls
        html = self.downloader.download(data['url'])

        #for test
        #fin = open("test", "w", encoding="utf-8")
        #fin.write(html)

        #fin = open("test", "r", encoding="utf-8")
        #html = fin.read();

        content_urls = strategy.parser_station(data['url'], html)


        # store urls to mysql
        perform_count = 0
        for content_url in content_urls:
            disease = content_url['disease']
            tmp_url = content_url['url']
            perform_count += self.url_manager.add_content_url(tmp_url, disease, data['station'])

        #updata sation state
        state = 1
        if len(content_urls) > perform_count or len(content_urls) == 0:
            state = -1

        self.data_store.updata_staion_state(state, data['station'])

    def handle_content(self):
        data = self.url_manager.get_content_url()
        strategy = self.strategy_container.get_srategy(data['station'])

        # get page contents
        html = self.downloader.download(data['url'])
        contents = strategy.parser_content(html)

        # store contents to mysql
        state = 0
        code = 1
        contents['knowledge_id'] = data['knowledge_id']
        if self.data_store.add_content(contents) < 0:
            state = -1

        #update state
        self.data_store.update_page_state(code, data['knowledge_id'])


if __name__ == "__main__":
    spider_man = SpiderMan()
    spider_man.start()


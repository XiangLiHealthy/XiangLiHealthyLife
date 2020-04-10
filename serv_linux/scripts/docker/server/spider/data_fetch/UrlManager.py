import DataStore
from collections import namedtuple


class UrlManager:
    def __init__(self, ds):
        self.ds = ds
        self.stations = []
        self.contents = []

    def get_station_url(self):
        if len(self.stations) < 1:
            station_datas = self.ds.get_station_urls()
            self.stations = station_datas

        if (len(self.stations) > 0) :
            station = self.stations[0]
            del(self.stations[0])
            return station

        return None

    def get_content_url(self):
       if len(self.contents) < 1 :
           contens = self.ds.get_contents_urls()
           self.contents = contens

       if len(self.contents) > 0:
           content_url = self.contents[0]
           del(self.contents[0])
           return content_url

       return None

    def add_content_url(self, disease, url, station):
        return self.ds.add_page_url(disease, url, station)

    def get_stations_size(self):
        if len(self.stations) < 1:
            station_datas = self.ds.get_station_urls()
            self.stations = station_datas

        return len(self.stations)

    def get_contents_size(self):
        #get contents url from mysql
        if (len(self.contents) < 1) :
            contens = self.ds.get_page_urls()
            self.contents = contens

        return len(self.contents)
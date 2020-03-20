from parse_srategy_iml.baikemy_comParser import baikemy_comParser


class ParserStrategyContainer:
    def __init__(self):

        tmp = baikemy_comParser()
        station = tmp.get_station()

        self.strategys = {station: tmp}

    def get_strategys(self):
        return self.strategys

    def get_srategy(self, station):
        return self.strategys[station]



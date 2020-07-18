import codecs
import pymysql

class DataStore:
    def __init__(self):
        self.db = pymysql.connect(host='127.0.0.1', port=3306, user='xiangli', passwd='123456', db='HealthyLife',
                              charset='utf8')
        self.db_cursor = self.db.cursor()


    def get_station_urls(self):
        try:
            sql = "select * from station where state = 0 limit 10;"
            self.db_cursor.execute(sql)

            datas = []
            rows = self.db_cursor.fetchall()
            for row in rows:
                data = {}
                data['station'] = row[0]
                data['url'] = row[1]
                data['state'] = row[2]
                datas.append(data)
            return datas

        except Exception as e:
            print(e)
            return None

    def get_page_urls(self):
        try:
            sql = "select * from medical_raw where state = 0 limit 100;"
            self.db_cursor.execute(sql)

            rows = self.db_cursor.fetchall()
            datas = []
            for row in rows:
                data = {}
                data['knowledge_id'] = row[0]
                data['station'] = row[3]
                data['url'] = row[2]
                datas.append(data)

            return datas
        except Exception as e:
            print(e)
            return  None

    def add_station_url(self, station, url):
        sql = "insert into station (station, url, state, add_time)" \
              " select '{}', '{}', 0, NOW() from dual " \
              "where not exists (select * from station where url" \
              " = '{}' limit 1);".format(station, url, url)
        self.perform_sql(sql)

    def add_page_url(self, url, disease, station):
        sql = "insert into medical_raw (src_url, disease, import_time, " \
              "state, station) select '{}', '{}', NOW(), 0, '{}' from DUAL where NOT EXISTS " \
              "(select * from medical_raw where src_url = '{}' limit 1);".format(url, disease, station, url)
        return self.perform_sql(sql)

    def add_content(self, data):
        count = 0
        code = 0

        id = data['knowledge_id']
        contents = data['elements']
        for content in contents:
            title = content['head']
            text = content['content']
            sql = "insert into diagnosis_element_tmp (knowledge_id, title, content, cleaning_state) values (" \
                  " {}, '{}', '{}',0 );".format( id, title, text)
            count += self.perform_sql(sql)

        if len(contents) > count :
            code = -1

        return code

    def updata_staion_state(self, state, station):
        sql = "update station set state = %d where station = '%s';" %(state, station)
        self.perform_sql(sql)

    def update_page_state(self, state, id):
        sql = "update medical_raw set state = %d where knowledge_id = '%d'" %(state, id)
        self.perform_sql(sql)

    def perform_sql(self,sql):
        try:
            self.db_cursor.execute(sql)
            self.db.commit()
            return 1

        except Exception as e:
            print(e)
            print(sql)
            return 0

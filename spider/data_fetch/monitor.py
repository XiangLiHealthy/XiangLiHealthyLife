from builtins import Exception

import pymysql
import time

class Monitor:
    def __init__(self):
        self.db = pymysql.connect(host='127.0.0.1', port=3306, user='xiangbaosong', passwd='Xl2016xl', db='HealthyLife',
                              charset='utf8')
        self.db_cursor = self.db.cursor()

    def perform(self):
        try:
            sql = "select * from " \
                  "( " \
                  "(select count(*) from medical_raw where state = 1) as finish_cout," \
                  "(select count(*) from diagnosis_element_tmp) as element_count" \
                  ")"
            self.db_cursor.execute(sql)
            self.db.commit()

            data = self.db_cursor.fetchone()
            msg = "finish_cunt:%d, element_count:%d" %(data[0], data[1])
            print(msg)

        except Exception as e :
            print(e)

    def start(self):
        while 1 :
            self.perform()
            time.sleep(5)

if __name__ == "__main__":
    monitor = Monitor()
    monitor.start()



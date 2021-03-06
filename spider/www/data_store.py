import os
import csv

def write_content(file_name, headers, colomns):
    # create file name

    mode = ''
    if not os.path.exists(file_name):
        with open(file_name, 'w', encoding='utf-8-sig', newline='') as f:
            writer = csv.writer(f)
            writer.writerow(headers)

    with open(file_name, 'a+', encoding='utf-8-sig', newline='') as f:
        try:
            datas = [colomns]
            writer = csv.writer(f)
            writer.writerows(datas)
        except Exception as e:
            print(e)

    return
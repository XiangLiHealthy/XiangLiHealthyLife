from urllib import request
import json
import requests

#import xlwt

# TODO
amap_web_key = '6f65134da2516b1a5cf12350e99f7710'  # 高德地图官网申请的Web API KEY
filename = r'C:\\Users\\hgvgh\\Desktop\\hehe.xls'  # 爬取到的数据写入的EXCEL路径

# 多边形边界集合：
'''
规则：经度和纬度用","分割，经度在前，纬度在后，坐标对用"|"分割。         
多边形为矩形时，可传入左上右下两顶点坐标对；其他情况下首尾坐标对需相同。
'''
polygon_list = [
    '104.01991721224772,30.675517485068525|104.04335935135217,30.69685368121003|104.10490633670742,30.681154431514038|104.10713973787826,30.642336396516658|104.05432192815479,30.623029707583|104.02008888045066,30.64717739010567|104.01991721224772,30.675517485068525']

# POI分类集合, 多个类型用竖线 | 分割；
type_list = '|120000|120100|120201|120002|120003|120300|120301|120302|140000|060402|060403|060404|060405|060406|060407|060408|060409|060411|060413|060414|060415|060100|060101|060102|060103'  # |060402|060403|060404|060405|060406|060407|060408|060409|060411|060413|060414|060415|060100|060101|060102|060103'

# 输出坐标系：0,默认高德坐标系，1,百度坐标系，2,WGS84坐标系
output_coordinate = 2

poi_search_url = "http://restapi.amap.com/v3/place/polygon"  # URL

offset = 25  # 分页请求数据时的单页大小


# 根据矩形坐标获取poi数据
def getpois(polygon, type_list):
    i = 1
    current_polygon_poi_list = []
    while True:  # 使用while循环不断分页获取数据
        result = getpoi_page(polygon, i, type_list)
        result = json.loads(result)  # 将字符串转换为json

        # print('第', str(i),'页，结果',result)

        if result['status'] is not '1':  # 接口返回的状态不是1代表异常
            print('======爬取错误，返回数据：' + result)
            break
        pois = result['pois']
        if len(pois) < offset:  # 返回的数据不足分页页大小，代表数据爬取完
            current_polygon_poi_list.extend(pois)
            break
        current_polygon_poi_list.extend(pois)
        i += 1
    print('===========当前polygon：', polygon, ',爬取到的数据数量：', str(len(current_polygon_poi_list)))

    return current_polygon_poi_list


# 单页获取pois
'''
http://restapi.amap.com/v3/place/polygon?polygon=116.80708,31.449926,117.510206,32.247823&key=c5304f29d1a11f14c4fb29854a831ef0&extensions=all&offset=5&page=1
'''


def getpoi_page(polygon, page, type_list):
    print(polygon)
    # req_url = poi_search_url + "?key=" + amap_web_key + '&extensions=all&polygon=' + polygon + '&offset=' + str(
    #    offset) + '&types=' + type_list + '&page=' + str(page) + '&output=json'
    req_url = poi_search_url + "?key=" + amap_web_key + '&extensions=all&polygon=' + polygon + '&offset=' + str(
        offset) + '&page=' + str(page) + '&output=json'

    data = ''
    with request.urlopen(req_url) as f:
        data = f.read()
        data = data.decode('utf-8')
        print(data)
    return data


# 获取指定位置经纬度
#url:https://restapi.amap.com/v3/geocode/geo?address=重庆&output=XML&key=6f65134da2516b1a5cf12350e99f7710
def get_location(address):
    url = "https://restapi.amap.com/v3/geocode/geo?address=%s&output=XML&key=6f65134da2516b1a5cf12350e99f7710" % address
    parameters = {'key': '6f65134da2516b1a5cf12350e99f7710', 'address': address}
    base = 'https://restapi.amap.com/v3/geocode/geo'
    response = requests.get(base, parameters)
    answer = response.json()

    gecodes = answer['geocodes']
    data = gecodes[0]
    location = data['location']
    return location

def get_polgon():
    addresses = ['徐庄软件园研发三区1栋E(东北门)','南京市栖霞区仙境路18号', '南京康富汽车维修中心', '南京市秦淮区御道街33-11号']
    polgon_tmp = []
    for address in addresses:
        location = get_location(address)
        polgon_tmp.append(location)

    print(addresses)
    print(polgon_tmp)
    return polgon_tmp

def format_polygon(polgon):
    result = ''
    for item in polgon:
        result += item + "|"

    result = result[:-1]

    return result

import csv
def write_csv(all_pois):
    header = ['name','location', 'type', 'address', 'tel']
    with open('/home/xiangbaosong/result.csv', 'w', encoding='utf-8', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(header)

        for poi in all_pois:
            data = []
            data.append(poi['name'])
            data.append(poi['location'])
            data.append(poi['type'])
            data.append(poi['address'])
            data.append(poi['tel'])
            datas = [data]
            writer.writerows(datas)


print(output_coordinate == 2)

all_poi_list = []  # 爬取到的所有数据

polgons = get_polgon()
format_pol = format_polygon(polgons)

#for polgon in polygon_list:
polygon_poi_list = getpois(format_pol, type_list)
all_poi_list.extend(polygon_poi_list)

#print('爬取完成,总的数量', len(all_poi_list))
#write_to_excel(all_poi_list, filename)
write_csv(all_poi_list)
print('写入成功')
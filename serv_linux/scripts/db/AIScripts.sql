--数据库名称: HealthyLife

---//1.描述字典表 诊断原理使用是的排列组合原理
create table dignosis_element
(
    dignosis_element_id bigint auto_increament primary key,
    content varchar(256),
    flag varchar(32) ------ /* 1 症状描述 2 致病原因 3 诊断结论 4 解决方法 5 检查 ``` 6 并发症      7 预防 */
    
)charset = utf8,egine = InnoDB
;

----//2.诊断方案表
create table diagnosis 
(
    diagnosis_id bigint auto_increament primary key,
    user_id bigint,
    symptom_detail varchar(1024),
    cause_detail varchar(1024),
    diagnosis_detail varchar(1024),
    solution_detail varchar(1024),
    feedback int,
    data_source varchar(64),
    create_time datetime,
    status int
)engine = InnoDB, charset = utf8;

create table diagnosis_element_map
{
    diagnosis_id bigint,
    diagnosis_element_id bigint
}engine = InnoDB, charset = utf8;

----//3.医学知识库表
create table medical 
( 
    medical_id bigint auto_increment primary key, 
    create_time datetime not null, 
    flag int not null default 0 ,----1 中医 ，2 西医
    src_url varchar(1024),
    name varchar(64)
    )engine = InnoDB, charset = utf8;

create table medical_dict
(
    medical_id bigint foreign key(medical) references,
    dignosis_element_id bigint foreign key(diagnosis_element) references,
)engine = InnoDB, charset = utf8;

------//4.病例表
create table case 
(
 case_id bigint auto_increament primary key,
 user_id bigint,
 foreign key(use_id) references use(user_id),
 create_time datetime,
 endtime datetime,
 status int,  --------- /* 0 生成 1 完成 -1 删除 */
)engine = InnoDB, charset = utf8
;

------//5.病例详细描述表
create tabled case_detail 
(
 case_detail_id bigint auto_increament primary key,
 case_id bigint,
 foreign key(case_id) references case(case_id),
 detail varchar(1024),
 type int
)engine = InnoDB, charset = utf8
;

-----//6.图片表
create table case_picture
(
 case_picture_id bigint auto_increament primary key,
 case_id bigint,
 foreign key(case_id) references case(case_id),
 type int,
 path varchar(260),

)engine = InnoDB, charset = utf8
;

-----//7.用户表
create table user 
(
    user_id bigint auto_increament primary key,
    tel varchar(16),
    password varchar(32) not null,
    name varchar(64),
    sex varchar(8),
    tall int,
    weight int,
    birthday date,
    native_place varchar(256),
    family varchar(256),
    marital_status int,
    blood_type int,
    occupation varchar(128),
)engine = InnoDB, charset = utf8
;

----爬虫数据缓存表
create table medical_tmp 
(
    knowledge_id bigint auto_increment primary key,
    disease varchar(64) ,
    src_url varchar(1024),
    station varchar(128),
    state int default 0,
    import_time datetime ,
)charset=utf8,engine = InnoDB;

create table diagnosis_element_tmp 
( 
    element_id bigint auto_increment primary key, 
    knowledge_id bigint,
    title varchar(128), 
    content varchar(4096), 
    cleaning_state int default 0
) charset = utf8, engine = InnoDB;

create table station 
( 
    station varchar(128), 
    url varchar(1024), 
    state int default 0,
    add_time datetime
 )charset = utf8, engine = InnoDB;
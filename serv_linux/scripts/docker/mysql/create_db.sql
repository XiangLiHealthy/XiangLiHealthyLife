#1.create db
create database if not exists HealthyLife character set utf8;

#2.choose db
use HealthyLife;

#3.create tables
create table if not exists dignosis_element
(
    dignosis_element_id bigint auto_increment primary key,
    content varchar(256),
    flag varchar(32) 
    # 1 symptom 2 cause 3 diagnosis 4 solution 5 examination  6 complication      7 prevention 

)charset = utf8,engine = InnoDB
;

create table if not exists diagnosis
(
    diagnosis_id bigint auto_increment primary key,
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

create table if not exists iagnosis_element_map
(
	diagnosis_id bigint,
	diagnosis_element_id bigint
)engine = InnoDB, charset = utf8;

# medical knowledge
create table if not exists edical
(
    medical_id bigint auto_increment primary key,
    create_time datetime not null,
    flag int not null default 0 ,#1 chinese medicine ，2 western medicine
    src_url varchar(1024),
    name varchar(64)
    )engine = InnoDB, charset = utf8;

create table if not exists medical_dict
(
    medical_id bigint,
    dignosis_element_id bigint
)engine = InnoDB, charset = utf8;

#diseas case
create table  if not exists disease_case
(
 case_id bigint auto_increment primary key,
 user_id bigint,
 create_time datetime,
 endtime datetime,
 status int # 0 creating 1 finish -1 delete 
)engine = InnoDB, charset = utf8
;

create table  if not exists  case_detail
(
 case_detail_id bigint auto_increment primary key,
 case_id bigint,
 detail varchar(1024),
 type int
)engine = InnoDB, charset = utf8
;

create table  if not exists case_picture
(
 case_picture_id bigint auto_increment primary key,
 case_id bigint,
 type int,
 path varchar(260)

)engine = InnoDB, charset = utf8;

create table if not exists user
(
    user_id bigint auto_increment primary key,
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
    occupation varchar(128)
)engine = InnoDB, charset = utf8;

create table  if not exists medical_tmp
(
    knowledge_id bigint auto_increment primary key,
    disease varchar(64) ,
    src_url varchar(1024),
    station varchar(128),
    state int default 0,
    import_time datetime
)charset=utf8,engine = InnoDB;

create table  if not exists diagnosis_element_tmp
(
    element_id bigint auto_increment primary key,
    knowledge_id bigint,
    title varchar(128),
    content varchar(4096),
    cleaning_state int default 0
) charset = utf8, engine = InnoDB;

create table  if not exists station
(
    station varchar(128),
    url varchar(1024),
    state int default 0,
    add_time datetime
 )charset = utf8, engine = InnoDB;



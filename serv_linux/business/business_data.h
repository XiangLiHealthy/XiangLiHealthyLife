#pragma once 
#include<string>
#include<list>
#include<vector>
#include<memory.h>
using std::string;

using namespace 		std;
typedef unsigned char 	byte;
typedef long long 		    LONG;

#define MID_SIZE 		256
#define BIG_SIZE 		1024

struct db_diagnosis_t {
	long long 	diagnosis_id;
	long long 	diagnosis_element_id;
	int			feedback;
	string		comment;
	int			data_source;
	long long	data_source_id;
};

struct db_medical_t {
	long long 	medical_id;
	string		create_time;
	int			type;
};

struct db_case_t {
	long long  	case_id;
	long long  	user_id;
	string	   	create_time;
	string		end_time;
	int			status;
};

struct db_case_detail_t {
	long long	case_detail_id;
	long long	case_id;
	string		detail;
	int			type;
};

struct db_case_picture_t {
	long long	case_pictur_id;
	long long	case_id;
	int			type;
	string		path[260];
};


struct db_user_t {	
	long long	user_id;
	string tel;
	string		password;
	string		name;
	string		sex;//0 未知 1 男 2 女
	int			tall;
	int			weight;
	string		birthday;
	string		native_place;
	string		family;
	string			marital_status;
	string			blood_type;//0 未知 , 1 A, 2 B, 3 AB,i 4 O;
	string		occupation;

	void Clear()
	{
		user_id = -1;
		tel = "";
		password = "";
		name = "";
		sex = "";
		tall = -1;
		weight = -1;
		birthday = "";
		native_place = "";
		family = "";
		marital_status = -1;
		blood_type = -1;
		occupation = "";
	}
};

struct symptom_t {
	long long					diagnosis_element_id;
	string						content;
	int							type;
	int							feedback;
};
struct symptom_container_t {
	string						detail;
	vector<symptom_t> 			elements;

	public:
	void Clear() {
		detail = "";
		elements.clear();
	}
};

struct cause_t {
	LONG						diagnosis_element_id;
	string						content;
	int 						type;
	int							suspect;
	int							feedback;
};
struct cause_container_t {
	string						detail;
	vector<cause_t>				elements;

	public:
	void Clear() {
		detail = "";
		elements.clear(); 
	}
};

struct diagnosis_t {
	LONG						diagnosis_id;
	LONG						diagnosis_element_id;
	string						content;
	float						probability;
	int							degree;
};
struct diagnosis_container_t {
	std::string 				detail;
	std::vector<diagnosis_t> 	elements;

	public:
	void Clear() {
		detail = "";
		elements.clear();
	}
};

struct solution_t {
	LONG						diagnosis_element_id;
	string						content;
};
struct solution_container_t {
	string						detail;
	vector<solution_t>			elements;

	public:
	void Clear() {
		detail = "";
		elements.clear();
	}
};

struct treatment_t 
{
	long long 								user_id;
	std::string								method;
	symptom_container_t    symptom;
	cause_container_t 			cause;
	diagnosis_container_t	diagnosis;
	solution_container_t	  solution;
	int 											feedback;
	string 									 	data_source;
	std::string								create_time;
	std::string								end_time;
	int												status;

	public:
	void Clear() 
	{
		method = "";
		symptom.Clear();
		cause.Clear();
		diagnosis.Clear();
		solution.Clear();
		create_time	= "";
		end_time 	= "";
		status		= 0;
	}
};

struct account_t
{
	string account;
	string password;
	string way;
};

struct  clinics_record_request_t
{
	string user_id;
	string start_num;
};

struct clinics_record_response_t
{
	string disease;
	string cause;
	string start_date;
	int time_long;
	int feedback_count;
	int status;

	void clear()
	{
		status = 0;
		disease = "";
		cause = "";
		start_date = "";
		time_long = 0;
		feedback_count = 0;
	}
};



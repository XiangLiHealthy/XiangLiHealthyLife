#pragma once 
#include<string>
#include<list>
#include<vector>
#include<memory.h>

using namespace 		std;
typedef unsigned char 	byte;
typedef long long 		LONG;

#define MID_SIZE 		256
#define BIG_SIZE 		1024

#define CLEAR(class_name) \
	public:\
	void Clear() {\
		memset(this, 0, sizeof(class_name));\
	}

struct db_diagnosis_t {
	long long 	diagnosis_id;
	long long 	diagnosis_element_id;
	int			feedback;
	char		comment[256];
	int			data_source;
	long long	data_source_id;

	CLEAR(db_diagnosis_t);
};

struct db_medical_t {
	long long 	medical_id;
	char		create_time[64];
	int			type;

	CLEAR(db_medical_t);
};

struct db_case_t {
	long long  	case_id;
	long long  	user_id;
	char	   	create_time[64];
	char		end_time[64];
	int			status;

	CLEAR(db_case_t);
};

struct db_case_detail_t {
	long long	case_detail_id;
	long long	case_id;
	char		detail[1024];
	int			type;

	CLEAR(db_case_detail_t);
};

struct db_case_picture_t {
	long long	case_pictur_id;
	long long	case_id;
	int			type;
	char		path[260];

	CLEAR(db_case_picture_t);
};


struct db_user_t {
	long long	user_id;
	char		password[256];
	char		name[64];
	char		sex[8];//0 未知 1 男 2 女
	int			tall;
	int			weight;
	char		birthday[32];
	char		native_place[256];
	char		family[256];
	int			marital_status;
	int			blood_type;//0 未知 , 1 A, 2 B, 3 AB,i 4 O;
	char		occupation[128];

	CLEAR(db_user_t);
};

struct symptom_t {
	long long					diagnosis_element_id;
	char						content[256];
	int							type;
	int							feedback;

	CLEAR(symptom_t);
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
	char						content[MID_SIZE];
	int 						type;
	int							suspect;
	int							feedback;

	CLEAR(cause_t);
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
	char						content[MID_SIZE];
	float						probability;
	int							degree;

	CLEAR(diagnosis_t);
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
	char						content[MID_SIZE];

	CLEAR(solution_t);
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

struct treatment_t {
	std::string					protocol;
	symptom_container_t 		symptom;
	cause_container_t 			cause;
	diagnosis_container_t		diagnosis;
	solution_container_t		solution;
	std::string					create_time;
	std::string					end_time;
	int							status;

	public:
	void Clear() {
		protocol = "";
		symptom.Clear();
		cause.Clear();
		diagnosis.Clear();
		solution.Clear();
		create_time	= "";
		end_time 	= "";
		status		= 0;
	}
};

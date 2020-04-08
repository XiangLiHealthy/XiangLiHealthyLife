#pragma once 

#include"../include/json/json.h"
#include"business_data.h"

class Coder 
{
public:
	static int 			DecodeTreatment(const Json::Value& jData,treatment_t& treatment );
	static int 			EncodeTreatment(const treatment_t& treatment_src, Json::Value& jDst);

	static int          DecodeAccountLogin(const Json::Value& jData, account_t& account);
	static int 			DecodeAccountRegister(const Json::Value& jData, db_user_t& user_info);

	static int 			DecodeClinicsRecord(const Json::Value& jData, clinics_record_request_t& request);
	static int 			EncodeClinicsRecord(const list<clinics_record_response_t>& response_data, Json::Value& jData);
private:
	static int			Decode(const Json::Value jSrc, symptom_container_t& dst);
	static int			Decode(const Json::Value jSrc, cause_container_t& dst);
	static int			Decode(const Json::Value jSrc, diagnosis_container_t& dst);
	static int			Decode(const Json::Value jSrc, solution_container_t& dst);
 
	static Json::Value	Encode( symptom_container_t symptom);
	static Json::Value Encode(cause_container_t cause);
	static Json::Value	Encode( diagnosis_container_t diagnosis);
	static Json::Value Encode( solution_container_t solution);

};

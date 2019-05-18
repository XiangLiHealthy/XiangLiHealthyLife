#pragma once 

#include"../include/json/json.h"
#include"business_data.h"

class Coder {
	public:
	int 		DecodeTreatment(const Json::Value& jData,treatment_t& treatment );
	int 		EncodeTreatment(const treatment_t& treatment_src, Json::Value& jDst);

	std::string	GetLastError();
	private:
	std::string m_error;

	int			Decode(const Json::Value jSrc, symptom_container_t& dst);
	int			Decode(const Json::Value jSrc, cause_container_t& dst);
	int			Decode(const Json::Value jSrc, diagnosis_container_t& dst);
	int			Decode(const Json::Value jSrc, solution_container_t& dst);

	Json::Value	Encode( symptom_container_t symptom);
	Json::Value Encode(cause_container_t cause);
	Json::Value	Encode( diagnosis_container_t diagnosis);
	Json::Value Encode( solution_container_t solution);

};

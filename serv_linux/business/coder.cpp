#include"coder.h"
#include<memory.h>
#include<stdlib.h>
#include "../log/log.h"

int Coder::DecodeTreatment(const Json::Value& data, treatment_t& dst) {
	//1.解析method
	// if(jSrc["method"].isNull()) 
	// {
	// 	LOG_ERROR("clinic,没有method键值");
	// 	return -1;
	// }
	// dst.method = jSrc["method"].asString();

	//2.获得data
	// if(jSrc["data"].isNull()) {
	// 	LOG_ERROR("clinic, 没有data键值");
	// 	return -1;
	// }
	// Json::Value data = jSrc["data"];

	if (data["method"].isNull())
	{
		LOG_ERROR("there is not method");
		return -1;
	}
	dst.method = data["method"].asString();
	
	//3.获取symptom_container
	if(data["Symptom"].isNull()) 
	{
		LOG_ERROR("没有symptom键值");
		return -1;
	}
	Json::Value jDiagnosisElement = data["Symptom"];

	if(Decode(jDiagnosisElement, dst.symptom) < 0) 
	{
		LOG_ERROR("decode sysmptom failed");
		return -1;
	}

	//4.获得cause_container
	if(!data["Cause"].isNull()) 
	{
		jDiagnosisElement = data["Cause"];
		if(Decode(jDiagnosisElement, dst.cause) < 0) 
		{
			LOG_ERROR("decode cause failed");
			return -1;
		}
	}
	

	//5.获得diagnosis_container//不要容忍代码冗余,不然破窗户理论会很快把项目整烂
	if(!data["Diagnosis"].isNull()) 
	{
		jDiagnosisElement = data["Diagnosis"];
		if(Decode(jDiagnosisElement, dst.diagnosis) < 0) 
		{
			LOG_ERROR("parser diagnosis faild");
			return -1;
		}
	}
	

	//6.获得solution_container
	if(!data["Solution"].isNull()) 
	{
		jDiagnosisElement = data["Diagnosis"];
		if(Decode(jDiagnosisElement, dst.solution) < 0) 
		{
			LOG_ERROR("parser solution failed");
			return -1;
		}
	}


	//7.获得其它信息
	if(!data["create_time"].isNull()) 
	{
		dst.create_time = data["create_time"].asString();
		if(data["end_time"].isNull()) 
		{
			LOG_ERROR("没有end_time键值");
			return -1;
		}
		dst.end_time = data["end_time"].asString();
	}

	return 0;
}

int Coder::EncodeTreatment(const treatment_t& src, Json::Value& jDst) {
	//1.编码ptotocol
	jDst["ptotocol"] 	= src.method;

	//2.生成symptom_container json 
	jDst["Symptom"]		= Encode(src.symptom);

	//3.生成cause_container json 
	jDst["Cause"] 		= Encode(src.cause);

	//4.生成diagnosis_contaier json 
	jDst["Diagnosis"]	= Encode(src.diagnosis);

	//5.生成solution_container json 
	jDst["Solution"]	= Encode(src.solution);

	//6.设置其它诊断属性
	jDst["create_time"]	= src.create_time;
	jDst["end_time"]	= src.end_time;
	jDst["stauts"]		= src.status;

	return 0;
}

int Coder::Decode(const Json::Value jSrc, symptom_container_t& dst) {
	if(jSrc["detail"].isNull()) 
	{
		LOG_ERROR("symptom没有键值detail");
		return -1;
	}
	dst.detail = jSrc["detail"].asString();


	if(jSrc["row"].isNull()) 
	{
		LOG_ERROR("symptom没row");
		return -1;
	}
	Json::Value row = jSrc["row"];

	for(int i = 0; i < row.size(); i++) {
		Json::Value element = row[i];
		if(element["diagnosis_element_id"].isNull()) 
		{
			LOG_ERROR("clinic不存在diagnosis_element_id键值");
			return -1;
		}

		symptom_t  diagnosis;
		//memset(&diagnosis, 0, sizeof(&diagnosis));
		diagnosis.diagnosis_element_id = element["diagnosis_element_id"].asInt();

		dst.elements.push_back(diagnosis);
	}

	return 0;
}

int Coder::Decode(const Json::Value jSrc, cause_container_t& dst) {
	if(jSrc["detail"].isNull()) 
	{
		LOG_ERROR("cause没有键值detail");
		return -1;
	}
	dst.detail = jSrc["detail"].asString();


	if(jSrc["row"].isNull()) {
		LOG_ERROR("symptom没row");
		return -1;
	}

	Json::Value row = jSrc["row"];

	for(int i = 0; i < row.size(); i++) {
		Json::Value element = row[i];
		if(element["diagnosis_element_id"].isNull()) {
			LOG_ERROR("clinic不存在diagnosis_element_id键值");
			return -1;
		}

		cause_t  diagnosis;
		//memset(&diagnosis, 0, sizeof(&diagnosis));
		diagnosis.diagnosis_element_id = element["diagnosis_element_id"].asInt();

		dst.elements.push_back(diagnosis);
	}

	return 0;
}
int Coder::Decode(const Json::Value jSrc, diagnosis_container_t& dst) {
	if(jSrc["detail"].isNull()) {
		LOG_ERROR("symptom没有键值detail");
		return -1;
	}
	dst.detail = jSrc["detail"].asString();


	if(jSrc["row"].isNull()) {
		LOG_ERROR("symptom没row");
		return -1;
	}
	Json::Value row = jSrc["row"];

	for(int i = 0; i < row.size(); i++) {
		Json::Value element = row[i];
		if(element["diagnosis_element_id"].isNull()) {
			LOG_ERROR("clinic不存在diagnosis_element_id键值");
			return -1;
		}

		diagnosis_t  diagnosis;
		//memset(&diagnosis, 0, sizeof(&diagnosis));
		diagnosis.diagnosis_element_id = element["diagnosis_element_id"].asInt();

		dst.elements.push_back(diagnosis);
	}

	return 0;
}

int Coder::Decode(const Json::Value jSrc, solution_container_t& dst) {
	if(jSrc["detail"].isNull()) {
		LOG_ERROR("symptom没有键值detail");
		return -1;
	}
	dst.detail = jSrc["detail"].asString();


	if(jSrc["row"].isNull()) {
		LOG_ERROR("symptom没row");
		return -1;
	}
	Json::Value row = jSrc["row"];

	for(int i = 0; i < row.size(); i++) {
		Json::Value element = row[i];
		if(element["diagnosis_element_id"].isNull()) {
			LOG_ERROR("clinic不存在diagnosis_element_id键值");
			return -1;
		}

		solution_t  diagnosis;
		//memset(&diagnosis, 0, sizeof(&diagnosis));
		diagnosis.diagnosis_element_id = element["diagnosis_element_id"].asInt();

		dst.elements.push_back(diagnosis);
	}

	return 0;
}

Json::Value Coder::Encode(symptom_container_t symptom) {
	Json::Value jSymptom;
	jSymptom["detail"]			= symptom.detail;

	int i;
	Json::Value jSymptomElements;
	for(i = 0; i < symptom.elements.size(); i ++ ){
		//正常diagnosi_element_t的json对象
		Json::Value element;
		symptom_t element_t 	= symptom.elements[i];
		
		char szValue[32];
		memset(szValue, 0, sizeof(szValue));
		sprintf(szValue, "%lld", element_t.diagnosis_element_id);

		element["diagnosis_id"] = szValue;
		element["content"]		= element_t.content;
		element["type"]			= element_t.type;
		element["feedback"]		= element_t.feedback;

		//将对象添加为数组元素
		jSymptomElements.append(element);
	}

	//将数组对象添加到symptom]节点
	jSymptom["row"] 			= jSymptomElements;
	return jSymptom;
}

Json::Value Coder::Encode( cause_container_t cause) {
	Json::Value jSymptom;
	jSymptom["detail"]			= cause.detail;

	int i;
	Json::Value jSymptomElements;
	for(i = 0; i < cause.elements.size(); i ++ ){
		//正常diagnosi_element_t的json对象
		Json::Value element;
		cause_t element_t 	= cause.elements[i];
		
		char szValue[32];
		memset(szValue, 0, sizeof(szValue));
		sprintf(szValue, "%lld", element_t.diagnosis_element_id);

		element["diagnosis_id"] = szValue;
		element["content"]		= element_t.content;
		element["type"]			= element_t.type;
		element["feedback"]		= element_t.suspect;

		//将对象添加为数组元素
		jSymptomElements.append(element);
	}

	//将数组对象添加到symptom]节点
	jSymptom["row"] 			= jSymptomElements;
	return jSymptom;

}

Json::Value Coder::Encode( diagnosis_container_t diagnosis) {
	Json::Value jDiagnosis;
	jDiagnosis["detail"]			= diagnosis.detail;

	int i;
	Json::Value jDiagnosisElements;
	for(i = 0; i < diagnosis.elements.size(); i ++ ){
		//正常diagnosi_element_t的json对象
		Json::Value element;
		diagnosis_t element_t 	= diagnosis.elements[i];

		char szValue[32] = {0};
		sprintf(szValue, "%lld", element_t.diagnosis_element_id);

		element["diagnosis_id"] = szValue;
		element["content"]		= element_t.content;
		element["probability"]	= element_t.probability;
		element["degree"]		= element_t.degree;

		//将对象添加为数组元素
		jDiagnosisElements.append(element);
	}

	//将数组对象添加到symptom]节点
	jDiagnosis["row"] 			= jDiagnosisElements;
	return jDiagnosis;

}

Json::Value Coder::Encode(const solution_container_t solution) {
	Json::Value jSolution;
	jSolution["detail"]			= solution.detail;

	int i;
	Json::Value jSymptomElements;
	for(i = 0; i < solution.elements.size(); i ++ ){
		//正常diagnosi_element_t的json对象
		Json::Value element;
		solution_t element_t 	= solution.elements[i];
		
		char szValue[32] = {0};
		sprintf(szValue, "%lld", element_t.diagnosis_element_id);

		element["diagnosis_id"] = szValue;
		element["content"]		= element_t.content;

		//将对象添加为数组元素
		jSymptomElements.append(element);
	}

	//将数组对象添加到symptom]节点
	jSolution["row"] 			= jSymptomElements;
	return jSolution;

}

int Coder::DecodeAccountLogin(const Json::Value& jData, account_t& account)
{
	//get account
	if (jData["account"].isNull())
	{
		LOG_ERROR("there is not key:account");
		return -1;
	}

	account.account = jData["account"].asString();

	//get password
	if (jData["password"].isNull() < 0)
	{
		LOG_ERROR("there is not key password");
		return -1;
	}

	account.password = jData["password"].asString();

	//get wat
	if (jData["way"].isNull())
	{
		LOG_ERROR("there is not key way");
		return -1;
	}

	account.way = jData["way"].asString();

	return 0;
}

int Coder::DecodeAccountRegister(const Json::Value& jData, db_user_t& user_info)
{
	if (!jData["tel"].isNull())
	{
		user_info.tel = jData["tel"].asString();
	}

	if (!jData["password"].isNull())
	{
		user_info.password = jData["password"].asString();
	}

	if (!jData["name"].isNull())
	{
		user_info.name = jData["name"].asString();
	}

	if (!jData["sex"].isNull())
	{
		user_info.sex = jData["sex"].asString();
	}

	if (!jData["tall"].isNull())
	{
		user_info.tall = jData["tall"].asInt();
	}

	if (!jData["weight"].isNull())
	{
		user_info.weight = jData["weight"].asInt();
	}

	if (!jData["birthday"].isNull())
	{
		user_info.birthday = jData["birthday"].asString();
	}

	if (!jData["native_place"].isNull())
	{
		user_info.native_place = jData["native_place"].asString();
	}

	if (!jData["family"].isNull())
	{
		user_info.family = jData["family"].asString();
	}

	if (!jData["marital_status"].isNull())
	{
		user_info.marital_status = jData["marital_stattus"].asString();
	}

	if (!jData["blood_type"].isNull())
	{
		user_info.blood_type = jData["blood_type"].asString();
	}

	if (!jData["occupation"].isNull())
	{
		user_info.occupation = jData["occupation"].asString();
	}

	return 0;
}
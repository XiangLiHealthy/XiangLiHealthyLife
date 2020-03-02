#include"coder.h"
#include<memory.h>
#include<stdlib.h>
#include "../log/log.h"

int Coder::DecodeTreatment(const Json::Value& jSrc, treatment_t& dst) {
	//1.解析protocol
	if(jSrc["protocol"].isNull()) {
		m_error = "clinic,没有protocol键值";
		return -1;
	}
	dst.protocol = jSrc["protocol"].asString();

	//2.获得data
	if(jSrc["data"].isNull()) {
		m_error = "clinic, 没有data键值";
		return -1;
	}
	Json::Value data = jSrc["data"];
	
	//3.获取symptom_container
	if(data["Symptom"].isNull()) {
		m_error = "没有symptom键值";
		return -1;
	}
	Json::Value jDiagnosisElement = data["Symptom"];

	if(Decode(jDiagnosisElement, dst.symptom) < 0) {
		return -1;
	}

	//4.获得cause_container
	if(data["Cause"].isNull()) {
		m_error = "没有键值Cause";
		return -1;
	}
	jDiagnosisElement = data["Cause"];

	if(Decode(jDiagnosisElement, dst.cause) < 0) {
		return -1;
	}

	//5.获得diagnosis_container//不要容忍代码冗余,不然破窗户理论会很快把项目整烂
	if(data["Diagnosis"].isNull()) {
		m_error = "没有键值Diagnosis";
		return -1;
	}
	jDiagnosisElement = data["Diagnosis"];

	if(Decode(jDiagnosisElement, dst.diagnosis) < 0) {
		return -1;
	}

	//6.获得solution_container
	if(data["Solution"].isNull()) {
		m_error = "没有Solution键值";
		return -1;
	}
	jDiagnosisElement = data["Diagnosis"];

	if(Decode(jDiagnosisElement, dst.solution) < 0) {
		return -1;
	}

	//7.获得其它信息
	if(data["create_time"].isNull()) {
		m_error = "没有create_time键值";
		return -1;
	}
	dst.create_time = data["create_time"].asString();

	if(data["end_time"].isNull()) {
		m_error = "没有end_time键值";
		return -1;
	}
	dst.end_time = data["end_time"].asString();


	if(data["status"].isNull()) {
		m_error = "没有staus键值";
		return -1;
	}
	dst.status = data["status"].asInt();

	return 0;
}

int Coder::EncodeTreatment(const treatment_t& src, Json::Value& jDst) {
	//1.编码ptotocol
	jDst["ptotocol"] 	= src.protocol;

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
	if(jSrc["detail"].isNull()) {
		m_error = "symptom没有键值detail";
		return -1;
	}
	dst.detail = jSrc["detail"].asString();


	if(jSrc["row"].isNull()) {
		m_error = "symptom没row";
		return -1;
	}
	Json::Value row = jSrc["row"];

	for(int i = 0; i < row.size(); i++) {
		Json::Value element = row[i];
		if(element["diagnosis_element_id"].isNull()) {
			m_error = "clinic不存在diagnosis_element_id键值";
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
	if(jSrc["detail"].isNull()) {
		m_error = "symptom没有键值detail";
		return -1;
	}
	dst.detail = jSrc["detail"].asString();


	if(jSrc["row"].isNull()) {
		m_error = "symptom没row";
		return -1;
	}

	Json::Value row = jSrc["row"];

	for(int i = 0; i < row.size(); i++) {
		Json::Value element = row[i];
		if(element["diagnosis_element_id"].isNull()) {
			m_error = "clinic不存在diagnosis_element_id键值";
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
		m_error = "symptom没有键值detail";
		return -1;
	}
	dst.detail = jSrc["detail"].asString();


	if(jSrc["row"].isNull()) {
		m_error = "symptom没row";
		return -1;
	}
	Json::Value row = jSrc["row"];

	for(int i = 0; i < row.size(); i++) {
		Json::Value element = row[i];
		if(element["diagnosis_element_id"].isNull()) {
			m_error = "clinic不存在diagnosis_element_id键值";
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
		m_error = "symptom没有键值detail";
		return -1;
	}
	dst.detail = jSrc["detail"].asString();


	if(jSrc["row"].isNull()) {
		m_error = "symptom没row";
		return -1;
	}
	Json::Value row = jSrc["row"];

	for(int i = 0; i < row.size(); i++) {
		Json::Value element = row[i];
		if(element["diagnosis_element_id"].isNull()) {
			m_error = "clinic不存在diagnosis_element_id键值";
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
		sprintf(szValue, "%l", element_t.diagnosis_element_id);

		element["diagnosis_id"] = szValue;
		element["content"]		= element_t.content;

		//将对象添加为数组元素
		jSymptomElements.append(element);
	}

	//将数组对象添加到symptom]节点
	jSolution["row"] 			= jSymptomElements;
	return jSolution;

}

std::string Coder::GetLastError() {
	return m_error;
}

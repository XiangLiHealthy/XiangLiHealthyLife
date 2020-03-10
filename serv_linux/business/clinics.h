#pragma once 
#include"proto.h"
//#include"../DataBase/DataBase.h"
#include"business_data.h"

class Clinics : public Proto 
{
	public:
							Clinics();
							~Clinics();

	virtual const char* 	getName();
	virtual int 	dispatch(const Json::Value& jData, Handle fd);
private:
	Json::Value				GetSymptom(treatment_t symptom, DataBase* pDB);
	Json::Value				GetCause(treatment_t treatment, DataBase* pDB );
	Json::Value				GetDiagnosis(treatment_t treatment, DataBase* pDB);
	Json::Value				GetSolution(treatment_t treatment, DataBase* pDB);
	Json::Value				Finish(treatment_t treatment, DataBase* pDB);

	vector<string>			SplitDetail(const string detail);
	vector<LONG>			GetSymptomID(const vector<string> details, DataBase* pDB);
	vector<symptom_t>		GetSymptomContent(const vector<LONG> element_ids, DataBase* pDB);

	vector<cause_t>			GetCauseContent(treatment_t treatment,DataBase* pDB);
	vector<diagnosis_t>		GetDiagnosisContent(treatment_t treatment, DataBase* pDB);
	vector<solution_t>		GetSolutionContent(treatment_t treatment, DataBase* pDB);
public:
};

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
	Json::Value				GetSymptom(treatment_t symptom );
	Json::Value				GetCause(treatment_t treatment  );
	Json::Value				GetDiagnosis(treatment_t treatment );
	Json::Value				GetSolution(treatment_t treatment );
	Json::Value				Finish(treatment_t treatment );

	vector<string>			SplitDetail(const string detail);
	vector<LONG>			GetSymptomID(const vector<string> details );
	vector<symptom_t>		GetSymptomContent(const vector<LONG> element_ids );

	vector<cause_t>			GetCauseContent(treatment_t treatment );
	vector<diagnosis_t>		GetDiagnosisContent(treatment_t treatment );
	vector<solution_t>		GetSolutionContent(treatment_t treatment );
	int 										SendData(const Json::Value& jData, Handle fd);
public:
};

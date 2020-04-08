#ifndef CLINICS_RECORD_
#define CLINCS_RECORD_

#include "proto.h"
#include "business_data.h"

class  ClinicsRecord : public Proto
{
public:
    ClinicsRecord();
    int dispatch(const Json::Value& jData, Handle fd) override;

private:
    int getRecords(const clinics_record_request_t& request, list<clinics_record_response_t>& responses);
};

#endif
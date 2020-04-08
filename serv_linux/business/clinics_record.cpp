#include "clinics_record.h"
#include "business_data.h"
#include "coder.h"
#include "../log/log.h"
#include <sstream>
#include "../sequence/json_seq_strategy.h"
#include "../DataBase/db_connector_container.h"

using std::stringstream;

ClinicsRecord::ClinicsRecord()
{
    m_name = "ClinicsRecord";
}

int ClinicsRecord::dispatch(const Json::Value& jData, Handle fd)
{
    stringstream desc;
    string result = "failed";
    Json::Value  proto_data;

    do
    {
         //parse data
        clinics_record_request_t request;
        if (Coder::DecodeClinicsRecord(jData, request) != 0)
        {
            desc << "parse request failed";
            LOG_ERROR(desc.str().c_str());
            break;
        }

        //get data from mysql
        list<clinics_record_response_t> responses;
        if (getRecords(request, responses) < 0)
        {
            desc << "perform sql failed";
            LOG_ERROR(desc.str().c_str());
            break;
        }

        //encode data
        if (Coder::EncodeClinicsRecord(responses, proto_data) < 0)
        {
            LOG_ERROR("encode responses failed");
            break;
        }

        result = "success";
    } while (0);
    
    proto_data["result"] = result;
    proto_data["desc"] = desc.str();
    if (!jData["metod"].isNull())
    {
        proto_data["method"] = jData["method"].asString();
    }

   Json::Value jRet;
   jRet["protocol"] = m_name;
   jRet["data"] = proto_data;

    if (JsonSeqStrategy::SendSeq(fd, jRet) < 0)
    {
        LOG_ERROR("send response failed");
        return -1;
    }

    return 0;
}

int ClinicsRecord::getRecords(const clinics_record_request_t& request, list<clinics_record_response_t>& responses)
{
    //create sql
    stringstream sql;
    sql << "select *,diagnosis_detail as d, DATEDIFF(NOW(), create_time) as time_long, \
    (select count(*) from diagnosis where diagnosis_detail = d) as feedback from diagnosis"
    << " where user_id = " << request.user_id
    << " order by create_time desc limit 100;";

    //perfrom sql
    std::shared_ptr<DataBase>  ptrDB = DBConnectorContainer::GetInstance().GetDB();
    if (nullptr == ptrDB)
    {
        LOG_ERROR("get db connect failed");
        return -1;
    }  

    if (ptrDB->Exec(sql.str()) < 0)
    {
        LOG_ERROR("perform sql faild");
        return -1;
    }

    //get datas
    responses.clear();
    while (ptrDB->HasNext())
    {
        clinics_record_response_t record;

        ptrDB->GetFieldValue("symptom_detail", record.disease);
        ptrDB->GetFieldValue("cause_detail", record.cause);
        ptrDB->GetFieldValue("create_time", record.start_date);
        ptrDB->GetFieldValue("feedback", record.feedback_count);
        ptrDB->GetFieldValue("time_long", record.time_long);
        ptrDB->GetFieldValue("status", record.status);

        responses.push_back(record);
    }

    return 0;
}


#include "account_manager.h"
#include "business_data.h"
#include "coder.h"
#include "../log/log.h"
#include <sstream>
#include "../sequence/json_seq_strategy.h"
#include "../DataBase/db_connector_container.h"

static string SUCCESS = "success";
static string FAILED = "failed";

AccountManager::AccountManager()
{
    m_name = "AccountManager";
}

int AccountManager::dispatch(const Json::Value& jData, Handle fd)
{
    Json::Value jRet;
    Json::Value data;
    do
    {
        //get method
        string method;
        data = GetMethod(jData, method);
        if (method.empty())
        {
            break;
        }

        //perform task
        if (method == "Login")
        {
            data = Login(jData);
        }
        else if (method == "Logout")
        {
            data = Logout(jData);
        }
        else if (method == "RegisterAcc")
        {
            data = RegisterAcc(jData);
        }
        else if (method == "UnregisterAcc")
        {
            data = UnregisterAcc(jData);
        }
        else
        {
            string desc = "not surport method:";
            desc += method;
            data = CreateData("failed", desc);
        }
    } while (0);
    
    jRet["protocol"] = m_name;
    jRet["data"] = data;

    if (JsonSeqStrategy::SendSeq(fd, jRet) < 0)
    {
        LOG_ERROR("send msg failed");
        return -1;
    }

    return 0;
}

Json::Value AccountManager::Login(const Json::Value& jData)
{
    account_t account;
    std::stringstream desc;
    string result = "failed";
    string user_id;

    //parser json
    do
    {
        if (Coder::DecodeAccountLogin(jData, account) < 0)
        {
            LOG_ERROR("json parser failed");
            return -1;
        }

        //verigy parameter
        if (account.account.empty() || account.password.empty() || account.way.empty())
        {
            desc << "invalid parameter " << "acc:" << account.account
            << "pass:" << account.password << "way:" << account.way;
            LOG_ERROR(desc.str().c_str());
            break;
        }

        //if account don`t exist, register accout
        db_user_t user_info;
        if (GetAccountInfo(account, user_info) < 0)
        {
            desc << "get account info failed";
            LOG_ERROR(desc.str().c_str());
            break;
        }

        if (user_info.user_id < 0)
        {
            desc << "unregister";
            break;
        }

        //varidy password
        if (account.password  != user_info.password)
        {
            desc << "password is wrong";
            break;
        }

        //return user id
        stringstream tmp;
        tmp << user_info.user_id;
        tmp >> user_id;

        result = "success";

        //set login state is success
    } while (0);
    
    Json::Value jRet;
    jRet["result"] = result;
    jRet["desc"] = desc.str();
    jRet["user_id"] = user_id;
    return jRet;
}

Json::Value AccountManager::Logout(const Json::Value& jData)
{

}

Json::Value AccountManager::RegisterAcc(const Json::Value& jData)
{
    Json::Value jRet;
    stringstream desc;
    string result = FAILED;
    db_user_t user_info;

    do
    {
        if (Coder::DecodeAccountRegister(jData, user_info) < 0)
        {
            desc << ("parser json failed");
            LOG_ERROR(desc.str().c_str());
            break;
        }

        //check key parameter
        if (!VerigyPara(user_info))
        {
            desc << "parameter varify failed";
            break;
        }

        //check register state
        if (IsAccountExist(user_info))
        {
            desc << ("account is exists");
            break;
        }

        //insert a account
        if (CreateAccount(user_info) < 0)
        {
            desc << "create account failed";
            break;
        }

        result = SUCCESS;
    } while (0);
    
    jRet["result"] = result;
    jRet["desc"] = desc.str();
    jRet["user_id"] = to_string(user_info.user_id);

    return jRet;
}

Json::Value AccountManager::UnregisterAcc(const Json::Value& jData)
{

}

int AccountManager::GetAccountInfo(const account_t& log_info, db_user_t& user_info)
{
    user_info.Clear();

    //create sql
    string sql = "select * from user where tel = ' ";
    sql += log_info.account;
    sql += " '; ";

    shared_ptr<DataBase> ptrDB = DBConnectorContainer::GetInstance().GetDB();
    if (!ptrDB)
    {
        LOG_ERROR("get db obj failed");
        return -1;
    }

    //perform sql
    if (ptrDB->Exec(sql) < 0)
    {
        LOG_ERROR("perfor sql failed");
        return -1;
    }

    //get filed
    if (!ptrDB->HasNext())
    {
        LOG_INFO("there isn't account");
        return 0;
    }

    ptrDB->GetFieldValue("user_id", user_info.user_id);
    ptrDB->GetFieldValue("tel", user_info.tel);
    ptrDB->GetFieldValue("password", user_info.password);
    ptrDB->GetFieldValue("name", user_info.password);
    ptrDB->GetFieldValue("sex", user_info.sex);
    ptrDB->GetFieldValue("tall", user_info.tall);
    ptrDB->GetFieldValue("weight", user_info.weight);
    ptrDB->GetFieldValue("birthday", user_info.birthday);
    ptrDB->GetFieldValue("native_place", user_info.native_place);
    ptrDB->GetFieldValue("family", user_info.family);
    ptrDB->GetFieldValue("marital_status", user_info.marital_status);
    ptrDB->GetFieldValue("blood_type", user_info.blood_type);
    ptrDB->GetFieldValue("occupation", user_info.occupation);

    return 0;
}

    bool AccountManager::IsAccountExist(const db_user_t& user_info)
    {
        //create sql
        stringstream sql;
        sql << "select * from user where tel = ' " << user_info.tel << " ' limit 1;";

        //perform sql
        shared_ptr<DataBase> ptrDB = DBConnectorContainer::GetInstance().GetDB();
        if (!ptrDB)
        {
            LOG_INFO("db connect isn't enoghu");
            return true;
        }

        if (ptrDB->Exec(sql.str()) < 0)
        {
            LOG_ERROR("perform sql failed");
            return true;
        }

        //get result
        return ptrDB->HasNext() ? true : false;
    }

    bool AccountManager::VerigyPara(const db_user_t& user_info)
    {
        if (user_info.tel.empty())
        {
            LOG_DEBUG("tel is empty");
            return false;
        }

        if (user_info.password.empty())
        {
            LOG_DEBUG("password is empty");
            return false;
        }

        if (user_info.birthday.empty())
        {
            LOG_DEBUG("birthday is empty");
            return false;
        }

        if (user_info.sex.empty())
        {
            LOG_DEBUG("sex is empty");
            return false;
        }

        return true;
    }

    int AccountManager::CreateAccount(db_user_t& user_info)
    {
        stringstream sql;
        sql << "insert into user (tel, password, name ,sex ,tall , weight, birthday,\
         native_place, family, marital_status, blood_type, occupation) \
         values ( "
         << " ' " << user_info.tel << " ' ,"
         << " ' " << user_info.password << " ', "
         << " ' " << user_info.name << " ', "
         << " ' " << user_info.sex << " ', "
         << " ' " << user_info.tall << " ', "
         << " ' " << user_info.weight << " ', "
         << " ' " << user_info.birthday << " ', "
         << " ' " << user_info.native_place << " ', "
         << " ' " << user_info.family << " ', "
         << " ' " << user_info.marital_status << " ', "
         << " ' " << user_info.blood_type << "', "
         << " ' " << user_info.occupation << " ' "
         << ")";

         shared_ptr<DataBase> ptrDB = DBConnectorContainer::GetInstance().GetDB();
         if (!ptrDB)
         {
             LOG_ERROR("get db falied");
             return -1;
         }

         if (ptrDB->Exec(sql.str()) < 0)
         {
             LOG_ERROR("perform sql failed");
             return -1;
         }

         return 0;
    }


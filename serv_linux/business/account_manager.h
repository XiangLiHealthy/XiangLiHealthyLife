#ifndef ACCUNT_MANAGER_
#define ACCUNT_MANAGER_
#include "proto.h"
#include "business_data.h"
#include <memory>

class AccountManager : public Proto 
{
public:
    AccountManager();
    int dispatch(const Json::Value& jData, Handle fd) override;

private:
    int GetMethod(const Json::Value& jData, string& method);
    Json::Value Login(const Json::Value& jData);
    Json::Value Logout(const Json::Value& jData);
    Json::Value RegisterAcc(const Json::Value& jData);
    Json::Value UnregisterAcc(const Json::Value& jData);

    int GetAccountInfo(const account_t& log_info, db_user_t& user_info);
    Json::Value CreateData(const string& result, const string& desc);
    bool IsAccountExist(const db_user_t& user_info, shared_ptr<DataBase> ptrDB);
    bool VerigyPara(const db_user_t& user_info);
    int CreateAccount(db_user_t& user_info, shared_ptr<DataBase> ptrDB);
};

#endif
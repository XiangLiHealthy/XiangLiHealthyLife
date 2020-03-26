package com.example.model_lib;
import com.example.commondata.*;

class accont_manage {
    public int login(String strID, String strPassword, String strResult)
    {
        return  0;
    }

    public boolean isLogin()
    {
        return m_login_state;
    }

    int get_verifacationCode() {
        return 0;
    }

    boolean isVerifacationCorrect(int nCode) {
        return  true;
    }

    int setPassword(String strPassword) {
        return 0;
    }

    int setAccoutInfo(data_accout st_accout) {
        return  0;
    }
    public data_accout getAccountInfo() {
        data_accout account = new data_accout();
        return  account;
    }

    int setPersonInfo(data_person st_person) {
        return 0;
    }
    data_person getPersonInfo() {
        data_person person = new data_person();

        return  person;
    }


    data_person m_stperson;
    data_accout m_staccount;
    boolean m_login_state = false;
}

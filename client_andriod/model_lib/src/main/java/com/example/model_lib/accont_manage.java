package com.example.model_lib;

import com.example.commondata.*;
import com.example.net_lib.NetFacade;
import com.example.threadlib.*;
import com.example.commondata.*;
import com.example.commondata.AccountMethod.*;

public class accont_manage extends Notify {
    public void notify(Notify task)
    {
        AccountManagerTask request = (AccountManagerTask) (task);

        AccountRequest answer = request.getData();
        switch (answer.method)
        {
            case LOGIN:
            {
                m_login_state = answer.loginInfo.result.compareTo("success") == 0 ?
                        true : false;
                m_account.user_id = answer.loginInfo.user_id;
            }break;

            case LOGOUT:
            {

            }break;

            case REGISTER:
            {

            }break;

            case UNREGISTER:
            {

            }break;

            default:
            {

            }
        }

        m_notify.notify((this));
    }

    public void Request(AccountRequest request) throws Exception
    {
        TaskQueue.taskQueue.put(new AccountManagerTask(this, request));
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
        return  m_account;
    }

    int setPersonInfo(data_person st_person) {
        return 0;
    }
    data_person getPersonInfo() {
        data_person person = new data_person();

        return  person;
    }

    AccountMethod m_method;
    data_person m_stperson;
    data_accout m_account = new data_accout();
    boolean m_login_state = false;
}

class AccountManagerTask extends Task
{
    AccountRequest m_request;

    AccountManagerTask(Notify notify, AccountRequest request)
    {
        m_request = request;
        m_notify = notify;
    }

    //get data
    public AccountRequest getData()
    {
        return m_request;
    }

    //perform net send
    public void run()
    {
        try
        {
            NetFacade.getInstance().requestAccount(m_request);
        } catch (Exception e)
        {
            e.printStackTrace();
        }
    }
}

package com.example.model_lib;
import android.accounts.AccountManager;

import com.example.commondata.ClinicsRecord;
import com.example.net_lib.NetFacade;
import com.example.threadlib.*;
import java.util.ArrayList;

public class clinics_history extends Notify{
    ArrayList<ClinicsRecord> m_clinics = new ArrayList<ClinicsRecord>();

    public void notify(Notify task)
    {
        ClinicsRecordTask record = (ClinicsRecordTask) task;

        m_clinics.addAll(record.getData());

        m_notify.notify(this);
    }

    public void add(ClinicsRecord cli)
    {
        m_clinics.add(cli);
    }


    public ArrayList<ClinicsRecord> getData(boolean bMore) throws Exception
    {
        if (true == bMore)
        {
            //从网络获取更多数据
            getServerRecords();
        }

        return m_clinics;
    }

    private void getServerRecords() throws InterruptedException
    {
        //get user_id
        String user_id = model_facade.getinstance().getAccountManager().getAccountInfo().user_id;

        //get data
        TaskQueue.taskQueue.put(new ClinicsRecordTask(user_id));
    }

    public ArrayList<ClinicsRecord> getDatas()
    {
        return m_clinics;
    }
}


class ClinicsRecordTask extends Task
{
    public void run()
    {
        try
        {
            m_arr_record = NetFacade.getInstance().getClinicsRecord().getRecords(m_user_id);
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
    }

    ClinicsRecordTask(String user_id)
    {
        m_user_id = user_id;
    }

    ArrayList<ClinicsRecord> getData()
    {
        return m_arr_record;
    }

    private ArrayList<ClinicsRecord> m_arr_record;
    private String m_user_id;
}
package com.example.net_lib;
import android.app.DownloadManager;

import java.io.*;
import java.net.*;

import com.example.commondata.*;


public class NetFacade {

    public void main()
    {

    }

    //每个app只允许一个网络连接
    public static synchronized NetFacade getInstance() throws Exception {
        return net;
    }

    public NetFacade() {
        //m_net.connet();
    }

    //AI自诊
    public Treatment request(enum_item eItem, Treatment clinics, int nPageNum) throws Exception {
        return m_clinics.request(eItem, clinics, nPageNum);
    }

    public CauseContainer getCause(Treatment clinics, int nPageNum){

        return null;
    }

    public DiagnosisContainer getDiagnosis(Treatment clinics, int nPageNum) {

        return null;
    }

    public SolutionContainer getSolution(Treatment clinics, int nPageNum) {

        return null;
    }

    public int  saveTreatment(Treatment clinics) {

        return 0;
    }

    //账户管理
    public void requestAccount(AccountRequest request) throws Exception
    {
        m_account.request(request);
    }

    //自诊记录
    public NetClinicsRecords getClinicsRecord()
    {
        return m_clinics_record;
    }

    static              NetFacade       net         = new NetFacade();
    private             JsonCoder       coder       = new JsonCoder();
    private             NetClinics      m_clinics   = new NetClinics(coder);
    private             NetAccountManager m_account = new NetAccountManager();
    private             NetClinicsRecords m_clinics_record = new NetClinicsRecords();
}

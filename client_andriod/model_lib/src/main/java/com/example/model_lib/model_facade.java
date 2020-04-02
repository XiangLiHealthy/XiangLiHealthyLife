package com.example.model_lib;

import android.accounts.AccountManager;

import com.example.commondata.*;
import com.example.net_lib.*;
import com.example.threadlib.TaskQueue;
import com.example.threadlib.ThreadManager;

import java.util.ArrayList;

/********************************************************************
应该算是代理模式,不太像外观者模式;这个类主要提供唯一的访问接口,不做详细的业务实现

应该只提供最简单的接口，管理对象即可；否则会导致该类无限制膨胀

 **************************************************************/


public class model_facade {


     public static model_facade getinstance() {
        return m_model;
    }

    public int init() {
        theadManager.start();
        return 0;
    }

    public void stop() {
         theadManager.stop();
    }

    /********************************************************************
     账户管理接口
     **************************************************************/
    public accont_manage getAccountManager()
    {
        return m_accont;
    }

    /********************************************************************
     ai自诊断接口
     **************************************************************/
    public model_clinics getClinics() {
        return m_clinics;
    }

    public  SymptomContainer RequestDiagnosis(enum_item procedure, String user_id )
    {
        return m_clinics.RequestDiagnosis(procedure, user_id);
    }
    public  SymptomContainer GetSymptom() {
        return m_clinics.GetSymptoms();
    }

    //获取AI自诊的历史记录
    public clinics_history getClninicsRecord()
    {
        return m_clinicsHistory;
    }

    public CauseContainer getCause(boolean bMore) {
        return m_clinics.getCause(bMore);
    }
    public DiagnosisContainer getDiagnosis(boolean bMore) {
        return  m_clinics.getDiagnosis(bMore);
    }
    public SolutionContainer getSolution(boolean bMore) {
        return m_clinics.getSolution(bMore);
    }


    /********************************************************************
     成员变量
     **************************************************************/
    static model_facade m_model             = new model_facade();
    ThreadManager       theadManager        = new ThreadManager();
    NetFacade           m_net               = new NetFacade();
    accont_manage       m_accont            = new accont_manage();
    model_clinics       m_clinics           = new model_clinics();
    clinics_history     m_clinicsHistory    = new clinics_history();
};




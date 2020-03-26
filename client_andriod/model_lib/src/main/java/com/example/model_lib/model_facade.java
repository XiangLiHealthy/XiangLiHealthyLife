package com.example.model_lib;

import com.example.commondata.*;
import com.example.net_lib.*;
import com.example.threadlib.TaskQueue;
import com.example.threadlib.ThreadManager;

import java.util.ArrayList;

/********************************************************************
应该算是代理模式,不太像外观者模式;这个类主要提供唯一的访问接口,不做详细的业务实现



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
    //get login state
    public boolean isLogin()
    {
        return  m_accont.isLogin();
    }

    //登录
    public int login(String strID, String strPasswd, String strResult) {
        return 0;
    }

    //获取个人信息
   public data_person getpersoninfo() {

        return  m_accont.getPersonInfo();
   }
   //设置个人信息
    public  int setPersonInfo(data_person st_person) {
        return m_accont.setPersonInfo(st_person);
    }

    //获取账户信息
    public data_accout getAccountInfo() {
        return  m_accont.getAccountInfo();
    }
    //设置账户信息
    public int setAccountInfo(data_accout st_account) {
        return  m_accont.setAccoutInfo(st_account);
    }

    //获取注册校验码
    public int getVerifacationCode() {
        return  m_accont.get_verifacationCode();
    }

    //判断校验码是否正确
    public boolean isVerirafacationCorrect(int nCode) {
        return m_accont.isVerifacationCorrect(nCode);
    }

    //获取校验码
    public int getVerafacationCode() {
        return  m_accont.get_verifacationCode();
    }


    /********************************************************************
     ai自诊断接口
     **************************************************************/
    public model_clinics getClinics() {
        return m_clinics;
    }

    public  SymptomContainer RequestDiagnosis(enum_item procedure ) {
        return m_clinics.RequestDiagnosis(procedure);
    }
    public  SymptomContainer GetSymptom() {
        return m_clinics.GetSymptoms();
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
    public ArrayList<model_clinics> getClinicsHistory() {
        return m_clinicsHistory.getData(true);
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




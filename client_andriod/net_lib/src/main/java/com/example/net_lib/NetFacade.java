package com.example.net_lib;
import java.io.*;
import java.net.*;

import com.example.commondata.*;


public class NetFacade {
    //每个app只允许一个网络连接
    public static synchronized NetFacade getInstance() throws Exception {
        if(!net.m_bConnet) {
            net.m_bConnet = net.m_net.connet();
        }

        //连接失败
        if(!net.m_bConnet) {
            throw new Exception("网络连接失败!");
        }

        return net;
    }

    public NetFacade() {
        //m_net.connet();
    }

    public Treatment request(enum_item eItem, Treatment clinics, int nPageNum)
    {
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

    static              NetFacade       net         = new NetFacade();
                        boolean         m_bConnet   = false;
    private             JsonCoder       coder       = new JsonCoder();
    private             Net             m_net       = new Net();

    private             NetClinics      m_clinics   = new NetClinics(coder, m_net);

}

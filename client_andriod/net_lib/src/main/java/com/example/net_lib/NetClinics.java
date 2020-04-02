package com.example.net_lib;

import com.example.commondata.*;
import org.json.JSONArray;
import org.json.JSONObject;
import com.example.net_lib.Net;


public class NetClinics {
    private JsonCoder m_jsonCoder;

    public NetClinics(JsonCoder json) {
        m_jsonCoder = json;
    }

    public Treatment request(enum_item eItem, Treatment clinics, int nPageNum) throws Exception {
        //将参数封装成json

        Net net = Net.getInstance();

        JSONObject jData = m_jsonCoder.encodeClinicsRequest(eItem, clinics, nPageNum);
        jData.put("user_id", clinics.m_user_id);
        jData = m_jsonCoder.setProtocol(jData, "Clinics");

        net.sendJson(jData);

        //等待结果返回
        jData = net.recvJson();

        //解析json数据
        return m_jsonCoder.decode(jData);
    }

    public CauseContainer getCause(Treatment clinics, int nPageNum){
        try {

        }
        catch (Exception e) {

        }

        return  null;
    }

    public DiagnosisContainer getDiagnosis(Treatment clinics, int nPageNum) {
        try
        {

        }
        catch (Exception e) {

        }

        return  null;
    }

    public SolutionContainer getSolution(Treatment clinics, int nPageNum) {
        try {

        }
        catch (Exception e) {

        }

        return  null;
    }

    public int  saveTreatment(Treatment clinics) {
        try {

        }
        catch (Exception e) {

        }

        return  -1;
    }
}

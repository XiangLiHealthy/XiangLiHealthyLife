package com.example.net_lib;

import com.example.commondata.*;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.Writer;

public class NetClinics {
    private JsonCoder m_jsonCoder;
    private Net m_net;

    public NetClinics(JsonCoder json, Net net) {
        m_jsonCoder = json;
        m_net = net;
    }

    public Treatment request(enum_item eItem, Treatment clinics, int nPageNum) {
        //将参数封装成json
        try {

            JSONObject jData = m_jsonCoder.encodeSymptom(eItem, clinics, nPageNum);

            //将长度+功能描述+data封装到字节流
            if (0 != m_net.send(jData))
            {
                return  null;
            }

            //等待结果返回
            jData = m_net.receiv();

            //解析json数据
            return m_jsonCoder.decode(jData);
        }
        catch (Exception e) {

        }

        return  null;
    }

    public CauseContainer getCause(Treatment clinics, int nPageNum){
        try {

        }
        catch (Exception e) {

        }

        return  null;
    }

    public DiagnosisContainer getDiagnosis(Treatment clinics, int nPageNum) {
        try {

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

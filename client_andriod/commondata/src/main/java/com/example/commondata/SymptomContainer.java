package com.example.commondata;

import java.util.ArrayList;

public class SymptomContainer {
    private   String m_strDetail = new String("");
    private   ArrayList<Symptom> m_lstItem = new ArrayList<Symptom>();

    public  SymptomContainer() {
        //测试数据
//        for(int i = 0; i < 13;i++)
//            m_lstItem.add(new Symptom());
    }

    public  void clear() {
        if (null == m_lstItem)
        {
            return;
        }

        m_strDetail = "";
        m_lstItem.clear();
    }

    public ArrayList<Symptom> getSelData() {

        return m_lstItem;
    }
    public void setDetail(String strDetail) {
        m_strDetail = strDetail;
    }
    public String getDetail() {
        return  m_strDetail;
    }
    public String getMain() {
        for (Symptom s: m_lstItem ){
            if (1 == s.nType) {
                return s.strDesc;
            }
        }

        return "无法描述";
    }

    public void add(ArrayList<Symptom> symptoms) {
        m_lstItem.addAll(symptoms);
    }

    public SymptomContainer getSymptoms() {
        return  this;
    }

    public ArrayList<Symptom> getData() {
        return m_lstItem;
    }

    int getDataNet() {

        //描述不能为空
        if (m_strDetail.isEmpty()) {
            System.out.println("详细描述为空");
            return 0;
        }

        System.out.println("数据不够,从网络中获取数据");

        System.out.println("网络出错");

        return 0;
    }
}

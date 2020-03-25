package com.example.commondata;

import java.util.ArrayList;

public class DiagnosisContainer {
    String m_strDetail = new String("");
    ArrayList<Diagnosis> m_lstItem = new ArrayList<Diagnosis>();
    CauseContainer m_causes;

    public DiagnosisContainer () {
        for (int i = 0; i < 9; i++) {
            m_lstItem.add(new Diagnosis());
        }
    }


    public void setDetail(String strDetail) {
        m_strDetail = strDetail;
    }
    public String getDetail() {
        return  m_strDetail;
    }

    public void setCauses(CauseContainer causes) {
        m_causes = causes;
    }

    public DiagnosisContainer getContainer(boolean bMore) {
        if (bMore) {
            //从网络中获取数据

            //将数据追加到列表
        }

        return  this;
    }

    public ArrayList<Diagnosis> getData() {
        return  m_lstItem;
    }
    public void add(ArrayList<Diagnosis> diagnoses) {m_lstItem.addAll(diagnoses);}
}

package com.example.commondata;

import java.util.ArrayList;
import java.util.List;

public class CauseContainer {
    public String m_strDetail = new String("");
    public ArrayList<Cause> m_lstItem = new ArrayList<Cause>();
    //private SymptomContainer m_symptoms;

    public CauseContainer() {
        for (int i = 0; i < 9; i++) {
            m_lstItem.add(new Cause());
        }
    }
    public void setDetail(String strDetail) {
        m_strDetail = strDetail;
    }
    public String getDetail() {
        return  m_strDetail;
    }

    public CauseContainer getCauses(boolean bMore ) {
        return  this;
    }

    public ArrayList<Cause> getData() {
        return  m_lstItem;
    }
    public void add(ArrayList<Cause> data) {
        m_lstItem.addAll(data);
    }

}

package com.example.commondata;

import java.util.ArrayList;

public class SolutionContainer {
    public String m_strDetail = new String("");
    public ArrayList<Solution> m_lstItem = new ArrayList<Solution>();

    private SymptomContainer m_symptoms;
    private CauseContainer m_causes;
    private DiagnosisContainer m_diagnosises;

    public  SolutionContainer() {
//        for(int i = 0; i < 9; i++) {
//            m_lstItem.add(new Solution());
//        }
    }

    public void setDetail(String strDetail) {
        m_strDetail = strDetail;
    }
    public String getDetail() {
        return  m_strDetail;
    }

    public void setSymptons(SymptomContainer symptoms) {
        m_symptoms = symptoms;
    }

    public  void setCauses(CauseContainer causes) {
        m_causes = causes;
    }

    public void setDiagnosis(DiagnosisContainer diagnosis) {
        m_diagnosises = diagnosis;
    }

    public SolutionContainer getSolution(boolean bMore) {
        if (bMore) {
            //从网络获取数据

            //将数据追加到列表
        }

        return  this;
    }

    public ArrayList<Solution> getData() {
        return m_lstItem;
    }
}

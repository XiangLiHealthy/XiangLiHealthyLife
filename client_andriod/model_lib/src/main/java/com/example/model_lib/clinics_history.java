package com.example.model_lib;
import java.util.ArrayList;

public class clinics_history {
    ArrayList<model_clinics> m_clinics = new ArrayList<model_clinics>();
    public void add(model_clinics cli) {
        m_clinics.add(cli);
    }


    public ArrayList<model_clinics> getData(boolean bMore) {
        if (true == bMore) {
            //从网络获取更多数据
        }

        return m_clinics;
    }
}

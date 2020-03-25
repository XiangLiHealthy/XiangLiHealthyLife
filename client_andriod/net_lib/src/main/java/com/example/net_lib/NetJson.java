package com.example.net_lib;
import org.json.*;
import com.example.net_lib.NetSequence.*;

import java.time.chrono.JapaneseDate;

public class NetJson {
    private NetSequence m_seq;

    public NetJson(NetSequence seq_instance)
    {
        m_seq = seq_instance;
    }

    public void SengMsg(JSONObject jData) throws Exception {
        byte[] byData = jData.toString().getBytes();
        m_seq.sendDataOnce(byData, DATA_TYPE.DATA_JONS);
    }

    public JSONObject rcvMsg() throws Exception {
        byte[] byData = null;


        byData = m_seq.recvData();
        DATA_TYPE type = m_seq.getType();
        if (DATA_TYPE.DATA_JONS != type) {
            throw  new Exception("data type not json");
        }

        String strData = new String(byData);
        JSONObject jData = new JSONObject(strData);
        return  jData;
    }
}

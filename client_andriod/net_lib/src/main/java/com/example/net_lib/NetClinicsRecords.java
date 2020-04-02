package com.example.net_lib;

import com.example.commondata.ClinicsRecord;

import org.json.JSONArray;
import org.json.JSONObject;

import java.lang.reflect.Array;
import java.util.ArrayList;

public class NetClinicsRecords {
    public ArrayList<ClinicsRecord> getRecords(String user_id) throws Exception
    {
        //encode request
        JSONObject data = encode(user_id);

        //send data
        Net.getInstance().sendJson(data);

        //decode result
        data = Net.getInstance().receiv();

        return decode(data);
    }

    private JSONObject encode(String user_id) throws Exception
    {
        JSONObject method = new JSONObject();
        method.put("method", PROTOCOL);
        method.put("user_id", user_id);

        JSONObject protocol = new JSONObject();
        protocol.put("protocol", "ClinicsRecord");
        protocol.put("data", method);

        return  protocol;
    }

    private ArrayList<ClinicsRecord> decode(JSONObject data) throws Exception
    {
        if (data.getString("protocol") != PROTOCOL)
        {
            throw new Exception("protocol is wrong");
        }

        JSONObject protocol_data = data.getJSONObject("data");
        JSONArray items = protocol_data.getJSONArray("datas");

        ArrayList<ClinicsRecord> arrayItems = new ArrayList<ClinicsRecord>();
        for (int i = 0; i < items.length(); i ++)
        {
            JSONObject item = items.getJSONObject(i);

            ClinicsRecord record = new ClinicsRecord();

            record.disease = item.getString("disease");
            record.cause = item.getString("cause");
            record.start_date = item.getString("start_date");
            record.feedback_count = item.getString("feadback_count");
            record.time_long = item.getString("time_long");
            arrayItems.add(record);
        }

        return arrayItems;
    }

    private String PROTOCOL = "ClinicsRecord";
}

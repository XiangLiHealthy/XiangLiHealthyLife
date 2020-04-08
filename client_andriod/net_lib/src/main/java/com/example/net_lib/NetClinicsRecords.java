package com.example.net_lib;

import com.example.commondata.ClinicsRecord;

import org.json.JSONArray;
import org.json.JSONObject;

import java.lang.reflect.Array;
import java.util.ArrayList;

import static java.lang.Thread.sleep;

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
        method.put("start_num", "0");

        JSONObject protocol = new JSONObject();
        protocol.put("protocol", "ClinicsRecord");
        protocol.put("data", method);

        return  protocol;
    }

    private ArrayList<ClinicsRecord> decode(JSONObject data) throws Exception
    {
        String protocol_name = data.getString("protocol");
        if (protocol_name.compareTo(PROTOCOL) != 0)
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
            record.cause.add(item.getString("cause"));
            record.start_date = item.getString("start_date");

            try
            {
                int time_long = item.getInt("time_long");
                int feedback = item.getInt("feedback_count");

                record.feedback_count = String.format("%d", feedback);
                record.time_long = String.format("%d",  time_long);
            }catch (Exception e)
            {
                e.printStackTrace();
            }


            arrayItems.add(record);
        }

        return arrayItems;
    }

    private String PROTOCOL = "ClinicsRecord";
}

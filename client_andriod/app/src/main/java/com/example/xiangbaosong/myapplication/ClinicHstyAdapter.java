package com.example.xiangbaosong.myapplication;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;


import com.example.commondata.Cause;
import com.example.model_lib.model_clinics;

import java.util.ArrayList;

public class ClinicHstyAdapter  extends BaseAdapter {
    private Context context;
    private ArrayList<model_clinics> m_lstData;

    ClinicHstyAdapter(Context ctxt, ArrayList<model_clinics> lstData) {
        m_lstData = lstData;
        context = ctxt;
    }


    public void add(ArrayList<model_clinics> lstData) {
        m_lstData = lstData;
    }

    @Override
    public int getCount() {
        return m_lstData.size();
    }

    @Override
    public Object getItem(int i) {
        return m_lstData.get(i);
    }

    @Override
    public long getItemId(int i) {
        return i;
    }

    @Override
    public View getView(int i, View convertView, ViewGroup viewGroup) {

        //加载自定义布局
        LayoutInflater mInflater = LayoutInflater.from(context);
        convertView = mInflater.inflate(R.layout.clinics_history_item, null);

        //对控件赋值
        final model_clinics data = m_lstData.get(i);
        String strText;

        //主要描述
        TextView text = (TextView) convertView.findViewById(R.id.clinics_h_i_desc_v);
        strText = data.GetSymptoms().getMain();
        text.setText((strText)  );
        strText = "";

       //原因
        text = convertView.findViewById(R.id.clinics_h_i_cause_v);

        ArrayList<Cause> causes= data.getCause(false).getData();
        for (Cause cause: causes) {
            strText += cause.strDesc + ",";

        }
        strText = strText.substring(0, strText.length() - 1);

        text.setText(strText );
        strText = "";

        //发作时间 病程
        text = convertView.findViewById(R.id.clinics_h_i_time_v);
        strText = data.getDatetime();
        text.setText(strText);

        text = convertView.findViewById(R.id.clinics_h_i_time_l);
        strText = "病程" + data.getDays() + "天";
        text.setText(strText);
        strText = "";

        //发作次数
        // text = convertView.findViewById(R.id.clinics_h_i_times_v);
        //strText = data.getDiagnosis(false).
        //text.setText((String) data.GetSymptoms(false).getMain() );



        return convertView;
    }
}

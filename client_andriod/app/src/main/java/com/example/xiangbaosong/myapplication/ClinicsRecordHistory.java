package com.example.xiangbaosong.myapplication;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ListView;

import java.util.ArrayList;

import com.example.commondata.ClinicsRecord;
import com.example.model_lib.*;
import com.example.threadlib.Notify;

public class ClinicsRecordHistory extends AppCompatActivity
{
    private ClinicHstyAdapter adapter;
    private ListView mListView;// 数据展示列表
    private ArrayList<ClinicsRecord> m_lstData = new ArrayList<ClinicsRecord>();
    private model_facade model = model_facade.getinstance();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_clinics_record_history);

        //get records
        try {
            ArrayList<ClinicsRecord> records = model_facade.getinstance().getClninicsRecord().getData(true);

            mListView = (ListView) findViewById(R.id.clinics_history_lst);
            adapter = new ClinicHstyAdapter(this, records);
            mListView.setAdapter(adapter);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void button_click(View v) {
        //1.获取到按钮id
        int nID = v.getId();

        Intent intent = null;

        if (nID != R.id.button_clinicing && nID != R.id.button_recovery) {
            intent = new Intent(this, ClinicsRecordHistoryFeedback.class);
        }

        //弹出子页面,相当于mfc中的子窗口弹出
        if (intent != null) {
            startActivity(intent);
        }
    }

    class Msg extends Notify
    {
        public void notify(Notify task)
        {
            //get data
            clinics_history history = (clinics_history) task;

            runOnUiThread(new Runnable()
            {
                @Override
                public void run()
                {
                    //update data
                    adapter.add(history.getDatas());

                    adapter.notifyDataSetChanged();
                }
            });
        }
    }
}

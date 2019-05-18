package com.example.xiangbaosong.myapplication;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ListView;

import java.util.ArrayList;
import com.example.model_lib.*;

public class ClinicsRecordHistory extends AppCompatActivity {
    private ClinicHstyAdapter adapter;
    private ListView mListView;// 数据展示列表
    private ArrayList<model_clinics> m_lstData = new ArrayList<model_clinics>();
    private model_facade model = model_facade.getinstance();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_clinics_record_history);

        mListView = (ListView)findViewById(R.id.clinics_history_lst);
        adapter = new ClinicHstyAdapter(this, model.getClinicsHistory());
        mListView.setAdapter(adapter);
    }

    public void button_click(View v){

        //1.获取到按钮id
        int nID = v.getId();

        Intent intent = null;

        if(nID != R.id.button_clinicing && nID != R.id.button_recovery){
            intent = new Intent(this,ClinicsRecordHistoryFeedback.class);
        }

        //弹出子页面,相当于mfc中的子窗口弹出
        if(intent != null){
            startActivity(intent);
        }
    }
}

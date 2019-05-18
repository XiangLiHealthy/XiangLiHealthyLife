package com.example.xiangbaosong.myapplication;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

public class Psychonogy extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_psychonogy);
    }

    public void button_click(View v){

        //1.获取到按钮id
        int nID = v.getId();

        Intent intent = null;

        switch (nID)
        {
            //自诊历史
            case R.id.button_dairy:
            {
                intent = new Intent(this,PsychonogyDairy.class);
            }break;

            //添加记录
            case R.id.button_shedule:
            {
                intent = new Intent(this,PsychonogySchedule.class);
            }break;

            case R.id.button_interpersonal:
            {
                intent = new Intent(this,PsychonogyInterpersonal.class);
            }break;
        }
        //弹出子页面,相当于mfc中的子窗口弹出
        if(intent != null){
            startActivity(intent);
        }
    }
}

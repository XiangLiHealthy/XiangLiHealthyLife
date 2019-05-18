package com.example.xiangbaosong.myapplication;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
//import com.example.xiangbaosong.myapplication.PrevenRecordHistory;

public class Prevent extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_prevent);
    }

    public void button_click(View v){

        //1.获取到按钮id
        int nID = v.getId();

        Intent intent = null;

        switch (nID)
        {
            //自诊历史
            case R.id.button_prevent_task:
            {
                intent = new Intent(this,PrevenRecordHistory.class);
            }break;

        }
        //弹出子页面,相当于mfc中的子窗口弹出
        if(intent != null){
            startActivity(intent);
        }
    }
}

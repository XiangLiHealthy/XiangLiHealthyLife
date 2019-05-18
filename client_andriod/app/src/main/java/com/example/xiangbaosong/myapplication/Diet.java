package com.example.xiangbaosong.myapplication;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

public class Diet extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_diet);
    }

    public void button_click(View v){

        //1.获取到按钮id
        int nID = v.getId();

        Intent intent = null;

        switch (nID)
        {
            //饮食记录
            case R.id.button_diet_history:
            {
                intent = new Intent(this,DietRecordHistory.class);
            }break;

            //营养测算
            case R.id.button_diet_nutrition:
            {
                intent = new Intent(this,DietNutritionCaculate.class);
            }break;

            //饮食计划
            case R.id.button_diet_plan:
            {
                intent = new Intent(this,DietPlan.class);
            }break;
        }
        //弹出子页面,相当于mfc中的子窗口弹出
        if(intent != null){
            startActivity(intent);
        }
    }
}

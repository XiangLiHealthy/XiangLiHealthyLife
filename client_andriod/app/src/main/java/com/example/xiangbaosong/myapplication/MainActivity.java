package com.example.xiangbaosong.myapplication;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.StrictMode;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

//import com.example.model_lib.ThreadManager;
import com.example.model_lib.model_facade;
import com.example.xiangbaosong.myapplication.R.id;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");

    }
    private static final int REQUEST_PERMISSION = 0;
    //ThreadManager m_threadManager = new ThreadManager();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        if (android.os.Build.VERSION.SDK_INT > 9) {
            StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
            StrictMode.setThreadPolicy(policy);
        }


        // Example of a call to a native method
        //TextView tv = (TextView) findViewById(R.id.sample_text);
        //tv.setText(stringFromJNI());
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            int hasWritePermission = checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE);
            int hasReadPermission = checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE);
            List<String> permissions = new ArrayList<String>();

            if (hasWritePermission != PackageManager.PERMISSION_GRANTED) {
                permissions.add(Manifest.permission.WRITE_EXTERNAL_STORAGE);
            } else {
                //              preferencesUtility.setString("storage", "true");
                }
            if (hasReadPermission != PackageManager.PERMISSION_GRANTED) {
                permissions.add(Manifest.permission.READ_EXTERNAL_STORAGE); }
                else {
                //              preferencesUtility.setString("storage", "true");
                     }
            if (!permissions.isEmpty()) {
                //              requestPermissions(permissions.toArray(new String[permissions.size()]), REQUEST_CODE_SOME_FEATURES_PERMISSIONS);
                ActivityCompat.requestPermissions(this, new String[] {Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_PHONE_STATE}, REQUEST_PERMISSION);
            }
        }

        //启动线程池
        //m_threadManager.start();
        model_facade.getinstance().init();
    }

    //@Override 响应点击事件
    public void btn_click(View v)
    {
        //1.获取到按钮id
        int nID = v.getId();

        Intent intent = null;

        switch (nID)
        {
            //健康状态
            case R.id.button_state:
            {
                intent = new Intent(this,HealthyState.class);
            }break;

            //预防
            case id.button_prevent:
            {
                intent = new Intent(this,Prevent.class);
            }break;

            //自诊断
            case id.button_clinics:
            {
                intent = new Intent(this,Clinics.class);
            }break;

            //饮食
            case id.button_diet:
            {
                intent = new Intent(this,Diet.class);

            }break;

            //运动
            case id.button_sports:
            {
                intent = new Intent(this,Sports.class);

            }break;

            //心理
            case id.button_psychology:
            {
                intent = new Intent(this,Psychonogy.class);

            }break;

            //睡眠
            case id.button_sleep:
            {
                intent = new Intent(this,Sleep.class);

            }break;

            //环境
            case id.button_environment:
            {

               // intent = new Intent(this,Environment.class);
            }break;

            //关爱家人
            case id.button_fimaly:
            {
                intent = new Intent(this,Family.class);

            }break;

            //个人中心
            case id.button_personal:
            {

                intent = new Intent(this,PersonalCenter.class);
            }break;

            //学习社区
            case id.button_study:
            {

                intent = new Intent(this,Study.class);
            }break;
        }
        //弹出子页面,相当于mfc中的子窗口弹出
        //Intent intent=new Intent(
        // MainActivity.this,  弗类
        // AIActivity.class);  子类


        if(intent != null){
            startActivity(intent);
        }
    }
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}

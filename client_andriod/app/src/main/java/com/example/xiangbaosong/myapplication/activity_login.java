package com.example.xiangbaosong.myapplication;

import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import com.example.model_lib.*;
import com.example.commondata.*;

public class activity_login extends AppCompatActivity {
    private TextView m_register_bt;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        m_register_bt = (TextView)findViewById(R.id.register_tv);
        m_register_bt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                register();
            }
        });
    }

    //@Override 响应点击事件
    public void btn_click(View v)
    {
        int nID = v.getId();
        switch (nID)
        {
            case R.id.login_bt:
            {
                login();
            }break;

            case R.id.register_tv:
            {
                register();
            }break;

            case R.id.help_tv:
            {

            }break;

            case R.id.accept_cb:
            {
                onButtonAccept();
            }break;
        }
    }

    private void login()
    {
        String account = new String();
        String password = new String();
        String error = new String();
        //校验参数

        //login -1 account is wrong, -2 password is wrong
        model_facade model = model_facade.getinstance();
        int nRet = model.login(account, password, error);
        if (0 == nRet)
        {
            super.onBackPressed();
        }


    }

    private void register()
    {
        Intent intent = new Intent(this, activity_register.class);
        if (null == intent)
        {
            return;
        }

        startActivity(intent);

    }

    private void onButtonAccept()
    {

    }

    //第三种
    @Override
    public void onBackPressed() {
        //super.onDestroy();
        //android.os.Process.killProcess(android.os.Process.myPid());
        //System.exit(0);
        //TODO something
        super.onBackPressed();
    }
}

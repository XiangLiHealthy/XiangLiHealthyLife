package com.example.xiangbaosong.myapplication;

import android.accounts.AccountManager;
import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.TextView;
import com.example.model_lib.*;
import com.example.commondata.*;
import com.example.net_lib.NetFacade;
import com.example.threadlib.*;

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

        m_register_bt = (Button)findViewById(R.id.accept_cb);
        m_register_bt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onButtonAccept();
            }
        });

        CheckBox accept_ctl = (CheckBox) findViewById(R.id.accept_cb);
        accept_ctl.setChecked(true);

        EditText account_ctl = (EditText)findViewById(R.id.account) ;
        account_ctl.setText("13883372441");

        EditText password_ctl = (EditText)findViewById(R.id.password);
       password_ctl.setText("666666");
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
        LoginInfo info = new LoginInfo();
        String error = new String();
        //get content data
        EditText account_ctl = (EditText)findViewById(R.id.account) ;
        info.accout = account_ctl.getText().toString();

        EditText password_ctl = (EditText)findViewById(R.id.password);
        info.password = password_ctl.getText().toString();


        //校验参数
        TextView wrong_ctl = (TextView)findViewById(R.id.login_wrong_tv);
        if (info.accout.isEmpty())
        {
            wrong_ctl.setText("账号不允许为空");
            return;
        }

        if (info.password.isEmpty())
        {
            wrong_ctl.setText("密码不允许为空");
            return;
        }

        //login -1 account is wrong, -2 password is wrong
        model_facade model = model_facade.getinstance();
        int nRet = 0;
        try {
            model.getAccountManager().setNotify(m_msg);

            AccountRequest request = new AccountRequest();
            request.method = AccountMethod.LOGIN;
            request.loginInfo = info;
            model.getAccountManager().Request(request);
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (0 == nRet)
        {
            super.onBackPressed();
        }

        //account is not exists
        if (1 == nRet)
        {
            password_ctl.setText("账户不存在");
        }

        //password id wrong
        else if (2 == nRet)
        {
            password_ctl.setText("密码错误");
        }
        else
        {
            wrong_ctl.setText(error);
        }
    }

    private void register()
    {
        Intent intent = new Intent(this, activity_register.class);
        if (null == intent)
        {
            TextView wrong_ctl = (TextView)findViewById(R.id.login_wrong_tv);
            wrong_ctl.setText("系统错误，内存不足！");
            return;
        }

        startActivity(intent);

    }

    private void onButtonAccept()
    {
        CheckBox accept_ctl = (CheckBox) findViewById(R.id.accept_cb);
        boolean is_accept = accept_ctl.isChecked();

        Button login_ctl = (Button) findViewById(R.id.login_bt);
        login_ctl.setEnabled(is_accept);

        accept_ctl.setChecked(is_accept);
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

    class Msg extends Notify{
        public synchronized  void notify(Notify notify)
        {
            //get msg
            accont_manage accountManager = (accont_manage) notify;
            //perform business

            //update
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    try{
                        //更新UI

                    }catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            });
        }
    }

    private Msg m_msg = new Msg();
}

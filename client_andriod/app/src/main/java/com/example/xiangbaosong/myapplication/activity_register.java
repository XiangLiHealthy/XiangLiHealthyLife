package com.example.xiangbaosong.myapplication;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import com.example.model_lib.*;
import com.example.commondata.*;
import com.example.threadlib.Notify;

public class activity_register extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register);

        EditText account_ctl = (EditText)findViewById(R.id.tel) ;
        account_ctl.setText("13883372441");

        EditText password_ctl = (EditText)findViewById(R.id.verify_code);
        password_ctl.setText("123456");

        account_ctl = (EditText)findViewById(R.id.rgst_password) ;
        account_ctl.setText("666666");

        password_ctl = (EditText)findViewById(R.id.confirm_password);
        password_ctl.setText("666666");
    }

    public void btn_click(View v)
    {
        int nID = v.getId();
        switch (nID)
        {
            case R.id.register:
            {
               register();
            }break;
        }
    }

    private void register()
    {
        //get tel,code,password, confirm password
        data_accout info = new data_accout();

        EditText tel_ctl = (EditText) findViewById(R.id.tel);
        info.tel = tel_ctl.getText().toString();

        EditText code_ctl = (EditText) findViewById(R.id.verify_code);
        info.verify_code = code_ctl.getText().toString();

        EditText password_ctl = (EditText) findViewById(R.id.rgst_password);
        info.password = password_ctl.getText().toString();

        String confirm_password = ( (EditText) findViewById(R.id.confirm_password) ).getText().toString();

        //verify parameter
        TextView tip_ctl = (TextView) findViewById(R.id.register_info_tv);
        if (info.tel.isEmpty())
        {
            tip_ctl.setText("电话不允许为空");
            return;
        }

        if (info.password.isEmpty())
        {
            tip_ctl.setText("密码不允许为空");
            return;
        }

        if (info.verify_code.isEmpty())
        {
            tip_ctl.setText("校验码不能为空");
            return;
        }


        if (info.password.compareTo(confirm_password) != 0)
        {
            tip_ctl.setText("确认密码错误");
            return;
        }


        //perform register
        model_facade model = model_facade.getinstance();
        String error = new String();
        int ret = 0;
        try {
            AccountRequest request = new AccountRequest();
            request.account_info = info;
            request.method = AccountMethod.REGISTER;

            model.getAccountManager().setNotify(m_msg);
            model.getAccountManager().Request(request);

        } catch (Exception e) {
            e.printStackTrace();
        }


        if (0 == ret )
        {
            tip_ctl.setText("注册成功，返回登录");
        }else
        {
            tip_ctl.setText(error);
        }

    }


    //notify
    class Msg extends Notify {
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

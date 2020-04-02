package com.example.net_lib;
import com.example.commondata.*;
import com.example.net_lib.*;

import org.json.JSONException;
import org.json.JSONObject;

import static com.example.commondata.AccountMethod.LOGIN;
import static com.example.commondata.AccountMethod.LOGOUT;
import static com.example.commondata.AccountMethod.REGISTER;
import static com.example.commondata.AccountMethod.UNREGISTER;

public class NetAccountManager {
    public void request(AccountRequest request) throws Exception
    {
        //encode request into json
        JSONObject data = encodeRequest(request);

        //send json
        Net.getInstance().sendJson(data);

        //receive json
        data = Net.getInstance().receiv();

        //decode into request
        AccountRequest tmp = decodeRequest(data);
        request.loginInfo = tmp.loginInfo;
        request.account_info = tmp.account_info;
    }

    public JSONObject encodeRequest(AccountRequest request) throws Exception
    {
        //encode method data
        String method = new String();
        JSONObject data = new JSONObject();

        switch (request.method)
        {
            case LOGIN:
            {
                method = LOGIN.getDescription();
                data = encodeLoginInfo(request.loginInfo);
            }break;

            case LOGOUT:
            {

            }break;

            case REGISTER:
            {
                method = REGISTER.getDescription();
                data = encodeAccount(request.account_info);
            }break;

            case UNREGISTER:
            {

            }break;

        }


        //set method
        JSONObject method_data = new JSONObject();
        method_data.put("method", method);
        method_data.put("data", data);

        //set protocol
        JSONObject proto_data = new JSONObject();
        proto_data.put("protocol", "AccountManager");
        proto_data.put("data", method_data);

        return proto_data;
    }

    public AccountRequest decodeRequest(JSONObject data) throws Exception
    {
        AccountRequest request = new AccountRequest();
        data = data.getJSONObject("data");

        String method = data.get("method").toString();
        if (method.compareTo(LOGIN.getDescription()) == 0)
        {
            request.loginInfo = decodeLoginInfo(data);
            request.method = LOGIN;
        }

        else if (method.compareTo(LOGOUT.getDescription()) == 0)
        {

        }

        else if (method.compareTo(REGISTER.getDescription()) == 0)
        {
            request.method = REGISTER;
            request.account_info = decodeAccount(data);
        }

        else if (method.compareTo(UNREGISTER.getDescription()) == 0)
        {

        }

        else
        {
            String msg = "not surrport method:" + method;
            throw new Exception(msg);
        }

        return request;
    }

//    public int login(LoginInfo info) throws Exception
//    {
//        //encode info
//        JSONObject data = encodeLoginInfo(info);
//
//        //send data
//        sendData(data, "Login");
//
//        //rcv data
//        data = rcvData("Login");
//
//        //decode data
//        info = decodeLoginInfo(data);
//
//        return 0;
//    }
//
//    public int logout(LoginInfo info)
//    {
//        return 0;
//    }
//
//    public void register(data_accout info) throws Exception
//    {
//        JSONObject data = encodeAccount(info);
//
//        sendData(data, "Register");
//
//        data = rcvData("Register");
//
//        info = decodeAccount(data);
//    }
//
//    public int Unregister(data_accout info )
//    {
//        return 0;
//    }
//
//    void sendData(JSONObject data, String method) throws Exception {
//        JSONObject data_tmp = new JSONObject();
//
//        data_tmp.put("method", method);
//        data_tmp.put("data", data);
//
//        JSONObject protocol = new JSONObject();
//        protocol.put("protocol", "AccountManager");
//        protocol.put("data", data_tmp);
//
//
//        Net.getInstance().sendJson(protocol);
//    }

    JSONObject rcvData( ) throws Exception
    {
        JSONObject data = Net.getInstance().receiv();
        if (null == data)
        {
            throw new Exception("receive null");
        }

        if (!data.has("data"))
        {
            throw new Exception("thers isn't data key");
        }

        data = (JSONObject) data.get("data");
        return data;
    }

    JSONObject encodeLoginInfo(LoginInfo info) throws JSONException {
        JSONObject data = new JSONObject();
        data.put("account", info.accout);
        data.put("password", info.password);
        data.put("way", info.way);

        return data;
    }

    LoginInfo decodeLoginInfo(JSONObject data) throws Exception
    {
        LoginInfo info = new LoginInfo();
        info.result = data.getString("result");
        info.desc = data.getString("desc");
        info.user_id = data.getString("user_id");

        return info;
    }

    JSONObject encodeAccount(data_accout info) throws Exception
    {
         JSONObject data = new JSONObject();
         data.put("tel", info.tel);
         data.put("password", info.password);
         data.put("name", info.name);
         data.put("sex", info.sex);
         data.put("tall", info.tall);
         data.put("weight", info.weight);
         data.put("birthday", info.birthday);
         data.put("native_place", info.native_place);
         data.put("family", info.family);
         data.put("marital_status", info.marital_status);
         data.put("blood_type", info.blood_type);
         data.put("occupation", info.occupation);

         return data;
    }

    data_accout decodeAccount(JSONObject data) throws Exception{
        data_accout info = new data_accout();

        info.user_id = data.getString("user_id");
        info.result = data.getString("result");
        info.desc = data.getString("desc");

        return info;
    }
}


package com.example.model_lib;

import com.example.commondata.AccountRequest;
import com.example.commondata.MemberInfo;
import com.example.commondata.data_accout;

import java.util.ArrayList;

import static com.example.model_lib.model_facade.getinstance;

public class MFimalymManager {
    public ArrayList<MemberInfo> getMembersInfo() {
        ArrayList<MemberInfo> members = new ArrayList<>();

        data_accout userInfo = model_facade.getinstance().getAccountManager().getAccountInfo();
        MemberInfo tmp      = new MemberInfo();
        tmp.name            = userInfo.name;
        tmp.user_id         = userInfo.user_id;
        tmp.sex             = userInfo.sex;
        tmp.relationship    = "1000";
        members.add(tmp);

        return members;
    }


}

package com.example.xiangbaosong.myapplication;

import android.graphics.Rect;

import com.example.commondata.MemberInfo;

import java.util.List;

public class FamilyMember {
    private MemberInfo mMemberInfo;
    private Rect mRect;
    private List<Rect> mSymols;

    public int draw() {

        if (drawFrame() < 0)
        {
            return -1;
        }

        if (drawPicture() < 0)
        {
            return -1;
        }

        if (drawSymbols() < 0)
        {
            return -1;
        }

        return 0;
    }

    public void setInfo(MemberInfo info) {
        mMemberInfo = info;
    }

    public MemberInfo getInfo() {
        return  mMemberInfo;
    }

    public void setRect(Rect rect) {
        mRect = rect;
    }

    public void setSymbols(List<Rect> rects) {
        mSymols = rects;
    }

    private int drawFrame() {

        return 0;
    }

    private int drawPicture() {

        return 0;
    }

    private int drawSymbols() {

        return 0;
    }
}

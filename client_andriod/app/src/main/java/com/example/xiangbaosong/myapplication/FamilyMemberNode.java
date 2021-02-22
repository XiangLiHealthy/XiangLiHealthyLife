package com.example.xiangbaosong.myapplication;

import android.graphics.Rect;

import java.lang.reflect.Array;
import java.util.ArrayList;

public class FamilyMemberNode {
    public FamilyMember mSelf;
    public FamilyMemberNode mFather;
    public FamilyMemberNode mMother;
    public ArrayList<FamilyMemberCouple>    mCouples;
    public Rect mRect;
}

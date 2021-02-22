package com.example.xiangbaosong.myapplication;

import android.graphics.Point;
import android.graphics.Rect;
import android.util.Log;

import com.example.commondata.MemberInfo;
import com.example.commondata.data_accout;
import com.example.model_lib.model_facade;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.List;

public class FamilyMemberManager {
    private String MODULE = "FamilyMemberManager";
    FamilyMemberNode mMemberTree;
    FamilyMemberNode mCenterMember;
    private int mFrameWidth = 0;
    private int mFrameHeight = 0;
    private int mVInterval = 0;
    private int mHInterval = 0;
    private Rect mCanvasRect;
    private Point mStartPos;
    private int mLeft = mStartPos.x;
    private int mTop = mStartPos.y;

    int init(Rect rect) {
        mCanvasRect = rect;

        //1.caculate member frame rect
        mFrameWidth     = rect.width() / 3;
        mFrameHeight    = rect.height() / 3;
        mVInterval      = mFrameHeight / 3;
        mHInterval      = mFrameHeight / 5;
        mFrameWidth     -= mVInterval;
        mFrameHeight    -= mHInterval;

        if (createMembers() < 0) {
            return -1;
        }

        return 0;
    }

    public List<FamilyMemberNode> getMembers() {
        return mMembers;
    }

    public int addMember() {

        return 0;
    }

    public int deleteMember() {

        return 0;
    }

    private int createMembers() {
        //1.create member node tree
        if (createMemberTree() < 0)
        {
            Log.e(MODULE, "create member tree node failed");
            return -1;
        }

        //2.caculate member rect
        if (caculateMemberRect() < -1)
        {
            Log.e(MODULE, "caculate member rect failed");
            return -1;
        }

        //3.draw member
        if (drawMember() < 0)
        {
            Log.e(MODULE, "draw member failed");
            return -1;
        }

        //4.translate to someone
        if (translateToROI() < 0)
        {
            Log.e(MODULE, "translate to ROI failed");
            return -1;
        }

        return 0;
    }

    private int createMember(ArrayList<MemberInfo> memberInfos, int top, int axis) {
        //1.find oldest generation members

        //2.get member

        //3.get couple

        //4.caculate rect

        //5.cacalate axis

        //6.caculate top
    }

    private int createMemberTree() {
        //get member_id and family_id of user
        data_accout user_info = model_facade.getinstance().getAccountManager().getAccountInfo();

        //1.get all members info
        ArrayList<MemberInfo> membersInfo = (ArrayList<MemberInfo>) model_facade.getinstance().
                getFamilyMembers(user_info.family_id);


        //3.create member rect
        MemberInfo selfInfo = new MemberInfo();
        selfInfo.member_id = user_info.member_id;
        if (createMemberNode(membersInfo, selfInfo, mMemberTree) < 0)
        {
            Log.e(MODULE, "create member node failed:member_id"
                    + user_info.member_id);
            return -1;
        }

        return 0;
    }

    private int caculateMemberRect() {
        //1.find oldest generation members
        FamilyMemberNode node = getCenterMember();

        if (caculateRect(node, 0, 0) < 0) {
            Log.e (MODULE, "cacualte rect failed");
            return -1;
        }

        return 0;
    }

    private FamilyMemberNode getCenterMember() {
        return mCenterMember;
    }

    private int caculateRect(FamilyMemberNode node, int top) {
        FamilyMemberNode tmpNode = node;
        mLeft = mStartPos.x;
        mTop = mStartPos.y;
        Point startPos = new Point();


            //找到最高的长辈，从上到下开始分配区域
            if (null != tmpNode.mFather) {
                caculateRect(tmpNode.mFather, top - mVInterval);
            }
            else if (null != node.mMother && null == node.mRect) {
               caculateRect(tmpNode.mMother, top - mVInterval);
            }
            else {
                startPos.x = mLeft + mVInterval;
                startPos.y = top;
                createRect(startPos, node);
            }

            //分配配偶的区域
            Point couplePos = new Point();
            couplePos.x = startPos.x + mFrameWidth + mHInterval;
            couplePos.y = startPos.y;

            for (FamilyMemberCouple couple : tmpNode.mCouples) {
                if (couple.mSelf.mFather != null) {
                    caculateRect(couple.mSelf.mFather, top - mVInterval);
                }
                else if (couple.mSelf.mMother != null) {
                    caculateRect(couple.mSelf.mMother, top - mVInterval);
                }
                else {
                    createRect(couplePos, couple.mSelf);
                    couplePos.x += (mFrameWidth + mHInterval);
                }

                //分配对应配偶孩子的绘制区域
                Point childPos = new Point();
                childPos.x = couple.mSelf.mRect.left - mHInterval / 2;
                childPos.y = couple.mSelf.mRect.bottom + mVInterval;

                for (FamilyMemberNode child : couple.mChildren) {
                    createRect(childPos, child);
                    childPos.x += (mFrameWidth + mHInterval);

                    for (FamilyMemberCouple childCouple : child.mCouples) {
                        //递归完成这条线路的所有成员
                        caculateRect(childCouple.mSelf, top + mVInterval + mFrameHeight);
                        childPos.x += (mFrameWidth + mHInterval);
                    }
                }
                createRect(couplePos, couple.mSelf);

                couplePos.x += (mFrameWidth + mHInterval);
            }
        }

        //set current node rect


        for (FamilyMemberCouple couple : node.mCouples) {
            //1.set couple rect

            //set childre rect
            for (FamilyMemberNode child : couple.mChildren) {

            }
        }

        //2.caculate rect

        //3.cacalate axis

        //4.caculate top

        return 0;
    }

    private void createRect(Point letfTop, FamilyMemberNode node) {
        Rect rect = new Rect();
        rect.top = letfTop.x + mVInterval;
        rect.left = letfTop.y + mHInterval;
        rect.bottom = rect.top + mFrameHeight;
        rect.right = rect.right + mFrameWidth;
        node.mRect = rect;

        if (rect.right > mLeft) {
            mLeft = rect.right;
        }
    }

    private int drawMember() {

        return 0;
    }

    private int translateToROI() {

        return 0;
    }

    private int createMemberNode(ArrayList<MemberInfo> membersInfo,MemberInfo selfInfo,
                                 FamilyMemberNode node) {
        if (null == node)
        {
            node = new FamilyMemberNode();
        }

        //find slef
        ArrayList<MemberInfo> memberInfos = findDelMember(membersInfo, selfInfo.member_id, "self");
        for (MemberInfo info : memberInfos) {
            node.mSelf = new FamilyMember();
            node.mSelf.setInfo(info);
        }

        //find father
        memberInfos = findDelMember(membersInfo, selfInfo.member_id,
                "father");
        for (MemberInfo info : memberInfos) {
            node.mFather = new FamilyMemberNode();
            node.mFather.mSelf.setInfo(info);
        }

        //find mother
        memberInfos = findDelMember(membersInfo, selfInfo.member_id, "mother");
        for (MemberInfo info : memberInfos) {
            node.mMother = new FamilyMemberNode();
            node.mMother.mSelf.setInfo(info);
        }

        //find couple
        memberInfos = findDelMember(membersInfo, selfInfo.member_id, "couple");
        for (MemberInfo info : memberInfos) {
            FamilyMemberCouple couple = new FamilyMemberCouple();

            couple.mSelf = new FamilyMemberNode();
            couple.mSelf.mSelf = new FamilyMember();
            couple.mSelf.mSelf.setInfo(info);

            ArrayList<MemberInfo> childrenInfo = findDelMember(memberInfos, info.member_id,
                    "children");
            for (MemberInfo child : childrenInfo) {
                FamilyMemberNode childNode = new FamilyMemberNode();
                childNode.mSelf.setInfo(child);

                if (selfInfo.sex == "Male") {
                    childNode.mFather = node;
                    childNode.mMother = couple.mSelf;
                } else {
                    childNode.mFather = couple.mSelf;
                    childNode.mMother = node;
                }

                couple.mChildren.add(childNode);
            }

            node.mCouples.add(couple);
        }

        //recursive deal father
        createMemberNode(membersInfo, node.mFather.mSelf.getInfo(), node.mFather);

        //recursive deal mather
        createMemberNode(membersInfo, node.mMother.mSelf.getInfo(), node.mMother);

        //recursive deal children
        for (FamilyMemberCouple couple : node.mCouples) {
            for (FamilyMemberNode child : couple.mChildren) {
                createMemberNode(membersInfo, child.mSelf.getInfo(), child);
            }
        }

        return 0;
    }

    private ArrayList<MemberInfo> findDelMember(ArrayList<MemberInfo> infos, String member_id, String relationship) {
        ArrayList<MemberInfo> infosTmp = new ArrayList<MemberInfo>();

        for (MemberInfo info : infos) {
            if (member_id == info.member_id && relationship ==
                    info.relationship) {
                infosTmp.add(info);

                infos.remove(info);
            }
        }

        return infosTmp;
    }


}

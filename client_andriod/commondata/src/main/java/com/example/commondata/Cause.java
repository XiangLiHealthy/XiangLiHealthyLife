package com.example.commondata;

public class Cause {
    public Cause () {
        nFeedback = 5;
        nSuspect = 1;
        nDataSrc = EDataSource.DIETE;
        bSelet = false;
        int nID = 0;
        strDesc = "喝凉水";
    }

    public int nFeedback;
    public int nSuspect;
    public EDataSource nDataSrc;
    public boolean bSelet;
    public int nID;
    public Object image;
    public String strDesc;
}

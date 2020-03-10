package com.example.net_lib;
import com.example.net_lib.NetFrame;

import java.text.SimpleDateFormat;
import java.util.Calendar;

enum SEQ_STATE
{
    SEQ_START,
    SEQ_BODY,
    SEQ_END,
    SEQ_START_END,
    SEQ_STATE_UNKNOW,
};

enum DATA_TYPE
{
    DATA_JONS,
    DATA_PICTURE,
    DATA_MOVIE,
    DATA_UNKNOW,
};

public class NetSequence {

    static NetSequence singleton = new NetSequence();
    public static NetSequence getInstance()
    {
          return singleton;
    }

    //send msg
    public void resetSeq()
    {
        m_strUID = "";
        m_current_seq_num = 0;
        m_seq_state = SEQ_STATE.SEQ_START;
    }

    public  void setUID(String UID)
    {
        m_strUID = UID;
    }

    public void setType(DATA_TYPE type)
    {
        m_data_type = type;
    }

    public  int sendData(byte[] arrData) throws Exception {
        if (m_strUID.isEmpty())
        {
            m_strUID = createUID();
        }
        byte[] arrUID = m_strUID.getBytes();


       m_current_seq_num = (short) sendSeq(m_strUID, m_data_type, m_seq_state, m_current_seq_num ++,
                arrData);

        if (m_seq_state == SEQ_STATE.SEQ_START)
        {
            m_seq_state = SEQ_STATE.SEQ_BODY;
        }

        return 0;
    }

    public int endSeq() throws Exception {
        try
        {
            m_seq_state = SEQ_STATE.SEQ_END;

            byte[] tmp = new byte[1];

            return  sendData(tmp);
        }catch (Exception e)
        {
            throw new Exception(e.getMessage());
        }
    }

    public int sendDataOnce(byte[] arrData, DATA_TYPE type) throws Exception {
            sendSeq(createUID(), type, SEQ_STATE.SEQ_START_END,
                    (short) 0, arrData);

        return 0;

    }

    private int sendSeq(String strUID, DATA_TYPE dt_type, SEQ_STATE seq_state,
                        short seq_num, byte[] arrData) throws Exception {

        SEQ_STATE state_tmp = seq_state;

        if (state_tmp == SEQ_STATE.SEQ_START_END && arrData.length > MAX_SEQ_DATA_LEN)
        {
            seq_state = SEQ_STATE.SEQ_START;
        }

        int nSentLen = 0;
        while (arrData.length - nSentLen  > 0)
        {

            int nSendLen = ( (arrData.length - nSentLen) > MAX_SEQ_DATA_LEN ) ? MAX_SEQ_DATA_LEN : (arrData.length - nSentLen) ;

            seq_state = (arrData.length - nSentLen > 0 && state_tmp == SEQ_STATE.SEQ_END) ? SEQ_STATE.SEQ_BODY : seq_state;

            seq_state = (arrData.length - nSentLen <= 0 && state_tmp == SEQ_STATE.SEQ_START_END) ? SEQ_STATE.SEQ_START_END : seq_state;

            FrameData frameData = new FrameData();
            frameData.setUID(strUID);

            short data_type = getDataTypeVale(dt_type);
           frameData.setDataType((byte) data_type);

            short state_value = getStateValue(seq_state);
            frameData.setSeqState((byte)state_value);

           frameData.setSeqNum(seq_num);

           frameData.setPaloadData(arrData, nSentLen, nSendLen);

            m_frame.sendSeq(frameData);


            seq_num++;
            nSentLen += nSendLen;
            if (SEQ_STATE.SEQ_START == seq_state)
            {
                seq_state = SEQ_STATE.SEQ_BODY;
            }
        }

        return seq_num;
    }

    private short getStateValue(SEQ_STATE seq_state)
    {
        switch (seq_state)
        {
            case SEQ_START:
                return 0;
            case SEQ_BODY:
                return 1;
            case SEQ_END:
                return 2;
            case SEQ_START_END:
                return 3;
                default:
                    return -1;
        }
    }
    private SEQ_STATE getSeqState(short seq_state)
    {
        switch (seq_state)
        {
            case 0:
                return SEQ_STATE.SEQ_START;

            case 1:
                return SEQ_STATE.SEQ_BODY;

            case 2:
                return SEQ_STATE.SEQ_END;

            case 3:
                return SEQ_STATE.SEQ_END;

            default:
                return SEQ_STATE.SEQ_STATE_UNKNOW;
        }
    }

    private short getDataTypeVale(DATA_TYPE type)
    {
        switch (type)
        {
            case DATA_JONS:
                return 1;

            case DATA_PICTURE:
                return 2;

            case DATA_MOVIE:
                return 3;

            default:
                return -1;
        }
    }

    private DATA_TYPE getDataType(short type)
    {
        switch (type)
        {
            case 1:
                return DATA_TYPE.DATA_JONS;

            case 2:
                return DATA_TYPE.DATA_PICTURE;

            case 3:
                return DATA_TYPE.DATA_MOVIE;

            default:
              return DATA_TYPE.DATA_UNKNOW;
        }
    }

    private  String createUID()
    {
        Calendar calendar = Calendar.getInstance();

        long td = Thread.currentThread().getId();

//        String uid = String.format("%04d%02d%02d%02%02d%02d.%03d.%d",
//                calendar.YEAR, calendar.MONTH, calendar.DATE,
//                calendar.HOUR, calendar.MINUTE, calendar.SECOND, calendar.MILLISECOND,
//                td);
        String uid = String.format("%04d%02d%02d",
                calendar.YEAR, calendar.MONTH, calendar.DATE
                );
        uid += String.format("%02d%02d%02d.%03d",
                calendar.HOUR, calendar.MINUTE, calendar.SECOND, calendar.MILLISECOND);
        uid += String.format(".%08d", td);


        return uid;
    }

    //receive msg
    public  byte[] recvData() throws Exception {
        byte[] arrSeq = m_frame.RcvSeq();

        if (arrSeq.length < SEQ_HEADER_SIZE)
        {
            throw new Exception("seq size to small");
        }

        //uid
        byte[] arrUID = new byte[UID_SIZE];
        System.arraycopy(arrUID, 0, arrSeq, 0, UID_SIZE);
        m_strUID = new String(arrUID);

        //data_type
        m_data_type = getDataType(arrSeq[UID_SIZE]);

        //seq state
        m_seq_state = getSeqState(arrSeq[UID_SIZE + 1]);

        //seq num
        m_current_seq_num = (short)(arrSeq[UID_SIZE + 2] * 256 + arrSeq[UID_SIZE + 3] );

        //data
        byte[] arrData = new byte[arrSeq.length - SEQ_HEADER_SIZE];
        System.arraycopy(arrData, 0, arrSeq, SEQ_HEADER_SIZE, arrData.length);

        return arrData;
    }

    public DATA_TYPE getType()
    {
        return m_data_type;
    }

    public String getUID()
    {
        return  m_strUID;
    }

    public SEQ_STATE getSeqState()
    {
        return  m_seq_state;
    }

    private NetFrame m_frame = new NetFrame();

    String m_strUID = new String();
    DATA_TYPE m_data_type;
    SEQ_STATE m_seq_state;
    short m_current_seq_num;

    private int UID_SIZE = 32;
    private int SEQ_HEADER_SIZE = 36;
    private int MAX_SEQ_SIZE = NetFrame.MAX_FRAME_SIZE - NetFrame.FRAME_FORMAT_LEN;
    private int MAX_SEQ_DATA_LEN = MAX_SEQ_SIZE - SEQ_HEADER_SIZE;
}

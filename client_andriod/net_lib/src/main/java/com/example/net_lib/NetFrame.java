package com.example.net_lib;


import java.io.DataInput;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.io.Writer;
import java.net.Socket;


class FrameData
{
    private byte[] m_frame_data = new byte[MAX_FRAME_SIZE];
    private short m_payload_len;
    static public int MAX_FRAME_SIZE = 8192;
    static public int FRAME_FORMAT_LEN = 8;
    static public int UID_SIZE = 32;
    static public int SEQ_FORMAT_LEN = UID_SIZE + 4;

    //frame oporat
    public byte[] getHeader()
    {
        byte[] header = new byte[2];

        header[0] = m_frame_data[0];
        header[1] = m_frame_data[1];

        return  header;
    }

    public void setHeader(byte[] header)
    {
        m_frame_data[0] = header[0];
        m_frame_data[1] = header[1];
    }

    public short getCheckSum()
    {
        int nCheckSum = m_frame_data[2] * 256;
        nCheckSum += m_frame_data[3] & 0xff;

        return  (short) nCheckSum;
    }

    public  void setCheckSum(short sum)
    {
        m_frame_data[2] = (byte) (sum / 256);
        m_frame_data[3] = (byte) (sum & 0x00ff);
    }

    public  short getSeqLen()
    {
        int len = m_frame_data[4] * 256;
        len += m_frame_data[5] & 0xff;

        return (short) len;
    }

    public void setSeqLen(short len)
    {
        m_frame_data[4] = (byte) (len / 256);
        m_frame_data[5] = (byte) (len & 0x00ff);
    }

    public byte[] getTail()
    {
        byte[] tail = new byte[2];
        short nSeqLen = getSeqLen();

        tail[0] = m_frame_data[nSeqLen + FRAME_FORMAT_LEN -2];
        tail[1] = m_frame_data[nSeqLen + FRAME_FORMAT_LEN - 1];

        return  tail;
    }

    public void setTail(byte[] tail)
    {
        short nSeqLen = getSeqLen();

        m_frame_data[nSeqLen + FRAME_FORMAT_LEN -2] = tail[0];
        m_frame_data[nSeqLen + FRAME_FORMAT_LEN - 1] = tail[1];
    }

    //sequnec oprator
    public void setUID(String strUID)
    {
        byte[] byUID = strUID.getBytes();
        System.arraycopy( byUID, 0, m_frame_data, FRAME_FORMAT_LEN - 2, byUID.length);

    }

    public String getUID()
    {
        byte[] byUID = new byte[UID_SIZE];
        System.arraycopy( byUID, 0, m_frame_data, FRAME_FORMAT_LEN - 2, UID_SIZE);

        String strUID = new String(byUID);

        return  strUID;
    }

    public void setDataType(byte type)
    {
        m_frame_data[FRAME_FORMAT_LEN - 2 + UID_SIZE] = type;
    }

    public byte getDataType()
    {
        return  m_frame_data[FRAME_FORMAT_LEN - 2 + UID_SIZE];
    }

    public void setSeqState(byte state)
    {
        m_frame_data[FRAME_FORMAT_LEN - 2 + UID_SIZE + 1] = state;
    }

    public byte getSeqState()
    {
        return m_frame_data[FRAME_FORMAT_LEN - 2 + UID_SIZE + 1];
    }

    public void setSeqNum(short num)
    {
        m_frame_data[FRAME_FORMAT_LEN - 2 + UID_SIZE + 3] = (byte) (num / 256);
        m_frame_data[FRAME_FORMAT_LEN - 2 + UID_SIZE + 4] = (byte) (num & 0x00ff);
    }

    public short getSeqNum()
    {
        return  (short) (m_frame_data[FRAME_FORMAT_LEN - 2 + UID_SIZE + 3] * 256
                + m_frame_data[FRAME_FORMAT_LEN - 2 + UID_SIZE + 4] );
    }

    public void setPaloadData(byte[] data, int nPos, int nLen) throws Exception {
        if (data.length > MAX_FRAME_SIZE - FRAME_FORMAT_LEN - SEQ_FORMAT_LEN)
        {
            throw  new Exception("data too long");
        }

        if (null != data)
        {
            System.arraycopy( data, nPos, m_frame_data, FRAME_FORMAT_LEN + SEQ_FORMAT_LEN - 2, nLen);
        }

        setSeqLen( (short) (nLen + SEQ_FORMAT_LEN) );
    }

    public byte[] getPayload()
    {
       int nLen = getSeqLen() - SEQ_FORMAT_LEN;

       byte[] paload = new byte[nLen];

       System.arraycopy(m_frame_data, FRAME_FORMAT_LEN - SEQ_FORMAT_LEN - 2, paload, 0, nLen);

       return  paload;
    }

    public void setFrameData(byte[] data)
    {
        m_frame_data = data;
    }

    public byte[] getFrameData()
    {
        return m_frame_data;
    }

    public short getFrameLen()
    {
        return (short) (getSeqLen() + FRAME_FORMAT_LEN);
    }
};



enum FIND_STATE
{
    FIND_OK,
    FIND_CONTINUE,
    FIND_ERROR
};

public class NetFrame {
    NetFrame()
    {

        initNet();
    }

    int initNet()
    {
        try
        {
            //m_client = new Socket("10.0.2.2",8888);
            //m_client = new Socket("127.0.0.1",8888);
            //m_client = new Socket("192.168.43.246",8888);
            m_client = new Socket("122.51.38.230",8888);
            m_client.setSoTimeout(5*1000);
            m_sender = new DataOutputStream(m_client.getOutputStream());
            m_receiver = new DataInputStream(m_client.getInputStream());
        }catch (Exception e)
        {
            m_last_error = e.getMessage();
            e.printStackTrace();
        }

        return 0;
    }

    public  int sendSeq(FrameData frameData)
    {
        try
        {
            frameData.setHeader(FRAME_HEADER);

            frameData.setCheckSum((short) 0);

            //frameData.setSeqLen();

            frameData.setTail(FRAME_TAIL);

            byte[] byData = frameData.getFrameData();
            int nLen = frameData.getFrameLen();

            m_sender.write(byData, 0, nLen);
        }catch (Exception e)
        {
            m_last_error = e.getMessage();
            return -1;
        }

        return 0;

    }

    public byte[] packageFrame(byte[] arrData)
    {
        //get frame length
        //create a frame
        byte[] frame = new byte[arrData.length + FRAME_FORMAT_LEN];

        //fill header
        frame[0] = FRAME_HEADER[0];
        frame[1] = FRAME_HEADER[1];

        short checksum = getCheckSum(arrData);
        frame[2] = (byte) (checksum & 0xff);
        frame[3] = (byte) (checksum & 0xff00);

        short len = (short) arrData.length;
        frame[4] = (byte)(len & 0xff);
        frame[5] = (byte)(len & 0xff00);

        //fill in data
        System.arraycopy(arrData, 0, frame,FRAME_FORMAT_LEN, len);

        //fill tail
        frame[FRAME_FORMAT_LEN - 2 + len] = FRAME_TAIL[0];
        frame[FRAME_FORMAT_LEN - 2 + len + 1] = FRAME_TAIL[1];

        return frame;

    }

    public  byte[] RcvSeq() throws Exception
    {
        try{
            do {
                    //find header pos
                    FIND_STATE state = findHeader();
                    if (state == FIND_STATE.FIND_OK)
                    {
                        //find tail pos
                        state = findTail();
                        if (state == FIND_STATE.FIND_OK)
                        {
                            break;
                        }

                        if (state == FIND_STATE.FIND_ERROR)
                        {
                            m_header_pos = 0;
                            m_rcv_len = 0;
                            throw new Exception("find tailf of fram error");
                        }
                    }
                    else if (m_rcv_len > 0)
                    {//save last byte, maybe it is a half of header
                        int len = m_tail_pos + m_rcv_len > MAX_FRAME_SIZE ? MAX_FRAME_SIZE :  m_tail_pos + m_rcv_len;
                        m_recv_buff[0] = m_recv_buff[len - 1];
                        m_header_pos = 0;
                        m_rcv_len = 1;
                        m_tail_pos = 1;
                    }

                    int nLen = m_receiver.read(m_recv_buff, m_header_pos + m_rcv_len, m_recv_buff.length - (m_header_pos + m_rcv_len) );
                    m_rcv_len += nLen;
            }while (true);

            //parse frame
            return parseFrame();
        }catch (Exception e)
        {
            m_last_error = "parse frame error" + e.getMessage();
            throw new Exception(m_last_error);
        }
    }

    private short getCheckSum(byte[] arrData)
    {
        //checksum
        short checksum = 0;
        return  checksum;
    }

    private FIND_STATE findHeader()
    {
        for (short index = 0; index < m_rcv_len; index ++)
        {
            int pos = m_header_pos + index;
            if (m_recv_buff[pos] == FRAME_HEADER[0] && m_recv_buff[index + 1] == FRAME_HEADER[1])
            {
                m_header_pos = pos;
                return FIND_STATE.FIND_OK;
            }
        }

        return FIND_STATE.FIND_CONTINUE;
    }

    private  FIND_STATE findTail()
    {
        //get data len
        if (m_rcv_len < FRAME_FORMAT_LEN)
        {
            return  FIND_STATE.FIND_CONTINUE;
        }

        //服务器是小段序
        int nDataLen = 0;
        nDataLen = m_recv_buff[m_header_pos + 5] * 256 & 0xffff;
        nDataLen +=  m_recv_buff[m_header_pos + 4] & 0xff;

        if (m_rcv_len < FRAME_FORMAT_LEN + nDataLen)
        {
            return  FIND_STATE.FIND_CONTINUE;
        }

        if (m_recv_buff[m_header_pos + FRAME_FORMAT_LEN + nDataLen - 2] == FRAME_TAIL[0]
                && m_recv_buff[m_header_pos + FRAME_FORMAT_LEN + nDataLen -1] == FRAME_TAIL[1])
        {
            m_data_len = nDataLen;
            m_tail_pos = m_header_pos + FRAME_FORMAT_LEN + nDataLen - 2;
            return FIND_STATE.FIND_OK;
        }

        return FIND_STATE.FIND_ERROR;
    }

    private byte[] parseFrame()
    {
        byte[] arrData = new byte[m_data_len];
        System.arraycopy(m_recv_buff, m_header_pos + FRAME_FORMAT_LEN - 2, arrData, 0, m_data_len);

        m_header_pos = m_header_pos + FRAME_FORMAT_LEN + m_data_len;
        m_rcv_len = m_rcv_len - FRAME_FORMAT_LEN - m_data_len;

        return arrData;
    }

    private byte[] FRAME_HEADER = {0x11, 0x22};
    private byte[] FRAME_TAIL = {0x33, 0x44};


    private Socket m_client ;
    private DataOutputStream m_sender;
    private DataInputStream m_receiver;

    private String m_last_error;


    private byte[] m_recv_buff = new byte[MAX_FRAME_SIZE];
    private int m_header_pos = 0;
    private int m_tail_pos = 0;
    private int m_data_len = 0;
    private int m_rcv_len = 0;

    static public int MAX_FRAME_SIZE = 8192;
    static public int FRAME_FORMAT_LEN = 8;

}

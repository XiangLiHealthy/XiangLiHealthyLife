package com.example.net_lib;


import java.io.DataInput;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.io.Writer;
import java.net.Socket;

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
            m_client = new Socket("10.0.2.2",6666);
            m_sender = new DataOutputStream(m_client.getOutputStream());
            m_receiver = new DataInputStream(m_client.getInputStream());
        }catch (Exception e)
        {
            m_last_error = e.getMessage();
            e.printStackTrace();
        }

        return 0;
    }

    public  int sendSeq(byte[] arrData)
    {
        try
        {
            byte[] frame = packageFrame(arrData);

            m_sender.write(frame, 0, frame.length);
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
        System.arraycopy(frame,FRAME_FORMAT_LEN, arrData, 0, len);

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


                //save last byte, maybe it is a half of header
                if (m_rcv_len > 0)
                {
                    m_recv_buff[0] = m_recv_buff[m_tail_pos + m_rcv_len - 1];
                    m_header_pos = 0;
                    m_rcv_len = 1;
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

        int nDataLen = m_recv_buff[m_header_pos + 4] * 256 + m_recv_buff[m_rcv_len + 5];

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
        System.arraycopy(arrData, 0, m_recv_buff, m_header_pos + FRAME_FORMAT_LEN - 2,m_data_len);

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

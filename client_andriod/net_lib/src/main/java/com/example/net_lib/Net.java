package com.example.net_lib;

import android.support.annotation.NonNull;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.net.Socket;
import java.util.Collection;
import java.util.HashMap;
import java.util.IdentityHashMap;
import java.util.Map;
import java.util.Set;
import java.util.Vector;

import com.example.net_lib.SeqData;
import com.example.net_lib.NetSequence;

public class Net extends Thread {

    private  static Net m_singleton = new Net();
    private NetSequence m_seq = NetSequence.getInstance();
    public NetJson m_net_json = new NetJson(m_seq);

    public static Net getInstance()
    {
        return  m_singleton;
    }

    public  static void main (String[] args)
    {
        String host = "127.0.0.1";
        int port = 6666;

        try {
            String msg = "{\"proto\":\"hellow world杨洋\"}";
            byte[] byMsg = msg.getBytes();

            while(true)
            {
                if (NetSequence.getInstance().sendDataOnce(byMsg, DATA_TYPE.DATA_JONS) < 0)
                {

                }

                sleep(1000);
            }
        } catch (Exception e) {
            String msg = e.getMessage();
            e.printStackTrace();
        }
    }

    //send msg
    public void sendJson(JSONObject jData) throws Exception
    {
        NetSequence seq_manager = NetSequence.getInstance();
        String test = new String(jData.toString().getBytes("UTF-8"));

        seq_manager.sendDataOnce(jData.toString().getBytes("UTF-8"), DATA_TYPE.DATA_JONS);
    }
    public void sendPicture(String file_name)
    {

    }
    public void sendPicture(String file_name, byte[] arrData)
    {

    }
    public void sendMovie(String file_name)
    {

    }

    //receive msg
    public JSONObject recvJson() throws Exception {
        NetSequence seq_manager = NetSequence.getInstance();

        byte[] arrData = seq_manager.recvData();
        SEQ_STATE seq_state = seq_manager.getSeqState();

        JSONObject jData = null;
        switch (seq_state)
        {
            case SEQ_BODY:
            case SEQ_START:
            {
                SeqData seqData = new SeqData();
                seqData.UID = seq_manager.getUID();
                seqData.data_type = seq_manager.getType();
                seqData.arrData = arrData;

                Vector<SeqData> seqs = m_seqs_chache.get(seqData.UID);
                seqs.add(seqData);
            }break;

            case SEQ_END:
            {
                String UID = seq_manager.getUID();

                Vector<SeqData> seqs = m_seqs_chache.get(UID);

                //get total len
                int nLen = 0;
                for (SeqData item: seqs)
                {
                    nLen += item.arrData.length;
                }
                byte[] arrTmp = new byte[nLen];

                //get all data
                int nPos = 0;
                for (SeqData item : seqs )
                {
                    System.arraycopy(arrTmp, nPos, item.arrData, 0, item.arrData.length);

                    nPos = item.arrData.length;
                }
                seqs.clear();

                String string = new String(arrTmp);
                jData = new JSONObject(string);
            }break;

            case SEQ_START_END:
            {
                String strData = new String(arrData);
                jData = new JSONObject(strData);
            }break;

            default:
                throw new Exception("unknown seq type");
        }

        return jData;
    }

    public String recvPicture()
    {
        return new String();
    }

    public byte[] recvPictureStream()
    {
        return  new byte[1];
    }

    public String recvMovie()
    {
        return  new String();
    }



    public int send(JSONObject jData)
    {
        try
        {
            synchronized (m_seq)
            {
                m_net_json.SengMsg(jData);
            }
        }catch (Exception e)
        {

            return  -1;
        }


        return 0;
    }

    public JSONObject receiv()
    {
        JSONObject jData = null;
        try
        {
            synchronized (m_seq)
            {
                return m_net_json.rcvMsg();
            }

        }catch (Exception e)
        {
            e.printStackTrace();
            return null;
        }

    }


    private HashMap<String, Vector<SeqData>> m_seqs_chache = new HashMap<>();
}

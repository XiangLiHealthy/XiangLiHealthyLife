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
    private Socket m_client;
    private Writer m_sender;
    private InputStreamReader m_receiver;
    private boolean m_bConnet = false;
    private boolean m_bShutDown = false;

    public  static void main (String[] args) {
        String host = "127.0.0.1";
        int port = 6666;

        try {
            Socket client = new Socket(host, port);
            Writer writer = new OutputStreamWriter(client.getOutputStream());

            writer.write("Hellow from Client\n" +
                    "");
            writer.flush();

            writer.close();


            client.close();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    //send msg
    public void sendJson(JSONObject jData) throws Exception {

        NetSequence seq_manager = NetSequence.getInstance();
        seq_manager.sendData(jData.toString().getBytes());

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

    public synchronized boolean connet() {
        String host = "10.0.2.2";
        int port = 6666;

        if (m_bConnet) {
            return true;
        }

        try {
             //Socket client  = new Socket(host, 6666);
             m_client       = new Socket(host, port);
             m_sender       = new OutputStreamWriter(m_client.getOutputStream());
             m_receiver     = new InputStreamReader(m_client.getInputStream());


        } catch (Exception e)
        {
            e.printStackTrace();
            return false;
        }

        m_bConnet = true;
        return true;
    }

    public int send(JSONObject jData)
    {
        try
        {
            synchronized (m_client)
            {
                BufferedWriter bw = new BufferedWriter(m_sender);

                //向服务器端发送一条消息
                bw.write(jData.length());
                bw.write(jData.toString());

                bw.flush();
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
            synchronized (m_client)
            {
                //读取服务器返回的消息
                BufferedReader br = new BufferedReader(m_receiver);

                //读取长度
                char szLeng[] = new char[4];
                int nLen = br.read(szLeng, 0, 4);
                if (nLen < 1) {
                    return null;
                }

                char[] szData = new char[nLen + 1];
                int nNewLen = br.read(szData, 0, nLen);
                if (nNewLen < nLen)
                {
                    return null;
                }

                String str = new String(szData);
                jData = new JSONObject(str);
            }

        }catch (Exception e)
        {

            return null;
        }

        return  jData;
    }


    private HashMap<String, Vector<SeqData>> m_seqs_chache = new HashMap<>();
}

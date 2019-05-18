package com.example.net_lib;

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

    //线程启动函数
    //public void run() {
    //    if (0 !connet() ) {
    //        return ;
    //    }
//
    //    //运行网络任务
    //    while(!m_bShutDown) {
    //        //从队列里面取出发送任务
//
//
    //        //执行发送
//
    //        //接收数据
//
    //        //将接收对象数据放到消息队列
//
    //        //通知业务线程来处理
    //    }
    //}



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

    public int send(JSONObject jData) {
        try {
            synchronized (m_client) {
                BufferedWriter bw = new BufferedWriter(m_sender);

                //向服务器端发送一条消息
                bw.write(jData.length());
                bw.write(jData.toString());

                bw.flush();
            }
        }catch (Exception e) {

            return  -1;
        }




        return 0;
    }

    public JSONObject receiv() {
        JSONObject jData = null;
        try{
            synchronized (m_client) {
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
                if (nNewLen < nLen) {
                    return null;
                }

                String str = new String(szData);
                jData = new JSONObject(str);
            }

        }catch (Exception e) {

            return null;
        }

        return  jData;
    }
}

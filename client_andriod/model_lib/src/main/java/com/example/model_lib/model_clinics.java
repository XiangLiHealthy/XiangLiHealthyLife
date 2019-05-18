package com.example.model_lib;

import com.example.commondata.*;
import com.example.net_lib.NetFacade;
import com.example.threadlib.Notify;
import com.example.threadlib.Task;
import com.example.threadlib.TaskQueue;

import java.util.Date;
//import java.util.Calendar;
//import com.example.net_lib.*;
import java.text.SimpleDateFormat;

public class model_clinics  extends Notify {
    public  int GetItemList(){
        return 0;
    }
    public  int  getNetCode(){return  net_return;}

    public void setDetail(String strDetail) {
        m_strDetail = strDetail;
    }
    public Treatment getTreatment() {
        return m_treatment;
    }

    public void setItem(enum_item item) {
        m_item = item;
    }

    public void notify(Notify task){
        synchronized (m_treatment) {
            try{
                GetDiagnosisElement getSymptoms = (GetDiagnosisElement) (task);
                Treatment treatment = getSymptoms.getData();

                //症状描述
                if (enum_item.SYMPTOM == m_treatment.m_eItem) {
                    m_treatment.m_symptomContainor.add(treatment.m_symptomContainor.getData());
                } else if(enum_item.CAUSE == m_treatment.m_eItem) {//原因
                    m_treatment.m_causeContainer.add(treatment.m_causeContainer.m_lstItem);
                }else if(enum_item.DIAGNOSIS == m_treatment.m_eItem) {//诊断
                    m_treatment.m_diagnosis.add(treatment.m_diagnosis.getData());
                }else if(enum_item.SOLUTION == m_treatment.m_eItem) {//解决方法
                    m_treatment.m_solution.m_lstItem.addAll(treatment.m_solution.m_lstItem);
                }
                else if(enum_item.FINISH == m_treatment.m_eItem) {

                }
                else {
                    net_return = -1;
                    lastErr = "错误的诊断步骤";
                }

            }catch (Exception e) {
                net_return = -1;
                lastErr = e.toString();
            }
            }


        m_notify.notify(this);
    }

    /**
     * 获取症状描述
     * @return 返回描述列表
     */
    public SymptomContainer RequestDiagnosis(final enum_item eItem) {
        //从网络中获取一批数据
        try
        {
            m_treatment.m_eItem = eItem;

            TaskQueue.taskQueue.put(new GetDiagnosisElement(this, m_treatment) {
                @Override
                public void run() throws Exception {
                    try
                    {
                        newTreatment = NetFacade.getInstance().request(eItem, m_treatment,1);
                    }catch (Exception e) {
                        net_return = -1;
                        lastErr = e.toString();
                    }

                }
            });
        } catch (InterruptedException e)
        {
            e.printStackTrace();
        }

        return  null;
    }

    public SymptomContainer GetSymptoms() {
        return  m_treatment.m_symptomContainor.getSymptoms();
    }


    public CauseContainer getCause(boolean bMore) {
        return  m_treatment.m_causeContainer.getCauses(bMore);
    }

    public DiagnosisContainer getDiagnosis(boolean bMore) {
        return  m_treatment.m_diagnosis.getContainer(bMore);
    }

    public SolutionContainer getSolution(boolean bMore) {
        return m_treatment.m_solution.getSolution(bMore);
    }

    public void setCilicsFrom(boolean bDoctor) {
        m_treatment.m_bFromDoctor = bDoctor;
    }

    void  setUploadState(boolean bSuc) {
        m_treatment.m_bUploadState = bSuc;
    }

    /**
     * 诊断记录接口
     */
    public String getDatetime() {
        return  m_treatment.m_strCreateTime;
    }

    public int getDays() {
        //获取当前时间
        try {
            Date now = new Date();
            SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");//可以方便地修改日期格式
            String strToday = dateFormat.format( now );


            //减去记录生成时间,以后改成用户自定义时间,默认为当前时间
            long from       = dateFormat.parse(m_treatment.m_strCreateTime).getTime();
            long to         = dateFormat.parse(strToday).getTime();
            return (int) ((to - from)/(1000 * 60 * 60 * 24));
        }catch (Exception e) {
            e.printStackTrace();
        }

        return  0;
    }
    /*******************************************************************
     *
     */



private  Treatment m_treatment = new Treatment();
private enum_item m_item;
private String m_strDetail;

private int net_return;
private String lastErr;
}

abstract class GetDiagnosisElement  extends Task{
    final Treatment old;
    Treatment       newTreatment;

    public GetDiagnosisElement(Notify notify, Treatment treatment) {
        m_notify = notify;
        old = treatment;
    }

    public Treatment getData() {
        return newTreatment;
    }
}
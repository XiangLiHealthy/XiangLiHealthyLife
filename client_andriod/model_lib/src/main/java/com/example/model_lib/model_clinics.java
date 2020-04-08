package com.example.model_lib;

import com.example.commondata.*;
import com.example.net_lib.NetFacade;
import com.example.threadlib.Notify;
import com.example.threadlib.Task;
import com.example.threadlib.TaskQueue;

import java.util.ArrayList;
import java.util.Date;
//import java.util.Calendar;
//import com.example.net_lib.*;
import java.text.SimpleDateFormat;

import javax.xml.transform.Source;

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
        synchronized (m_treatment)
        {
            try{
                    GetDiagnosisElement getSymptoms = (GetDiagnosisElement) (task);
                    Treatment treatment = getSymptoms.getData();

                    handleTask(treatment);
            }catch (Exception e)
            {
                net_return = -1;
                lastErr = e.toString();
            }
        }


        m_notify.notify(this);
    }

    private void handleTask(Treatment treatment)
    {
        if (null == treatment)
        {
            return;
        }

        //症状描述
        if (enum_item.SYMPTOM == m_treatment.m_eItem)
        {
            ArrayList<Symptom> tmp = treatment.m_symptomContainor.getData();
            m_treatment.m_symptomContainor.add(tmp);
        }
        else if(enum_item.CAUSE == m_treatment.m_eItem)
        {//原因
            ArrayList<Cause> tmp = treatment.m_causeContainer.m_lstItem;
            m_treatment.m_causeContainer.add(tmp);
        }
        else if(enum_item.DIAGNOSIS == m_treatment.m_eItem)
        {//诊断
            ArrayList<Diagnosis> tmp = treatment.m_diagnosis.getData();
            m_treatment.m_diagnosis.add(tmp);
        }else if(enum_item.SOLUTION == m_treatment.m_eItem)
        {//解决方法
            ArrayList<Solution> tmp = treatment.m_solution.m_lstItem;
            m_treatment.m_solution.m_lstItem.addAll(tmp);
        }
        else if(enum_item.FINISH == m_treatment.m_eItem)
        {

        }
        else {
            net_return = -1;
            lastErr = "错误的诊断步骤";
        }
    }

    /**
     * 获取症状描述
     * @return 返回描述列表
     */
    public SymptomContainer RequestDiagnosis(final enum_item eItem) {
        //从网络中获取一批数据
        try
        {
            Treatment tmp = createTreatment();
            String user_id = model_facade.getinstance().getAccountManager().getAccountInfo().user_id;

            tmp.m_eItem = eItem;
            tmp.m_user_id = user_id;

            TaskQueue.taskQueue.put(new GetDiagnosisElement(this, tmp));
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

    private Treatment createTreatment()
    {
        Treatment treatment = new Treatment();


        treatment.m_symptomContainor.setDetail(m_treatment.m_symptomContainor.getDetail());
        for(Symptom symptom : m_treatment.m_symptomContainor.getData())
        {
            if (symptom.bSelet)
            {
                treatment.m_symptomContainor.getData().add(symptom);
            }
        }

        treatment.m_causeContainer.setDetail(m_treatment.m_causeContainer.getDetail());
        for (Cause cause : m_treatment.m_causeContainer.getData())
        {
            if (cause.bSelet)
            {
                treatment.m_causeContainer.getData().add(cause);
            }
        }

        treatment.m_diagnosis.setDetail(m_treatment.m_diagnosis.getDetail());
        for (Diagnosis diagnosis : m_treatment.m_diagnosis.getData())
        {
            if (diagnosis.bSelet)
            {
                treatment.m_diagnosis.getData().add(diagnosis);
            }
        }

        treatment.m_solution.setDetail(m_treatment.m_solution.getDetail());
        for (Solution solution : m_treatment.m_solution.getData())
        {
            if (solution.bSelet)
            {
                treatment.m_solution.getData().add(solution);
            }
        }

        return treatment;
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

class GetDiagnosisElement  extends Task{
    Treatment old;
    Treatment       newTreatment;

    public GetDiagnosisElement(Notify notify, Treatment treatment) {
        m_notify = notify;
        old = treatment;
    }

    public Treatment getData() {
        return newTreatment;
    }

    @Override
    public void run()
    {
        try {
            newTreatment = NetFacade.getInstance().request( old, 1);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
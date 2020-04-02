package com.example.commondata;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

/**
 * 诊断方案
 */
public class Treatment implements Serializable,Cloneable{

    //public void setSymptomContainer(SymptomContainer container) {
    //    m_symptomContainor = container;
    //}
    //public SymptomContainer getSymptom() {
    //    return  m_symptomContainor;
    //}
    //
    //public void setCauseContainer(CauseContainer container) {
    //    m_causeContainer = container;
    //}

    public Treatment deepClone() {
        //将对象写入流中
        ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        ObjectOutputStream objectOutputStream = null;
        try {
            objectOutputStream = new ObjectOutputStream(outputStream);
            objectOutputStream.writeObject(this);
            ByteArrayInputStream inputStream = new ByteArrayInputStream(outputStream.toByteArray());
            ObjectInputStream objectInputStream = new ObjectInputStream(inputStream);
            return (Treatment) objectInputStream.readObject();
        } catch (IOException e) {
            e.printStackTrace();
        }catch (ClassNotFoundException e) {
            e.printStackTrace();
        }

        //从流中取出
        return null;
    }

    public void setUserId(String user_id)
    {
        m_user_id = user_id;
    }

    public enum_item m_eItem            = enum_item.SYMPTOM;
    public SymptomContainer m_symptomContainor = new SymptomContainer();
    public CauseContainer m_causeContainer = new CauseContainer();
    public DiagnosisContainer m_diagnosis = new DiagnosisContainer();
    public SolutionContainer m_solution = new SolutionContainer();
    public boolean m_bFromDoctor = false;
    public boolean m_bUploadState = false;
    public String m_strCreateTime;
    public String   end_time;
    public int      status;
    public String error = new String();
    public String m_user_id;
}

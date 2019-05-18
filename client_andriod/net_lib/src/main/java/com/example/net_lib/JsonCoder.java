package com.example.net_lib;

import com.example.commondata.*;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.ArrayList;

import static com.example.commondata.enum_item.CAUSE;
import static com.example.commondata.enum_item.DIAGNOSIS;
import static com.example.commondata.enum_item.FINISH;
import static com.example.commondata.enum_item.SOLUTION;
import static com.example.commondata.enum_item.SYMPTOM;
import static com.example.commondata.enum_item.UNKNOWN;

class JsonCoder {
    /**
     * 自诊
     */
    public JSONObject encodeSymptom(enum_item eItem, Treatment clinics, int nPageNum)throws Exception {
        JSONObject json = new JSONObject();

        String strMethod = GetMethod(eItem);
        if(null == strMethod) {
            throw new Exception("非法诊断步骤");
        }

        try{
            //设置方法名
            json.put("method", strMethod);

            //症状描述不能为空
            if(null == clinics.m_symptomContainor) {
                throw  new Exception("不允许症状描述为空!");
            }

            //构造描述对象
            JSONObject jData = encodeSymptom(clinics.m_symptomContainor);
            json.put("Symptom",jData);

            //添加原因对象
            if(null != clinics.m_causeContainer) {
                jData = encodeCause(clinics.m_causeContainer);
                json.put("Cause", jData);
            }

            //添加诊断结论对象
            if(null != clinics.m_diagnosis) {
                jData = encodeDianosis(clinics.m_diagnosis);
                json.put("Diagnosis", jData);
            }

            //添加解决方法
            if(null != clinics.m_solution)
            {
                jData = encodeSolution(clinics.m_solution);
                json.put("Solution",        jData);

                json.put("create_time",     clinics.m_strCreateTime);
                json.put("end_time",        clinics.end_time);
                json.put("status",          clinics.status);
            }

        }catch (Exception e) {
            //System.out.println("json编码异常");
            throw new Exception("json编码异常");
        }

        return  json;
    }


    public Treatment decode(JSONObject jData) throws Exception{
        Treatment clinics = new Treatment();
        try {

            int nRet = jData.getInt("return");
            if (0 != nRet) {
                String strErr = jData.getString("error");
                throw new Exception(strErr);
            }

            JSONObject data = jData.getJSONObject("data");

            String method   = jData.getString("protocol");
            clinics.m_eItem = GetMethod(method);

            //解析症状描述
            JSONObject jDiagnosisElement = data.getJSONObject("Symptom");
            if(null != jDiagnosisElement) {
                SymptomContainer symptomContainer = new SymptomContainer();

                symptomContainer.setDetail(jDiagnosisElement.getString("detail"));

                JSONArray arr = jData.getJSONArray("row");
                ArrayList<Symptom> symptoms = new ArrayList<Symptom>();
                for (int i = 0; i < arr.length(); i++) {
                    JSONObject Jsymptom = (JSONObject) arr.get(i);

                    Symptom symptom_t = new Symptom();
                    symptom_t.nID       = Jsymptom          .getInt(    "dignosis_element_id");
                    symptom_t.strDesc   = jDiagnosisElement .getString( "content");
                    symptom_t.nType     = jDiagnosisElement .getInt(    "type");
                    symptom_t.nFeedback = jDiagnosisElement .getInt(    "feedback");

                    symptoms.add(symptom_t);
                }
                symptomContainer.add(symptoms);
                clinics.m_symptomContainor = symptomContainer;
            }

            //解析原因推测
            jDiagnosisElement = data.getJSONObject("Cause");
            if(null != jDiagnosisElement) {
                CauseContainer causeContainer = new CauseContainer();

                causeContainer.setDetail(jDiagnosisElement.getString("detail"));

                JSONArray arr = jData.getJSONArray("row");
                ArrayList<Cause> causes = new ArrayList<Cause>();
                for (int i = 0; i < arr.length(); i++) {
                    JSONObject Jsymptom = (JSONObject) arr.get(i);

                    Cause cause = new Cause();
                    cause.nID       = Jsymptom              .getInt(    "dignosis_element_id");
                    cause.strDesc   = jDiagnosisElement     .getString( "content");
                    cause.nSuspect     = jDiagnosisElement  .getInt(    "suspect");
                    cause.nFeedback = jDiagnosisElement     .getInt(    "feedback");

                    causes.add(cause);
                }

                causeContainer.m_lstItem    = causes;
                clinics.m_causeContainer    = causeContainer;
            }

            //解析诊断结论
            jDiagnosisElement = data.getJSONObject("Diagnosis");
            if(null != jDiagnosisElement) {
                DiagnosisContainer diagnosisContainer = new DiagnosisContainer();

                diagnosisContainer.setDetail(jDiagnosisElement.getString("detail"));

                JSONArray arr = jData.getJSONArray("row");
                ArrayList<Diagnosis> diagnoses = new ArrayList<Diagnosis>();
                for (int i = 0; i < arr.length(); i++) {
                    JSONObject Jsymptom     = (JSONObject) arr.get(i);
                    Diagnosis diagnosis     = new Diagnosis();
                    diagnosis.nID           = Jsymptom          .getInt(    "dignosis_element_id");
                    diagnosis.strDesc       = jDiagnosisElement .getString( "content");
                    diagnosis.fProbility    = jDiagnosisElement .getInt(    "probilatity");
                    diagnosis.nIndex        = jDiagnosisElement .getInt(    "degree");

                    diagnoses.add(diagnosis);
                }

                diagnosisContainer.add(diagnoses);
                clinics.m_diagnosis = diagnosisContainer;
            }

            //解析解决方法
            jDiagnosisElement = data.getJSONObject("Solution");
            if(null != jDiagnosisElement) {
                SolutionContainer solutionContainer = new SolutionContainer();

                solutionContainer.setDetail(jDiagnosisElement.getString("detail"));

                JSONArray arr = jData.getJSONArray("row");
                ArrayList<Solution> solutions = new ArrayList<Solution>();
                for (int i = 0; i < arr.length(); i++) {
                    JSONObject Jsymptom = (JSONObject) arr.get(i);

                    Solution solution   = new Solution();
                    solution.nID        = Jsymptom          .getInt(    "dignosis_element_id");
                    solution.strDesc    = jDiagnosisElement .getString( "content");

                    solutions.add(solution);
                }

                solutionContainer.m_lstItem = solutions;
                clinics.m_solution          = solutionContainer;
            }

            //解析生成时间
            clinics.m_strCreateTime = data.getString("create_time");

            //结束时间
            clinics.end_time        = data.getString("end_time");

            //执行状态
            clinics.status          = data.getInt("status");

        }catch (Exception e)
        {
           throw e;
        }

        return  clinics;
    }


     private String GetMethod(enum_item eItem) {
        switch(eItem)
        {
            case FINISH:    return "Finish";
            case SOLUTION:  return "GetSolution";
            case DIAGNOSIS: return "GetDiagnosis";
            case SYMPTOM:   return "GetSymptoms";
            case CAUSE:     return "GetCause";
            default:        return null;
        }
     }

     private enum_item GetMethod(String method) {
        if          ("GetSymptom"   == method)    {
            return SYMPTOM;
        }else if    ("GetCause"     == method)      {
            return CAUSE;
        }else if    ("GetSolution"  == method)   {
            return SOLUTION;
        }else if    ("GetDiagnosis" == method)  {
            return DIAGNOSIS;
        }else {
            return UNKNOWN;
        }
     }

     private JSONObject encodeSymptom(SymptomContainer symptomContainer) throws Exception{
        JSONObject jData = new JSONObject();

        jData.put("detail", symptomContainer.getDetail());

        JSONArray ja = new JSONArray();
        for (int i = 0; i < symptomContainer.getData().size(); i++) {
            Symptom symptom = symptomContainer.getData().get(i);

            JSONObject jRow = new JSONObject();
            jRow.put("dignosis_element_id", symptom.nID);

            ja.put(i, jRow);
        }

        jData.put("row", ja);

        return jData;
     }

     private JSONObject encodeCause(CauseContainer cause) throws Exception{
         JSONObject jData = new JSONObject();

         jData.put("detail", cause.getDetail());

         JSONArray ja = new JSONArray();
         for (int i = 0; i < cause.getData().size(); i++)
         {
             Cause symptom = cause.getData().get(i);

             JSONObject jRow = new JSONObject();
             jRow.put("dignosis_element_id", symptom.nID);

             ja.put(i, jRow);
         }

         jData.put("row", ja);

         return jData;
     }

    private JSONObject encodeDianosis(DiagnosisContainer diagnosisContainer) throws Exception {
        JSONObject jData = new JSONObject();

        jData.put("detail", diagnosisContainer.getDetail());

        JSONArray ja = new JSONArray();
        for (int i = 0; i < diagnosisContainer.getData().size(); i++) {
            Diagnosis symptom = diagnosisContainer.getData().get(i);

            JSONObject jRow = new JSONObject();
            jRow.put("dignosis_element_id", symptom.nID);

            ja.put(i, jRow);
        }

        jData.put("row", ja);

        return jData;
    }

     private JSONObject encodeSolution(SolutionContainer solutionContainer) throws Exception{
         JSONObject jData = new JSONObject();

         jData.put("detail", solutionContainer.getDetail());

         JSONArray ja = new JSONArray();
         for (int i = 0; i < solutionContainer.getData().size(); i++) {
             Solution symptom = solutionContainer.getData().get(i);

             JSONObject jRow = new JSONObject();
             jRow.put("dignosis_element_id", symptom.nID);

             ja.put(i, jRow);
         }

         jData.put("row", ja);

         return jData;
     }
}

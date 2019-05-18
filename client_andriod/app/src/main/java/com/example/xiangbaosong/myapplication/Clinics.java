package com.example.xiangbaosong.myapplication;

import android.content.Intent;
import android.os.StrictMode;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.*;
import android.widget.*;

import com.example.commondata.Cause;
import com.example.commondata.CauseContainer;
import com.example.commondata.Diagnosis;
import com.example.commondata.DiagnosisContainer;
import com.example.commondata.Solution;
import com.example.commondata.SolutionContainer;
import com.example.commondata.Symptom;
import com.example.model_lib.*;
import com.example.commondata.*;
import com.example.threadlib.Notify;

import java.util.ArrayList;
import java.util.HashMap;

import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;
//import  com.example.pichurelibrary.*;

import java.util.List;

//import com.example.pichurelibrary.imageloader.GlideImageLoader;
//import com.example.pichurelibrary.loader.ImageLoader;
//import com.example.pichurelibrary.view.*;

public class Clinics extends AppCompatActivity {

    //private ListView listView;
    //private ArrayAdapter<String> adapter;
    //作为数据源来使用
    private ArrayList<HashMap<String, Object>>  data        = null;
    private model_facade                        model       = model_facade.getinstance();
    private Button                              mButtonGetValue; // 获取数据按钮
    private ListView                            mListView;// 数据展示列表
    private CheckboxAdapter                     listItemAdapter; // ListView数据展示适配器
    private ArrayList<HashMap<String, Object>>  listData;// ListView展示数据源
    private enum_item m_eItem     = enum_item.SYMPTOM ;
    View                                        lastBtn     = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //让软键盘将屏幕上推十分有用
        getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_PAN);

        setContentView(R.layout.activity_clinics);
        if (android.os.Build.VERSION.SDK_INT > 9) {
            StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
            StrictMode.setThreadPolicy(policy);
        }

        initView();
        registerListener();
        //listData = new ArrayList<HashMap<String, Object>>();
        //initListViewData(listData);
        loadData();


        //设置好异步通知
        model.getClinics().setNotify(m_notify);


       //ImagePicker imagePicker = ImagePicker.getInstance();
       //imagePicker.setImageLoader(new GlideImageLoader());   //设置图片加载器
       //imagePicker.setShowCamera(true);  //显示拍照按钮
       //imagePicker.setCrop(true);        //允许裁剪（单选才有效）
       //imagePicker.setSaveRectangle(true); //是否按矩形区域保存
       //imagePicker.setSelectLimit(9);    //选中数量限制
       //imagePicker.setStyle(CropImageView.Style.RECTANGLE);  //裁剪框的形状
       //imagePicker.setFocusWidth(800);   //裁剪框的宽度。单位像素（圆形自动取宽高最小值）
       //imagePicker.setFocusHeight(800);  //裁剪框的高度。单位像素（圆形自动取宽高最小值）
       //imagePicker.setOutPutX(1000);//保存文件的宽度。单位像素
       //imagePicker.setOutPutY(1000);//保存文件的高度。单位像素

        //getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_RESIZE);
        //setContentView(R.layout.activity_search_position);

        //添加列表框的适配器
        //data = model.GetSymptom(" ", 0, 9);
        //adapter = new ArrayAdapter<String>(Clinics.this, android.R.layout.simple_list_item_multiple_choice, data);
//
        //listView = (ListView)findViewById(R.id.clinics_treatment_lst);
        //listView.setChoiceMode(ListView.CHOICE_MODE_MULTIPLE);//设置多选操作
        //listView.setAdapter(adapter);
//
        //listView.setOnItemClickListener(new AdapterView.OnItemClickListener()
        //{
        //    @Override
        //    public void onItemClick(AdapterView<?> parent, View view, int position, long id)
        //    {
        //        String str = (String) ((TextView) view).getText();
        //        //str就是textView上的内容
        //    }
        //});
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        //getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }


    public void button_click(View v){

        //1.获取到按钮id
        int nID = v.getId();

        Intent intent = null;


        switch (nID)
        {
            //自诊历史
           case R.id.button_history:
           {
               intent = new Intent(this,ClinicsRecordHistory.class);
           }break;

            //添加记录
            case R.id.button_add:
            {
                intent = new Intent(this,ClinicsRecordAdd.class);
            }break;

            //上传图片
            case R.id.clinics_upload:
            {
                //int n = 9;
                intent = new Intent(this, PhotoGet.class);
            }break;
        }
        //弹出子页面,相当于mfc中的子窗口弹出
        if(intent != null){
            startActivity(intent);
        }
    }

    public void button_clinics(View v) {

        //获取文本框中的描述
        String strInput = new String();
        EditText editText1 =(EditText)findViewById(R.id.clinics_input);
        strInput = editText1.getText().toString();

        ArrayList<HashMap<String, Object>> data = new ArrayList<HashMap<String, Object>>();
        HashMap<Integer, HashMap<String, Object>> state = null;
        boolean bGetMore;

        //按钮颜色跟踪
        if ( null != lastBtn) {
            lastBtn.setBackgroundColor(0xff00ddff);
        }
        v.setBackgroundColor(0xffffbb33);
        lastBtn = v;


        try {

            saveViewData(m_eItem);

            //1.获取到按钮id
            int nID = v.getId();
            switch (nID)
            {
                //症状描述
                case R.id.button_symptom:
                {
                    m_eItem = enum_item.SYMPTOM;
                }break;

                //原因推测
                case R.id.button_cause:
                {
                    m_eItem = enum_item.CAUSE;
                }break;

                //诊断结论
                case R.id.button_diagnosis:
                {
                    m_eItem = enum_item.DIAGNOSIS;
                }break;

                //解决方法
                case R.id.button_solution:
                {
                    m_eItem = enum_item.SOLUTION;
                }break;

                //完成诊断
                case R.id.button_cilinics_finish:
                {
                    m_eItem = enum_item.SYMPTOM;
                    strInput = "";
                }

                //更新界面显示
            }

            model.RequestDiagnosis(m_eItem);

            //将分解后的描述动态生成复选框
            //adapter.clear();
            //adapter.addAll(data);
            //adapter.notifyDataSetChanged();

            //editText1.setText(strInput);
            listItemAdapter.clear();
            //listItemAdapter.addAll(data);
            listItemAdapter.notifyDataSetChanged();
        }
        catch (Exception e) {
            System.out.println(e.getCause());
        }

    }

    /**
     * 初始化布局
     */
    private void initView() {
        EditText editText1 =(EditText)findViewById(R.id.clinics_input);
        editText1.setText("肚子不舒服;每天晚上凌晨会痛;不是很痛;侧身睡就会感觉舒服点;已经几年了!");

       mButtonGetValue = (Button) findViewById(R.id.button_symptom);
        mListView = (ListView) findViewById(R.id.clinics_treatment_lst);
    }

    private void registerListener() {
       mButtonGetValue.setOnClickListener(new OnClickListenerImpl());
    }

    /**
     * 加载数据
     */
    private void loadData() {
        listItemAdapter = new CheckboxAdapter(this);
        mListView.setAdapter(listItemAdapter);
    }

    /**
     * 初始化ListView数据源
     *
     * @param listData
     */
    private void initListViewData(SymptomContainer symptomContainer, ArrayList<HashMap<String, Object>> listData) {
        if (listData == null)
            listData = new ArrayList<HashMap<String, Object>>();

        for (Symptom symptom: symptomContainer.getData()) {
            HashMap<String, Object> map = new HashMap<String, Object>();
            map.put("friend_image", R.drawable.images);
            map.put("friend_username", createItemText(symptom));
            map.put("friend_id", symptom.nID);
            map.put("selected", symptom.bSelet);

            // 向容器添加数据
            listData.add(map);
        }
    }

    private void initListViewData(CauseContainer causes, ArrayList<HashMap<String, Object>> listData) {
        if (listData == null)
            listData = new ArrayList<HashMap<String, Object>>();

        ArrayList<Cause> lcauses = causes.getData();
        for (Cause cause: lcauses) {
            HashMap<String, Object> map = new HashMap<String, Object>();
            map.put("friend_image", R.drawable.images);
            map.put("friend_username", createItemText(cause));
            map.put("friend_id", cause.nID);
            map.put("selected", cause.bSelet);

            // 向容器添加数据
            listData.add(map);
        }
    }

    private void initListViewData(DiagnosisContainer dignosises, ArrayList<HashMap<String, Object>> listData) {
        if (listData == null)
            listData = new ArrayList<HashMap<String, Object>>();

        ArrayList<Diagnosis> lisDiagnosis = dignosises.getData();
        for (Diagnosis diagnosis: lisDiagnosis) {
            HashMap<String, Object> map = new HashMap<String, Object>();
            map.put("friend_image", R.drawable.images);
            map.put("friend_username", createItemText(diagnosis));
            map.put("friend_id", diagnosis.nID);
            map.put("selected", diagnosis.bSelet);

            // 向容器添加数据
            listData.add(map);
        }
    }

    private void initListViewData(SolutionContainer soltions, ArrayList<HashMap<String, Object>> listData) {
        if (listData == null)
            listData = new ArrayList<HashMap<String, Object>>();

        ArrayList<Solution> lstSolution = soltions.getData();
        for (Solution solution: lstSolution) {
            HashMap<String, Object> map = new HashMap<String, Object>();
            map.put("friend_image", R.drawable.images);
            map.put("friend_username", createItemText(solution));
            map.put("friend_id", solution.nID);
            map.put("selected", solution.bSelet);

            // 向容器添加数据
            listData.add(map);
        }
    }

    // 获取Checkbox值的点击事件
    private class OnClickListenerImpl implements OnClickListener {

        @Override
        public void onClick(View v) {
            HashMap<Integer, HashMap<String, Object>> state = listItemAdapter.state;
            String options = "选中数据:";

            for (int j = 0; j < listItemAdapter.getCount(); j++) {
                System.out.println("state.get(" + j + ")==" + state.get(j));
                if (state.get(j) != null) {
                    @SuppressWarnings("unchecked")
                    HashMap<String, Object> map = (HashMap<String, Object>) listItemAdapter.getItem(j);
                    String username = map.get("friend_username").toString();
                    String id = map.get("friend_id").toString();
                    options += "\n" + id + "." + username;
                }
            }
            // 展示数据
            Toast.makeText(getApplicationContext(), options, Toast.LENGTH_LONG).show();
        }
    }

    private String createItemText(Cause cause) {
        String strText = cause.strDesc + " ";
        strText += cause.nFeedback + "人反馈 ";
        switch (cause.nDataSrc)
        {
            case CLINICS:
             {
                 strText += cause.nSuspect + "怀疑";
             }break;

            case DIETE:
            {
                strText +=  "饮食";
            }break;

            case SPORTS:
            {
                strText +=  "运动";
            }break;

            case SLEEP:
            {
                strText += "睡眠";
            }break;

            case PSYCHONOGY:
            {
                strText += "心理";
            }break;

            case EVIRONMENT:
            {
                strText += "环境";
            }break;
        }

        return  strText;
    }

    private String createItemText(Diagnosis diagnosis) {
        String strText = diagnosis.strDesc + " ";
        strText += "概率" + diagnosis.fProbility*100 + "% ";
        strText += "严重程度" + diagnosis.nIndex;

        return  strText;
    }

    private String createItemText(Solution solution) {
        String strText = solution.strDesc + " ";
        strText += "效果" + solution.fValue + "% ";
        strText += "成本" + solution.fCost + "元";

        return  strText;
    }

    private String createItemText(Symptom symptom) {
        String strText = symptom.strDesc;
        if (symptom.nType == 1) {
            strText += " ";
        }else {
            strText += "  ";
        }

        strText += symptom.nFeedback + "人反馈";

        return  strText;
    }

    void saveViewData(enum_item item) {
        //保存文本输入
        HashMap<Integer, HashMap<String, Object>> state = null;

        EditText editText1 =(EditText)findViewById(R.id.clinics_input);
        String strInput = editText1.getText().toString();



        //保存选择状态
        switch (item)
        {
            case SYMPTOM:{
                //获取症状描述容器对象
                SymptomContainer symptomContainer = model.GetSymptom();

                //添加详细描述
                symptomContainer.setDetail(strInput);

                //标记选择的选项
                ArrayList<Symptom> symptomsData = symptomContainer.getData();
                state = listItemAdapter.state;
                for (int j = 0; j < listItemAdapter.getCount(); j++) {
                    symptomsData.get(j).bSelet = state.get(j) != null ? true : false;
                }

                //提交请求
               // model.RequestSymptom();
            }break;

            case CAUSE:{

                model.getCause(false).setDetail(strInput);

                ArrayList<Cause> causes = model.getCause(false).getData();
                state = listItemAdapter.state;
                for (int j = 0; j < listItemAdapter.getCount(); j++) {
                    causes.get(j).bSelet = state.get(j) != null ? true : false;
                }

            }break;
            case DIAGNOSIS:{

               // model.getDiagnosis(strInput);

                ArrayList<Cause> causes = model.getCause(false).getData();
                state = listItemAdapter.state;
                for (int j = 0; j < listItemAdapter.getCount(); j++) {
                    causes.get(j).bSelet = state.get(j) != null ? true : false;
                }
            }break;
            case SOLUTION:{

                model.getSolution(false).setDetail(strInput);

                ArrayList<Diagnosis> diagnoses = model.getDiagnosis(false).getData();
                state = listItemAdapter.state;
                for (int j = 0; j < listItemAdapter.getCount(); j++) {
                    diagnoses.get(j).bSelet = state.get(j) != null ? true : false;
                }
            }break;
            case FINISH:{

            }break;
        }


    }

    class NetUpdate extends Notify {
        public synchronized void  notify(Notify notify) {
            model_clinics clinics = (model_clinics) notify;

            Treatment treatment = clinics.getTreatment();

            try {
                //1.确定是哪个步骤的更
                switch (treatment.m_eItem) {
                    case SYMPTOM: {
                        //3.将数据封装成显示的格式
                        initListViewData(model.GetSymptom(), data);
                    }
                    break;
                    case CAUSE: {
                        initListViewData(treatment.m_causeContainer, data);
                    }
                    break;
                    case DIAGNOSIS: {
                        initListViewData(treatment.m_diagnosis, data);
                    }
                    break;
                    case SOLUTION: {
                        initListViewData(treatment.m_solution, data);
                    }
                    break;
                    case FINISH: {

                    }
                    break;
                }

                //4.执行更新
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        try{
                            //更新UI
                            listItemAdapter.clear();
                            listItemAdapter.addAll(data);
                            listItemAdapter.notifyDataSetChanged();
                        }catch (Exception e) {
                            e.printStackTrace();
                        }

                    }
                });
            }catch (Exception e) {
                e.printStackTrace();
            }

        }
    };

    private NetUpdate m_notify = new NetUpdate();
}

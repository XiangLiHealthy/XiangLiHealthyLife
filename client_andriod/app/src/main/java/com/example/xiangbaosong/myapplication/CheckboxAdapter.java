package com.example.xiangbaosong.myapplication;

import java.util.ArrayList;
import java.util.HashMap;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.ImageView;
import android.widget.TextView;

public class CheckboxAdapter extends BaseAdapter {
    private Context context;
    private ArrayList<HashMap<String, Object>> listData;
    //checkbox选中的数据
    HashMap<Integer, HashMap<String, Object>> state = new HashMap<Integer, HashMap<String, Object>>();

    public CheckboxAdapter(Context context){
        this.context = context;
    }

    // 构造方法初始化数据
    public CheckboxAdapter(Context context, ArrayList<HashMap<String, Object>> listData) {
        this.context = context;
        this.listData = listData;
    }

    //清理数据
    public  void clear() {
        if (null == listData) {
            return;
        }

        state.clear();
        this.listData.clear();
    }

    //添加数据
    public void addAll(ArrayList<HashMap<String, Object>> listData) {
        if(null == this.listData) {
            this.listData = listData;
        }else
        {
            this.listData.addAll(listData);
        }

        initSelect();
    }

    public void initSelect() {
        int position = 0;
        for (HashMap<String, Object> obj: listData) {
            if (true == ((boolean)obj.get("selected"))) {
                state.put(position, listData.get(position));
            }

            position++;
        }
    }

    @Override
    public int getCount() {
        return (listData != null && !listData.isEmpty())?listData.size():0;
    }

    @Override
    public Object getItem(int position) {
        return listData.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(final int position, View convertView, ViewGroup parent) {

        LayoutInflater mInflater = LayoutInflater.from(context);
        convertView = mInflater.inflate(R.layout.item_list, null);
        ImageView image = (ImageView) convertView.findViewById(R.id.friend_image);
        final HashMap<String, Object> viewData = listData.get(position);
        image.setBackgroundResource((Integer) viewData.get("friend_image"));
        TextView username = (TextView) convertView.findViewById(R.id.friend_username);
        username.setText((String) viewData.get("friend_username"));
        //TextView id = (TextView) convertView.findViewById(R.id.friend_id);
        //id.setText((String) viewData.get("friend_id"));

        //在被选择的位置插入 复制一个对象;
        CheckBox check = (CheckBox) convertView.findViewById(R.id.selected);
        check.setChecked((state.get(position) == null ? false : true));

        check.setOnCheckedChangeListener(new OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    state.put(position, viewData);
                } else {
                    state.remove(position);
                }

                //HashMap<String, Object> item = listData.get(position);
                //item.keySet("selected",isChecked);
            }
        });



        return convertView;
    }
}
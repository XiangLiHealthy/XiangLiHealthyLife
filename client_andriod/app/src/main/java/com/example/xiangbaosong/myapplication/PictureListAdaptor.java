package com.example.xiangbaosong.myapplication;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.RadioButton;

import java.io.File;
import java.util.ArrayList;

public class PictureListAdaptor extends BaseAdapter {
    private Context context;
    ArrayList names = new ArrayList();
    ArrayList descs = null;
    ArrayList fileNames = new ArrayList();

    String m_Dir = new String();

    class Picture {
        String strPath;
        Bitmap newbm;
        boolean bSel;
    }

    private ArrayList<ArrayList<Picture>> m_lstData = new ArrayList<ArrayList<Picture>>();
    static final int COL = 3;

    PictureListAdaptor(Context ctxt) {
        context = ctxt;
    }
    void setDir(String strDir) {
        m_Dir = strDir;
    }


    public void showMore() {

        try{
            //Cursor cursor = context.getContentResolver().query( MediaStore.Images.Media.EXTERNAL_CONTENT_URI, null, null, null, null);
            //while (cursor.moveToNext()) {
            //    //获取图片的名称
            //    String name = cursor.getString(cursor.getColumnIndex(MediaStore.Images.Media.DISPLAY_NAME));
            //    //获取图片的生成日期
            //    byte[] data = cursor.getBlob(cursor.getColumnIndex(MediaStore.Images.Media.DATA));
            //    //获取图片的详细信息
            //    String desc = cursor.getString(cursor.getColumnIndex(MediaStore.Images.Media.DESCRIPTION));
            //    names.add(name); descs.add(desc);
            //    fileNames.add(new String(data, 0, data.length - 1));
            //}


            //return result ;

            //得到默认数量的图片
            // 得到该路径文件夹下所有的文件
            File fileAll = new File(m_Dir);
            File[] files = fileAll.listFiles();
            if (null == files) {
                return;
            }

            // 将所有的文件存入ArrayList中,并过滤所有图片格式的文件
            //第一个位置是cache,应该过滤掉

            //for (int i = 1; i < files.length; i++)
            for (int i = 1; i < 27; i++)
            {

                //每排放三个
                ArrayList<Picture> lstPic = new ArrayList<Picture>();
                for(int j = i; j < i+COL; j++) {
                    File file = files[j];
                    Picture picName = new Picture();
                    picName.strPath = file.getPath();
                    lstPic.add(picName);
                }

                i += COL;
                //将一排的数据放到列表
                m_lstData.add(lstPic);
            }

            //更新显示
            //notifyDataSetInvalidated();
        }catch (Exception e) {
            e.getCause();
        }

    }

    @Override
    public int getCount() {
        return m_lstData.size();
    }

    @Override
    public Object getItem(int i) {
        return m_lstData.get(i);
    }

    @Override
    public long getItemId(int i) {
        return i;
    }

    @Override
    public View getView(int i, View convertView, ViewGroup viewGroup) {

        //加载自定义布局
       LayoutInflater mInflater = LayoutInflater.from(context);
       convertView = mInflater.inflate(R.layout.actctivity_picture, null);

       //对控件赋值
       final ArrayList<Picture> data = m_lstData.get(i);
       String strText;

       //获取第一个位置的图片控件
       //加载对应路径的图片到对应的位置
       //将选择状态设置为未选择
       ImageView img;
       String strPath;
       RadioButton btn;

        int width = 0;
        int height = 0;
        // 设置想要的大小

        // 得到新的图片
        if (data.size() > 0) {
           loadImage(convertView,R.id.photo_picture1,data.get(0));
           setRadio(convertView,R.id.photo_sel1,data.get(0).bSel);
       }

       //获取第二个位置的图片控件
       //加载对应路径的图片到对应的位置
       //将选择状态设置为未选择
       if (data.size() > 1) {
           loadImage(convertView,R.id.photo_picture2,data.get(1));
           setRadio(convertView,R.id.photo_sel2,data.get(1).bSel);
       }

        //获取第三个位置的图片控件
        //加载对应路径的图片到对应的位置
        //将选择状态设置为未选择
        if (data.size() > 2) {
            loadImage(convertView,R.id.photo_picture3,data.get(2));
            setRadio(convertView,R.id.photo_sel3,data.get(2).bSel);
        }

        return convertView;
    }

    private void loadImage(View convertView,int nCtrID, Picture picture) {

        try{
            ImageView img = (ImageView) convertView.findViewById(nCtrID);

            if (null == picture.newbm) {
                File file = new File(picture.strPath);
                if (file.exists()) {
                    Bitmap bm = BitmapFactory.decodeFile(picture.strPath);

                    int width = bm.getWidth();
                    int height = bm.getHeight();

                    Bitmap bitmap = BitmapFactory.decodeResource(convertView.getResources(), nCtrID);
                    int newWidth = (int) (120*2.5);
                    int newHeight = (int) (100*2.5);

                    // 计算缩放比例
                    float scaleWidth = ((float) newWidth) / width;
                    float scaleHeight = ((float) newHeight) / height;

                    // 取得想要缩放的matrix参数
                    Matrix matrix = new Matrix();
                    matrix.postScale(scaleWidth, scaleHeight);
                    picture.newbm = Bitmap.createBitmap(bm, 0, 0, width, height, matrix, true);
                }
            }
            img.setImageBitmap(picture.newbm);

        }catch (Exception e) {
            e.getCause();
        }
    }

    private void setRadio(View convertView,int nID, boolean bSel) {
        RadioButton btn = (RadioButton) convertView.findViewById(nID);
        btn.setSelected(bSel);
    }
}

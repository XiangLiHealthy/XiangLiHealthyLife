package com.example.xiangbaosong.myapplication;

import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import android.app.Activity;
import android.content.ContentResolver;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.media.ThumbnailUtils;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;


public class PhotoGet extends AppCompatActivity {
    //@Override
   //protected void onCreate(Bundle savedInstanceState) {
   //    super.onCreate(savedInstanceState);
   //    setContentView(R.layout.activity_photo_get);
   //}

    private ListView m_lstView;
    private Button btnPhone;
    private ImageView imageView;
    private ArrayList<String> m_dirs;


    private static final String IMAGE_UNSPECIFIED = "image/*";
    private final int IMAGE_CODE = 0; // 这里的IMAGE_CODE是自己任意定义的

    private TextView tv;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // TODO Auto-generated method stub
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_photo_get);

        //关联到自定义照片视图
        m_lstView = (ListView) findViewById(R.id.picture_list);
        PictureListAdaptor adaptor = new PictureListAdaptor(this);

        m_dirs = getDICMDir();
        if (null != m_dirs && m_dirs.size() > 0) {
            adaptor.setDir(m_dirs.get(0));
            adaptor.showMore();

            String strPath = m_dirs.get(0);
            strPath.trim();
            String fileName = strPath.substring(strPath.lastIndexOf("/")+1);
            fileName = " < " + fileName;

            Button btn =  findViewById(R.id.button7);
            btn.setText(fileName);
        }

        m_lstView.setAdapter(adaptor);

        //btnPhone = (Button) findViewById(R.id.btnPhone);
        //imageView = (ImageView) findViewById(R.id.imageView);
//
        //tv = (TextView) findViewById(R.id.img_path);
        //btnPhone.setOnClickListener(new OnClickListener() {
//
        //    @Override
        //    public void onClick(View v) {
        //        // TODO Auto-generated method stub
//
        //        setImage1(); // 魅族显示风格：最新，照片，图库；华为：包含有相片的一组目录，
        //        // 小米：选择要使用的应用，最后没有结果
//
        //        // setImage(); //魅族显示风格：图库，文件选择(图片文件) ；华为：最近的照片 小米：选择要使用的应用，最后没有结果
//
        //    }
//
        //});

    }

    private void setImage1() {
        Intent intent = new Intent(Intent.ACTION_PICK, null);
        intent.setDataAndType(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, IMAGE_UNSPECIFIED);
        startActivityForResult(intent, IMAGE_CODE);
    }

    private void setImage() {
        // TODO Auto-generated method stub
        // 使用intent调用系统提供的相册功能，使用startActivityForResult是为了获取用户选择的图片

        Intent getAlbum = new Intent(Intent.ACTION_GET_CONTENT);

        getAlbum.setType(IMAGE_UNSPECIFIED);

        startActivityForResult(getAlbum, IMAGE_CODE);

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        // TODO Auto-generated method stub


        Bitmap bm = null;

        // 外界的程序访问ContentProvider所提供数据 可以通过ContentResolver接口

        ContentResolver resolver = getContentResolver();

        if (requestCode == IMAGE_CODE) {

            try {

                Uri originalUri = data.getData(); // 获得图片的uri

                bm = MediaStore.Images.Media.getBitmap(resolver, originalUri);

                imageView.setImageBitmap(ThumbnailUtils.extractThumbnail(bm, 100, 100));  //使用系统的一个工具类，参数列表为 Bitmap Width,Height  这里使用压缩后显示，否则在华为手机上ImageView 没有显示
                // 显得到bitmap图片
                // imageView.setImageBitmap(bm);

                String[] proj = { MediaStore.Images.Media.DATA };

                // 好像是android多媒体数据库的封装接口，具体的看Android文档
                Cursor cursor = managedQuery(originalUri, proj, null, null, null);

                // 按我个人理解 这个是获得用户选择的图片的索引值
                int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
                // 将光标移至开头 ，这个很重要，不小心很容易引起越界
                cursor.moveToFirst();
                // 最后根据索引值获取图片路径
                String path = cursor.getString(column_index);
                tv.setText(path);
            } catch (IOException e) {
                Log.e("TAG-->Error", e.toString());

            }

            finally {
                return;
            }
        }

        super.onActivityResult(requestCode, resultCode, data);

    }

    private  ArrayList<String> getDICMDir() {
        try{
            //得到默认数量的图片
            // 得到该路径文件夹下所有的文件
            String m_Dir = Environment.getExternalStorageDirectory() +
                    File.separator + Environment.DIRECTORY_DCIM + File.separator;
            File fileAll = new File(m_Dir);
            File[] files = fileAll.listFiles();
            if (null == files) {
                return null;
            }

            // 将所有的文件存入ArrayList中,并过滤所有图片格式的文件
            ArrayList<String> lstPic = new ArrayList<String>();
            for (int i = 0; i < files.length; i++) {
                    File file = files[i];
                    String strPath = file.getPath();
                    strPath.trim();
                    String fileName = strPath.substring(strPath.lastIndexOf("/")+1);

                    //点号开头的文件夹给过滤掉
                if (strPath.indexOf(".") == -1) {
                    lstPic.add(strPath);
                }
            }

            return lstPic;
            //更新显示
        }catch (Exception e) {
            e.getCause();
            return  null;
        }
    }
}

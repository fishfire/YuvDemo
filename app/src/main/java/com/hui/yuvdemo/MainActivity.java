package com.hui.yuvdemo;

import android.Manifest;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Toast;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

public class MainActivity extends AppCompatActivity implements ActivityCompat.OnRequestPermissionsResultCallback, View.OnClickListener {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private ImageView imageView1;
    private ImageView imageView2;
    private EditText mEditText1;
    public static final int ARGB = 1;
    public static final int ABGR = 2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        imageView1 = findViewById(R.id.iv_image1);
        imageView2 = findViewById(R.id.iv_image2);
        mEditText1 = findViewById(R.id.et_coordinates);
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                String writeExternalStorage = Manifest.permission.WRITE_EXTERNAL_STORAGE;
                ActivityCompat.requestPermissions(MainActivity.this, new String[]{writeExternalStorage}, 1);

            }
        });

        // Example of a call to a native method
        Button tv = findViewById(R.id.sample_text);
        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {


                Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.drawable.th);
                imageView1.setImageBitmap(bitmap);
                ByteBuffer allocate = ByteBuffer.allocate(bitmap.getByteCount());
                bitmap.copyPixelsToBuffer(allocate);
                byte[] array = allocate.array();

                int width = bitmap.getWidth();
                int height = bitmap.getHeight();
                Bitmap bitmap2 = Bitmap.createBitmap(2 * width, 2 * height, Bitmap.Config.ARGB_8888);
                int width1 = bitmap2.getWidth();
                int height1 = bitmap2.getHeight();
                byte[] dst = nativeARGBScale(array, width, height, width1, height1);
                if (dst != null) {
                    byte[] i420 = nativeABGRToI420(array, width1, height1);
                    try {
                        FileOutputStream fileOutputStream = new FileOutputStream("/sdcard/1.yuv");
                        fileOutputStream.write(i420);
                        fileOutputStream.flush();
                        fileOutputStream.close();
                    } catch (FileNotFoundException e) {
                        e.printStackTrace();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    bitmap2.copyPixelsFromBuffer(ByteBuffer.wrap(dst));
                    imageView2.setImageBitmap(bitmap2);
                    FileOutputStream fileOutputStream = null;
                    try {
                        File file = new File("/sdcard/1.jpg");
                        if (file.exists()) {
                            file.delete();
                        }
                        fileOutputStream = new FileOutputStream("/sdcard/1.jpg");
                        bitmap2.compress(Bitmap.CompressFormat.JPEG, 100, fileOutputStream);
                        fileOutputStream.close();
                    } catch (FileNotFoundException e) {
                        e.printStackTrace();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }

                }

            }
        });
        tv.setText(stringFromJNI());
        findViewById(R.id.bt_clip).setOnClickListener(this);
    }


    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native byte[] nativeARGBScale(byte[] array, int width, int height, int dst_width, int dst_height);

    /**
     * @param array  格式为ABGR
     * @param width
     * @param height
     * @return
     */
    public native byte[] nativeABGRToI420(byte[] array, int width, int height);

    public native byte[] nativeI420CropToARGB(byte[] array, int width, int height, int dst_width, int dst_height, int clip_x, int clip_y, int clip_width, int clip_height);

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.bt_clip:
//                final Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.drawable.th);
//                imageView1.setImageBitmap(bitmap);
//                ByteBuffer dst = ByteBuffer.allocate(bitmap.getByteCount());
//                bitmap.copyPixelsToBuffer(dst);
//
//                int width = bitmap.getWidth();
//                int height = bitmap.getHeight();
//                Log.w("MainActivity","w,h = "+ width + ","+height);
//                final byte[] bytes = nativeI420CropToARGB(dst.array(), bitmap.getWidth(), bitmap.getHeight(), width, height, 60, 58, 200, 400);
//                Bitmap bm = Bitmap.createBitmap(width,width, Bitmap.Config.ARGB_8888);
//                bm.copyPixelsFromBuffer(ByteBuffer.wrap(bytes));
//                imageView2.setImageBitmap(bm);
//                try {
//                    FileOutputStream fos = new FileOutputStream("/sdcard/1.jpg");
//                    bm.compress(Bitmap.CompressFormat.JPEG,100,fos);
//                    fos.flush();
//                    fos.close();
//                } catch (FileNotFoundException e) {
//                    e.printStackTrace();
//                } catch (IOException e) {
//                    e.printStackTrace();
//                }
                final String text = mEditText1.getText().toString();
                int crop_x = 0;
                int crop_y = 0;
                int dst_width = 300;
                int dst_height = 300;
                if (!TextUtils.isEmpty(text)){
                    final String[] split = text.split("-");
                    crop_x = Integer.parseInt(split[0]);
                    crop_y = Integer.parseInt(split[1]);
                    if (split.length > 3) {
                        dst_width = Integer.parseInt(split[2]);
                        dst_height = dst_width;
                    }
                }


                final InputStream inputStream = getResources().openRawResource(R.raw.haha);
                try {
                    byte[] bytes = new byte[inputStream.available()];
                    inputStream.read(bytes);
                    inputStream.close();
                    byte[] b = nativeI420CropToARGB(bytes,1280,720,dst_width,dst_height,crop_x,crop_y,dst_width,dst_height);
                    Bitmap bm = Bitmap.createBitmap(dst_width,dst_height, Bitmap.Config.ARGB_8888);
                    bm.copyPixelsFromBuffer(ByteBuffer.wrap(b));
                    imageView2.setImageBitmap(bm);

                } catch (IOException e) {
                    e.printStackTrace();
                }

                break;
        }

    }
}

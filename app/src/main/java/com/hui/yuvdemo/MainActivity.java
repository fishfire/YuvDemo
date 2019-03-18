package com.hui.yuvdemo;

import android.Manifest;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;

public class MainActivity extends AppCompatActivity implements ActivityCompat.OnRequestPermissionsResultCallback {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private ImageView imageView1;
    private ImageView imageView2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        imageView1 = findViewById(R.id.iv_image1);
        imageView2 = findViewById(R.id.iv_image2);
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
                Bitmap bitmap2 = Bitmap.createBitmap(width / 2, height / 2, Bitmap.Config.ARGB_8888);
                byte[] dst = nativeARGBScale(array, width, height, bitmap2.getWidth(), bitmap2.getHeight());
                if (dst != null) {
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
    }


    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native byte[] nativeARGBScale(byte[] array, int width, int height, int dst_width, int dst_height);
}

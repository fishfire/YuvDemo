//
// Created by 刘会 on 2019/3/17.
//

#ifndef YUVDEMO_YUVJNI_H
#define YUVDEMO_YUVJNI_H

#include <jni.h>
#include <string>
#include <cstdlib>
#include "libyuv/include/libyuv.h"

class YuvJni {
private:
    //分别用来存储1420，1420缩放，I420旋转和镜像的数据
    jbyte *Src_i420_data;
    jbyte *Src_i420_data_scale;
    jbyte *Src_i420_data_rotate;
public:
    void JNI_OnUnload(JavaVM *jvm, void *reserved);
    void init(jint width, jint height, jint dst_width, jint dst_height)
    void scaleI420(jbyte *src_i420_data, jint width, jint height, jbyte *dst_i420_data, jint dst_width,
                   jint dst_height, jint mode);
    void rotateI420(jbyte *src_i420_data, jint width, jint height, jbyte *dst_i420_data, jint degree);
    void nv21ToI420(jbyte *src_nv21_data, jint width, jint height, jbyte *src_i420_data);
    void mirrorI420(jbyte *src_i420_data, jint width, jint height, jbyte *dst_i420_data);
    void JNICALL
    compressYUV(JNIEnv *env, jclass type,
                jbyteArray src_, jint width,
                jint height, jbyteArray dst_,
                jint dst_width, jint dst_height,
                jint mode, jint degree,
                jboolean isMirror);
    void argbToI420(jbyte *src_argb, jint width, jint height, jbyte *src_i420_data);
};
















#endif //YUVDEMO_YUVJNI_H

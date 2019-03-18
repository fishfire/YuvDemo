
#include "YuvJni.h"




 void YuvJni::JNI_OnUnload(JavaVM *jvm, void *reserved) {
    //进行释放
    free(Src_i420_data);
    free(Src_i420_data_scale);
    free(Src_i420_data_rotate);
}

void YuvJni::init(jint width, jint height, jint dst_width, jint dst_height) {
    Src_i420_data = (jbyte *) malloc(sizeof(jbyte) * width * height * 3 / 2);
    Src_i420_data_scale = (jbyte *) malloc(sizeof(jbyte) * dst_width * dst_height * 3 / 2);
    Src_i420_data_rotate = (jbyte *) malloc(sizeof(jbyte) * dst_width * dst_height * 3 / 2);
}

void YuvJni::scaleI420(jbyte *src_i420_data, jint width, jint height, jbyte *dst_i420_data, jint dst_width,
               jint dst_height, jint mode) {

    jint src_i420_y_size = width * height;
    jint src_i420_u_size = (width >> 1) * (height >> 1);
    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_i420_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_i420_y_size + src_i420_u_size;

    jint dst_i420_y_size = dst_width * dst_height;
    jint dst_i420_u_size = (dst_width >> 1) * (dst_height >> 1);
    jbyte *dst_i420_y_data = dst_i420_data;
    jbyte *dst_i420_u_data = dst_i420_data + dst_i420_y_size;
    jbyte *dst_i420_v_data = dst_i420_data + dst_i420_y_size + dst_i420_u_size;

    libyuv::I420Scale((const uint8_t *) src_i420_y_data, width,
                      (const uint8_t *) src_i420_u_data, width >> 1,
                      (const uint8_t *) src_i420_v_data, width >> 1,
                      width, height,
                      (uint8_t *) dst_i420_y_data, dst_width,
                      (uint8_t *) dst_i420_u_data, dst_width >> 1,
                      (uint8_t *) dst_i420_v_data, dst_width >> 1,
                      dst_width, dst_height,
                      (libyuv::FilterMode) mode);
}

void YuvJni::rotateI420(jbyte *src_i420_data, jint width, jint height, jbyte *dst_i420_data, jint degree) {
    jint src_i420_y_size = width * height;
    jint src_i420_u_size = (width >> 1) * (height >> 1);

    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_i420_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_i420_y_size + src_i420_u_size;

    jbyte *dst_i420_y_data = dst_i420_data;
    jbyte *dst_i420_u_data = dst_i420_data + src_i420_y_size;
    jbyte *dst_i420_v_data = dst_i420_data + src_i420_y_size + src_i420_u_size;

    //要注意这里的width和height在旋转之后是相反的
    if (degree == libyuv::kRotate90 || degree == libyuv::kRotate270) {
        libyuv::I420Rotate((const uint8_t *) src_i420_y_data, width,
                           (const uint8_t *) src_i420_u_data, width >> 1,
                           (const uint8_t *) src_i420_v_data, width >> 1,
                           (uint8_t *) dst_i420_y_data, height,
                           (uint8_t *) dst_i420_u_data, height >> 1,
                           (uint8_t *) dst_i420_v_data, height >> 1,
                           width, height,
                           (libyuv::RotationMode) degree);
    }
}

void YuvJni::mirrorI420(jbyte *src_i420_data, jint width, jint height, jbyte *dst_i420_data) {
    jint src_i420_y_size = width * height;
    jint src_i420_u_size = (width >> 1) * (height >> 1);

    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_i420_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_i420_y_size + src_i420_u_size;

    jbyte *dst_i420_y_data = dst_i420_data;
    jbyte *dst_i420_u_data = dst_i420_data + src_i420_y_size;
    jbyte *dst_i420_v_data = dst_i420_data + src_i420_y_size + src_i420_u_size;

    libyuv::I420Mirror((const uint8_t *) src_i420_y_data, width,
                       (const uint8_t *) src_i420_u_data, width >> 1,
                       (const uint8_t *) src_i420_v_data, width >> 1,
                       (uint8_t *) dst_i420_y_data, width,
                       (uint8_t *) dst_i420_u_data, width >> 1,
                       (uint8_t *) dst_i420_v_data, width >> 1,
                       width, height);
}


void YuvJni::nv21ToI420(jbyte *src_nv21_data, jint width, jint height, jbyte *src_i420_data) {
    jint src_y_size = width * height;
    jint src_u_size = (width >> 1) * (height >> 1);

    jbyte *src_nv21_y_data = src_nv21_data;
    jbyte *src_nv21_vu_data = src_nv21_data + src_y_size;

    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_y_size + src_u_size;


    libyuv::NV21ToI420((const uint8_t *) src_nv21_y_data, width,
                       (const uint8_t *) src_nv21_vu_data, width,
                       (uint8_t *) src_i420_y_data, width,
                       (uint8_t *) src_i420_u_data, width >> 1,
                       (uint8_t *) src_i420_v_data, width >> 1,
                       width, height);
}

void YuvJni::argbToI420(jbyte *src_argb, jint width, jint height, jbyte *src_i420_data) {
//    int ABGRToI420(const uint8_t* src_abgr,
//                   int src_stride_abgr,
//                   uint8_t* dst_y,
//                   int dst_stride_y,
//                   uint8_t* dst_u,
//                   int dst_stride_u,
//                   uint8_t* dst_v,
//                   int dst_stride_v,
//                   int width,
//                   int height);
    libyuv::ABGRToI420()
}

//extern "C"
//JNIEXPORT void JNICALL
//compressYUV(JNIEnv *env, jclass type,
//                                         jbyteArray src_, jint width,
//                                         jint height, jbyteArray dst_,
//                                         jint dst_width, jint dst_height,
//                                         jint mode, jint degree,
//                                         jboolean isMirror) {
//    //为中间操作需要的分配空间
//    init(width, height, dst_width, dst_height);
//
//    jbyte *Src_data = env->GetByteArrayElements(src_, NULL);
//    jbyte *Dst_data = env->GetByteArrayElements(dst_, NULL);
//    //nv21转化为i420
//    nv21ToI420(Src_data, width, height, Src_i420_data);
//    //进行缩放的操作
//    scaleI420(Src_i420_data, width, height, Src_i420_data_scale, dst_width, dst_height, mode);
//    if (isMirror) {
//        //进行旋转的操作
//        rotateI420(Src_i420_data_scale, dst_width, dst_height, Src_i420_data_rotate, degree);
//        //因为旋转的角度都是90和270，那后面的数据width和height是相反的
//        mirrorI420(Src_i420_data_rotate, dst_height, dst_width, Dst_data);
//    } else {
//        rotateI420(Src_i420_data_scale, dst_width, dst_height, Dst_data, degree);
//    }
//    env->ReleaseByteArrayElements(dst_, Dst_data, 0);
//}
//
//extern "C"
//JNIEXPORT void JNICALL
//cropYUV(JNIEnv *env, jclass type, jbyteArray src_, jint width,
//                                     jint height, jbyteArray dst_, jint dst_width, jint dst_height,
//                                     jint left, jint top) {
//    //裁剪的区域大小不对
//    if (left + dst_width > width || top + dst_height > height) {
//        return;
//    }
//
//    //left和top必须为偶数，否则显示会有问题
//    if (left % 2 != 0 || top % 2 != 0) {
//        return;
//    }
//
//    jint src_length = env->GetArrayLength(src_);
//    jbyte *src_i420_data = env->GetByteArrayElements(src_, NULL);
//    jbyte *dst_i420_data = env->GetByteArrayElements(dst_, NULL);
//
//
//    jint dst_i420_y_size = dst_width * dst_height;
//    jint dst_i420_u_size = (dst_width >> 1) * (dst_height >> 1);
//
//    jbyte *dst_i420_y_data = dst_i420_data;
//    jbyte *dst_i420_u_data = dst_i420_data + dst_i420_y_size;
//    jbyte *dst_i420_v_data = dst_i420_data + dst_i420_y_size + dst_i420_u_size;
//
//    libyuv::ConvertToI420((const uint8_t *) src_i420_data, src_length,
//                          (uint8_t *) dst_i420_y_data, dst_width,
//                          (uint8_t *) dst_i420_u_data, dst_width >> 1,
//                          (uint8_t *) dst_i420_v_data, dst_width >> 1,
//                          left, top,
//                          width, height,
//                          dst_width, dst_height,
//                          libyuv::kRotate0, libyuv::FOURCC_I420);
//
//    env->ReleaseByteArrayElements(dst_, dst_i420_data, 0);
//}
//
//extern "C"
//JNIEXPORT void JNICALL
//yuvI420ToNV21(JNIEnv *env, jclass type, jbyteArray i420Src,
//                                           jbyteArray nv21Src,
//                                           jint width, jint height) {
//
//    jbyte *src_i420_data = env->GetByteArrayElements(i420Src, NULL);
//    jbyte *src_nv21_data = env->GetByteArrayElements(nv21Src, NULL);
//
//    jint src_y_size = width * height;
//    jint src_u_size = (width >> 1) * (height >> 1);
//
//    jbyte *src_i420_y_data = src_i420_data;
//    jbyte *src_i420_u_data = src_i420_data + src_y_size;
//    jbyte *src_i420_v_data = src_i420_data + src_y_size + src_u_size;
//
//    jbyte *src_nv21_y_data = src_nv21_data;
//    jbyte *src_nv21_vu_data = src_nv21_data + src_y_size;
//
//
//    libyuv::I420ToNV21(
//            (const uint8_t *) src_i420_y_data, width,
//            (const uint8_t *) src_i420_u_data, width >> 1,
//            (const uint8_t *) src_i420_v_data, width >> 1,
//            (uint8_t *) src_nv21_y_data, width,
//            (uint8_t *) src_nv21_vu_data, width,
//            width, height);
//}
//




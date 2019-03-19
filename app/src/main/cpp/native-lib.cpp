#include <jni.h>
#include <string>
#include "libyuv/include/libyuv.h"
#include "YuvJni.h"

using namespace libyuv;

extern "C" JNIEXPORT jstring

JNICALL
Java_com_hui_yuvdemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_hui_yuvdemo_MainActivity_nativeARGBScale(JNIEnv *env, jobject thiz, jbyteArray array_,
                                                  jint width, jint height, jint dst_width,
                                                  jint dst_height) {
    jbyte *array = env->GetByteArrayElements(array_, NULL);
//    YuvJni yuvJni;
//    yuvJni.init(width,height,dst_width,dst_height);

    size_t dst_size = dst_width * dst_height * 4;
    uint8_t *dst = static_cast<uint8_t *>(malloc(dst_size));
    jbyteArray result = NULL;
    int ret = ARGBScale(reinterpret_cast<const uint8_t *>(array), width * 4, width, height, dst,
                        dst_width * 4, dst_width, dst_height, kFilterNone);

    if (ret == 0) {
        result = env->NewByteArray(dst_size);
        env->SetByteArrayRegion(result, 0, dst_size, reinterpret_cast<const jbyte *>(dst));
    }
    free(dst);
    dst = NULL;
    env->ReleaseByteArrayElements(array_, array, 0);

    return result;
}


extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_hui_yuvdemo_MainActivity_nativeABGRToI420(JNIEnv *env, jobject instance, jbyteArray array_,
                                                   jint width, jint height) {

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
    jbyte *array = env->GetByteArrayElements(array_, NULL);


    jbyteArray result = NULL;
    if (height % 2 != 0) {
        height++;
    }


    size_t dst_size = width * height * 3 >> 1;
    uint8_t *dst = static_cast<uint8_t *>(malloc(dst_size));
    uint8_t *dst_y = dst;
    int dst_stride_y = width;
    uint8_t *dst_u = dst + width * height;
    int dst_stride_u = (width + 1) >> 1;
    uint8_t *dst_v = dst + (width * height * 5 >> 2);
    int dst_stride_v = (width + 1) >> 1;
    int ret = ABGRToI420(reinterpret_cast<const uint8_t *>(array), width << 2,
                         dst_y, dst_stride_y, dst_u, dst_stride_u, dst_v, dst_stride_v, width,
                         height);

    if (ret == 0) {
        result = env->NewByteArray(dst_size);
        env->SetByteArrayRegion(result, 0, dst_size, reinterpret_cast<const jbyte *>(dst));
    }
    free(dst);
    dst = NULL;


    env->ReleaseByteArrayElements(array_, array, 0);

    return result;
}
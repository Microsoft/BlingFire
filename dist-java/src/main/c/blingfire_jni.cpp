#include <jni.h>
#include "blingfiretokdll.h"

using namespace BlingFire;

extern "C" {

JNIEXPORT jint JNICALL Java_com_github_lfoppiano_blingfire_BlingFire_GetBlingFireTokVersion
  (JNIEnv *env, jclass cls) {
    return GetBlingFireTokVersion();
}

JNIEXPORT jlong JNICALL Java_com_github_lfoppiano_blingfire_BlingFire_LoadModel
  (JNIEnv *env, jclass cls, jstring jModelPath) {
    const char *modelPath = env->GetStringUTFChars(jModelPath, 0);
    // LoadModel takes const char*
    void* handle = LoadModel(modelPath);
    env->ReleaseStringUTFChars(jModelPath, modelPath);
    return (jlong) handle;
}

JNIEXPORT void JNICALL Java_com_github_lfoppiano_blingfire_BlingFire_FreeModel
  (JNIEnv *env, jclass cls, jlong handle) {
    FreeModel((void*) handle);
}

JNIEXPORT jint JNICALL Java_com_github_lfoppiano_blingfire_BlingFire_TextToSentencesWithModel
  (JNIEnv *env, jclass cls, jbyteArray inBytes, jint inLen, jbyteArray outBytes, jint maxOutLen, jlong handle) {
    
    jbyte* inBuf = env->GetByteArrayElements(inBytes, NULL);
    jbyte* outBuf = env->GetByteArrayElements(outBytes, NULL);

    int res = TextToSentencesWithModel((char*)inBuf, (int)inLen, (char*)outBuf, (int)maxOutLen, (void*)handle);

    // Mode 0: copy back and free
    env->ReleaseByteArrayElements(inBytes, inBuf, JNI_ABORT); // input doesn't change
    env->ReleaseByteArrayElements(outBytes, outBuf, 0); // output changes

    return res;
}

JNIEXPORT jint JNICALL Java_com_github_lfoppiano_blingfire_BlingFire_TextToSentencesWithOffsetsWithModel
  (JNIEnv *env, jclass cls, jbyteArray inBytes, jint inLen, jbyteArray outBytes, jintArray startOffsets, jintArray endOffsets, jint maxOutLen, jlong handle) {

    jbyte* inBuf = env->GetByteArrayElements(inBytes, NULL);
    jbyte* outBuf = env->GetByteArrayElements(outBytes, NULL);
    jint* startBuf = env->GetIntArrayElements(startOffsets, NULL);
    jint* endBuf = env->GetIntArrayElements(endOffsets, NULL);

    // BlingFire expects int* for offsets
    int res = TextToSentencesWithOffsetsWithModel((char*)inBuf, (int)inLen, (char*)outBuf, (int*)startBuf, (int*)endBuf, (int)maxOutLen, (void*)handle);

    env->ReleaseByteArrayElements(inBytes, inBuf, JNI_ABORT);
    env->ReleaseByteArrayElements(outBytes, outBuf, 0);
    env->ReleaseIntArrayElements(startOffsets, startBuf, 0);
    env->ReleaseIntArrayElements(endOffsets, endBuf, 0);

    return res;
}

JNIEXPORT jint JNICALL Java_com_github_lfoppiano_blingfire_BlingFire_TextToWordsWithModel
  (JNIEnv *env, jclass cls, jbyteArray inBytes, jint inLen, jbyteArray outBytes, jint maxOutLen, jlong handle) {
    
    jbyte* inBuf = env->GetByteArrayElements(inBytes, NULL);
    jbyte* outBuf = env->GetByteArrayElements(outBytes, NULL);

    int res = TextToWordsWithModel((char*)inBuf, (int)inLen, (char*)outBuf, (int)maxOutLen, (void*)handle);

    env->ReleaseByteArrayElements(inBytes, inBuf, JNI_ABORT);
    env->ReleaseByteArrayElements(outBytes, outBuf, 0);

    return res;
}

JNIEXPORT jint JNICALL Java_com_github_lfoppiano_blingfire_BlingFire_TextToWordsWithOffsetsWithModel
  (JNIEnv *env, jclass cls, jbyteArray inBytes, jint inLen, jbyteArray outBytes, jintArray startOffsets, jintArray endOffsets, jint maxOutLen, jlong handle) {
    
    jbyte* inBuf = env->GetByteArrayElements(inBytes, NULL);
    jbyte* outBuf = env->GetByteArrayElements(outBytes, NULL);
    jint* startBuf = env->GetIntArrayElements(startOffsets, NULL);
    jint* endBuf = env->GetIntArrayElements(endOffsets, NULL);

    int res = TextToWordsWithOffsetsWithModel((char*)inBuf, (int)inLen, (char*)outBuf, (int*)startBuf, (int*)endBuf, (int)maxOutLen, (void*)handle);

    env->ReleaseByteArrayElements(inBytes, inBuf, JNI_ABORT);
    env->ReleaseByteArrayElements(outBytes, outBuf, 0);
    env->ReleaseIntArrayElements(startOffsets, startBuf, 0);
    env->ReleaseIntArrayElements(endOffsets, endBuf, 0);

    return res;
}

}

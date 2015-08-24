#include <jni.h>
#include <algorithm>

#include "base64.h"

using namespace std;

unsigned char key[] = {4, 2, 9, 4, 9, 6, 7, 2, 9, 5};


string applyXor(string sequence) {
    int maxIndex = sizeof(key) - 1;

    string result = sequence;
    size_t sequenceSize = sequence.size();

    int keyIndex = 0;

    for (int i = 0; i < sequenceSize; i++) {
        if (keyIndex > maxIndex)
            keyIndex = 0;

        result[i] = sequence[i] ^ key[keyIndex++];
    }

    return result;
}

extern "C" {

JNIEXPORT jstring JNICALL
Java_com_redmadrobot_ndkdemo_MainActivity_encrypt(JNIEnv *env, jobject instance,
                                                  jstring plainText_) {
    auto *plainText = env->GetStringUTFChars(plainText_, 0);

    string sequence = applyXor(plainText);
    size_t sequenceSize = sequence.size();

    reverse(sequence.begin(), sequence.end());

    int code = 0;

    for (unsigned long i = 0; i < sequenceSize; i++) {
        code = sequence[i] + 5;
        sequence[i] = (char) code;
    }

    env->ReleaseStringUTFChars(plainText_, plainText);

    return env->NewStringUTF(base64::encode(sequence).c_str());
}

JNIEXPORT jstring JNICALL
Java_com_redmadrobot_ndkdemo_MainActivity_decrypt(JNIEnv *env, jobject instance,
                                                  jstring ciphertext_) {
    auto *ciphertext = env->GetStringUTFChars(ciphertext_, 0);

    string sequence =  base64::decode(ciphertext);
    size_t sequenceSize = sequence.size();

    int code;
    for (int i = 0; i < sequenceSize; i++) {
        code = sequence[i] - 5;
        sequence[i] = (char) code;
    }

    reverse(sequence.begin(), sequence.end());

    env->ReleaseStringUTFChars(ciphertext_, ciphertext);

    return env->NewStringUTF(applyXor(sequence).c_str());
}
}
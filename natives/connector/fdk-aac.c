#include "connector.h"
#include <aacdecoder_lib.h>

CONNECTOR_EXPORT jlong JNICALL Java_com_sedmelluq_discord_lavaplayer_natives_aac_AacDecoderLibrary_create(JNIEnv *jni, jobject me, jint transport_type) {
	return (jintptr_t) aacDecoder_Open(transport_type, 1);
}

CONNECTOR_EXPORT void JNICALL Java_com_sedmelluq_discord_lavaplayer_natives_aac_AacDecoderLibrary_destroy(JNIEnv *jni, jobject me, jintptr_t instance) {
	aacDecoder_Close((HANDLE_AACDECODER) instance);
}

CONNECTOR_EXPORT jint JNICALL Java_com_sedmelluq_discord_lavaplayer_natives_aac_AacDecoderLibrary_configure(JNIEnv *jni, jobject me, jintptr_t instance, jlong buffer_data) {
	UCHAR* buffer = (UCHAR*)&buffer_data;
	UINT length = sizeof(jintptr_t);
	
	return aacDecoder_ConfigRaw((HANDLE_AACDECODER) instance, &buffer, &length);
}

CONNECTOR_EXPORT jint JNICALL Java_com_sedmelluq_discord_lavaplayer_natives_aac_AacDecoderLibrary_fill(JNIEnv *jni, jobject me, jintptr_t instance, jobject direct_buffer, jint offset, jint length) {
	UINT in_length = length;
	UINT in_left = length - offset;
	UCHAR* buffer = (*jni)->GetDirectBufferAddress(jni, direct_buffer);
	
	AAC_DECODER_ERROR error = aacDecoder_Fill((HANDLE_AACDECODER) instance, &buffer, &in_length, &in_left);
	if (error != AAC_DEC_OK) {
		return -error;
	}
	
	return length - offset - (jint) in_left;
}

CONNECTOR_EXPORT jint JNICALL Java_com_sedmelluq_discord_lavaplayer_natives_aac_AacDecoderLibrary_decode(JNIEnv *jni, jobject me, jintptr_t instance, jobject direct_buffer, jint length, jboolean flush) {
	INT_PCM* buffer = (*jni)->GetDirectBufferAddress(jni, direct_buffer);

	return aacDecoder_DecodeFrame((HANDLE_AACDECODER) instance, (INT_PCM*) buffer, length, flush ? AACDEC_FLUSH : 0);
}

CONNECTOR_EXPORT jlong JNICALL Java_com_sedmelluq_discord_lavaplayer_natives_aac_AacDecoderLibrary_getStreamInfo(JNIEnv *jni, jobject me, jintptr_t instance) {
	HANDLE_AACDECODER handle = (HANDLE_AACDECODER) instance;
	if (handle == NULL) {
		return 0;
	}

	CStreamInfo* stream_info = aacDecoder_GetStreamInfo(handle);
	return ((jlong) stream_info->sampleRate) << 32LL | ((jlong) stream_info->frameSize) << 16 | ((jlong) stream_info->numChannels);
}





#include <string.h>
#include <jni.h>
#include"curl/curl.h"
#include <errno.h>

#include <android/log.h>
#define LOGE(format, ...)  __android_log_print(ANDROID_LOG_ERROR, "(>_<)", format, ##__VA_ARGS__)
#define LOGI(format, ...)  __android_log_print(ANDROID_LOG_INFO,  "(^_^)", format, ##__VA_ARGS__)
#ifdef  __cplusplus
extern "C" {
#endif

void checkError(const char* op) {
	LOGE("%s error %d, %s\n", op, errno, strerror(errno));
}


jstring
Java_com_example_hellojni_HelloJni_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{

   #define ABI "armeabi"
	/*
	CURL *curl;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();
	if (curl){
		curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
		curl_easy_setopt(curl,CURLOPT_URL, "http://www.baidu.com");
		res = curl_easy_perform(curl);
		if (0!=res){
			LOGE("curl error: %d\n", res);
		}
		else
			LOGI("SUCCESSFUL!");
		curl_version_info_data *data = curl_version_info(CURLVERSION_NOW);
		LOGE("data version= %s, ssl_version = %s", data->version, data->ssl_version);
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();*/


	CURL *curl;
	CURLcode res;
	struct curl_slist *headerlist=NULL;
	static const char buf[] = "Expect:";
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	headerlist = curl_slist_append(headerlist, buf);
	if(curl)
	{
		LOGI("Successful setup!");
		int res;
	    res = curl_easy_setopt(curl, CURLOPT_URL, "https://passport.baidu.com/v2/?login");
	    if (0!=res){
	    	    			LOGE("CURLOPT_URL error: %d\n", res);
	    	    		}
	    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	    if (0!=res){
	    	    			LOGE("CURLOPT_HTTPHEADER error: %d\n", res);
	    	    		}
	    res = curl_easy_setopt(curl, CURLOPT_COOKIEJAR,"cookie_open.txt");//把服务器发过来的cookie保存到cookie_open.txt
	    if (0!=res){
	    	    			LOGE("CURLOPT_COOKIEJAR error: %d\n", res);
	    	    		}
	    #ifdef SKIP_PEER_VERIFICATION
	    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	#endif
	//#ifdef SKIP_HOSTNAME_VERFICATION
	    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	//#endif
	    res = curl_easy_perform(curl);
	    checkError("curl_easy_perform");
	    if (0!=res){
	    			LOGE("curl error: %d\n", res);
	    		}
	    /* always cleanup */
	    curl_easy_cleanup(curl);
	    /* free slist */
	    curl_slist_free_all(headerlist);
	}



	return (*env)->NewStringUTF(env, "Hello from JNI !  Compiled with ABI " ABI ".");
}
#ifdef  __cplusplus
};
#endif

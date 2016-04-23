# CURLcompile
a test app for libcurl
http://blog.csdn.net/cnhua57inyu/article/details/41693661

前面的讲解已经很详细，完全可以在linux下成功编译出curl的静态库供JNI调用，但本着详细的原则也为了以后再次用到时方便查阅，对此做个总结。
1.准备条件：

    操作系统：Linux（本机Ubuntu14.04）

    编译支持：android-ndk-r10e

    curl源文件：curl-7.46.0

    openssl源文件：openssl-android

2.编译准备：（shell）
	a.设置环境变量
	
		export CC=arm-linux-androideabi-gcc
		export LDFLAGS="-L$ANDROID_NDK_HOME/platforms/android-21/arch-arm/usr/lib"
		export CPPFLAGS="-I$ANDROID_NDK_HOME/platforms/android-21/arch-arm/usr/include"
		
		其中：
		ANDROID_NDK_HOME为NDK的绝对路径；
		CC为编译静态库提供了编译器，ubuntu下默认编译器为GCC；
		LDFLAGS是告诉编译器从哪里寻找需要的库文件；
		CPPFLAGS可选的编译器选项，在编译 C/C++ 代码文件的时候使用。这可能是有用的，指定一个附加的包含路径(相对于NDK的顶层目录)，宏定义，或者编译选项
	b.生成android编译器：
	
		$ANDROID_NDK_HOME/build/tools/make-standalone-toolchain.sh --platform=android-21
		
		原则上，此处的platform与上面的保持一致，若出现
		Unable to auto-config arch from toolchain
		则这是说无法自动配置 toolchain,需要手动配置.先看看你的ndk支持编译哪些cpu（通过打开toolchains文件夹查看），然后编辑 make-standalone-toolchain.sh,找到并修改 TOOLCHAIN_NAME变量
		此处修改为
		TOOLCHAIN_NAME=arm-linux-androideabi-4.9
		重新执行上述命令
根据打印的信息，将生成的arm-linux-androideabi-4.9.tar.bz2解压到任意位置
export PATH=$PATH:/yourDir/arm-linux-androideabi-4.9/bin
c.编译openssl：
	生成libssl.so和libcrypto.so两个库文件，将两个文件放入到
android-ndk-r10e\platforms\android-21\arch-arm\usr\lib目录下；这里的android-21对应上面指定的LDFLAGS；然后把openssl-android/include/openssl目录拷贝到android-ndk-r10e/platforms/android-21/arch-arm/usr/include目录；
3.编译：（shell）
	./configure --host=arm-linux-androideabi --with-ssl --disable-ftp --disable-gopher --disable-file --disable-imap --disable-ldap --disable-ldaps --disable-pop3 --disable-proxy --disable-rtsp --disable-smtp --disable-telnet --disable-tftp --without-gnutls --without-libidn --without-librtmp –disable-dict


执行结果如下：
    curl version:     7.46.0
    Host setup:       arm-unknown-linux-androideabi
    Install prefix:   /usr/local
    Compiler:         arm-linux-androideabi-gcc
    SSL support:      enabled (OpenSSL)
   SSH support:      no      (--with-libssh2)
    zlib support:     enabled
    GSS-API support:  no      (--with-gssapi)
   TLS-SRP support:  no      (--enable-tls-srp)
    resolver:         default (--enable-ares / --enable-threaded-resolver)
    IPv6 support:     no      (--enable-ipv6)
    Unix sockets support: enabled
    IDN support:      no      (--with-{libidn,winidn})
    Build libcurl:    Shared=yes, Static=yes
    Built-in manual:  enabled
    --libcurl option: enabled (--disable-libcurl-option)
    Verbose errors:   enabled (--disable-verbose)
    SSPI support:     no      (--enable-sspi)
    ca cert bundle:   no
    ca cert path:     no
    LDAP support:     no      (--enable-ldap / --with-ldap-lib / --with-lber-lib)
    LDAPS support:    no      (--enable-ldaps)
    RTSP support:     no      (--enable-rtsp)
    RTMP support:     no      (--with-librtmp)
    metalink support: no      (--with-libmetalink)
    PSL support:      no      (libpsl not found)
    HTTP2 support:    disabled (--with-nghttp2)
    Protocols:        HTTP HTTPS SMB SMBS

    SONAME bump:     yes - WARNING: this library will be built with the SONAME
                   number bumped due to (a detected) ABI breakage.
                   See lib/README.curl_off_t for details on this.

	在这里要注意：Compiler:  arm-linux-androideabi-gcc 对应上面的环境变量 CC

	执行完成以后，还需要修改curl_config.h文件的一些宏定义；
	打开：
	    HAVE_LIBSSL(#define HAVE_LIBSSL 1)
	    HAVE_OPENSSL_CRYPTO_H
	    HAVE_OPENSSL_ERR_H、
	HAVE_OPENSSL_PEM_H、 
	HAVE_OPENSSL_PKCS12_H、
	HAVE_OPENSSL_RSA_H、
	HAVE_OPENSSL_SSL_H、
	 HAVE_OPENSSL_X509_H、
	USE_OPENSSL、
	USE_SSLEAY
	android没有编译openssl中的engine，
	关掉
	HAVE_OPENSSL_ENGINE_H
	（？？）注释掉宏定义HAVE_MALLOC_H和HAVE_IOCTL
	注释掉宏 
	HAVE_GETPWUID_R、
	HAVE_GETEUID、
	HAVE_GETPWUID，否则编译的时候会出现类似： error: undefined reference to 'getpwuid_r'之类的错误；

	
	再执行
		make
		就会生成libcurl的库文件了，在lib/.libs下；
	
4.测试运行：（eclipse）
	需要将libcurl.a、libcrypto.so、libssl.so、libz.so拷贝到jni目录下；头文件拷贝到openssl下；
	libz.so位置在，android NDK自带支持；android-ndk-r10e/platforms/android-21/arch-arm/usr/lib

	Android.mk文件：位于jni目录下

#!/bin/sh

# Build related parameters
root_dir=/home/mohamad/BOT/Binance/binacpp
root_src_dir="${root_dir}/src"
current_dir=`pwd`

libcurl_dir="${root_dir}/lib/libcurl-7.56.0"
libcurl_include="${libcurl_dir}/include"
libcurl_lib="${libcurl_dir}/lib"

jsoncpp_dir="${root_dir}/lib/jsoncpp-1.8.3"
jsoncpp_include="${jsoncpp_dir}/include"
jsoncpp_src="${jsoncpp_dir}/src"

libwebsockets_dir="${root_dir}/lib/libwebsockets-2.4.0"
libwebsockets_include="${libwebsockets_dir}/include"
libwebsockets_lib="${libwebsockets_dir}/lib"

libbinacpp_dir="${root_dir}/lib/libbinacpp"
libbinacpp_include="${libbinacpp_dir}/include"
libbinacpp_lib="${libbinacpp_dir}/lib"

SRC="src/main.cpp src/binacpp_utils.cpp src/Menu.cpp src/CurlAPI.cpp src/Binance.cpp src/Ramzinex.cpp src/BotMethod.cpp src/Coinex.cpp src/HitBTC.cpp src/Kucoin.cpp"
BINARY=bot


# Grep related parameters
TorGrepPattern1="Not Found"
TorGrepPattern2="Total"
TorGrepPattern3="--:--:--"


# Remove old current binary file
rm $BINARY


# Build the libbinacpp
cd $root_src_dir
make 
cd $current_dir

# Build current binary
g++ -std=c++11 -I$libcurl_include -I$jsoncpp_include -I$libwebsockets_include -I$libbinacpp_include $SRC -L$libcurl_lib -L$libwebsockets_lib -L$libbinacpp_lib -lcurl -lcrypto -lwebsockets -lbinacpp -o $BINARY


# Execute current binary
export SSL_CERT_FILE=`pwd`/cacert.pem
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$libcurl_lib:$libwebsockets_lib:$libbinacpp_lib

if [ -f $BINARY ]
then
	echo "Successfully Build the $BINARY\n\n\n\n"
    torify ./$BINARY 3>&1 1>&2 2>&3 | grep -v -e "$TorGrepPattern1" -e "$TorGrepPattern2" -e "$TorGrepPattern3"
    # torify ./$BINARY 2>&1 | grep -v -e "$TorGrepPattern1" -e "$TorGrepPattern2" -e "$TorGrepPattern3"
else
	echo "Cannot Build the $BINARY"
fi





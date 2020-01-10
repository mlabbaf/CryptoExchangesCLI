#!/bin/sh

# Fill related parameters
root_dir=`pwd` 

libcurl_dir="${root_dir}/lib/libcurl-7.56.0"
libcurl_include="${libcurl_dir}/include"
libcurl_lib="${libcurl_dir}/lib"

jsoncpp_dir="${root_dir}/lib/jsoncpp-1.8.3"
jsoncpp_include="${jsoncpp_dir}/include"
# jsoncpp_src="${jsoncpp_dir}/src"

libwebsockets_dir="${root_dir}/lib/libwebsockets-2.4.0"
libwebsockets_include="${libwebsockets_dir}/include"
libwebsockets_lib="${libwebsockets_dir}/lib"

SRC="src/main.cpp src/Menu.cpp src/CurlAPI.cpp src/Binance.cpp src/Ramzinex.cpp src/BotMethod.cpp src/Coinex.cpp src/HitBTC.cpp src/Kucoin.cpp src/jsoncpp.cpp src/binacpp_utils.cpp src/Exchange.cpp"

BINARY=bot


# Grep related parameters
TorGrepPattern1="Not Found"
TorGrepPattern2="Total"
TorGrepPattern3="--:--:--"


# Remove old current binary file
if [ -f ./bin/$BINARY ]; then
	rm ./bin/$BINARY
fi

# Create bin directory
mkdir -p bin

# Build the binary
g++ -std=c++11 -I$libcurl_include -I$jsoncpp_include -I$libwebsockets_include $SRC -L$libcurl_lib -L$libwebsockets_lib -lcurl -lcrypto -lwebsockets -o ./bin/$BINARY


# Execute current binary
export SSL_CERT_FILE=`pwd`/cacert.pem
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$libcurl_lib:$libwebsockets_lib:$libbinacpp_lib

if [ -f ./bin/$BINARY ]
then
	echo "Successfully Build the $BINARY\n\n\n\n"
    torify ./bin/$BINARY 3>&1 1>&2 2>&3 | grep -v -e "$TorGrepPattern1" -e "$TorGrepPattern2" -e "$TorGrepPattern3"
else
	echo "Cannot Build the $BINARY"
fi





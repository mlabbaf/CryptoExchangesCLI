#!/bin/sh

# Build related parameters
root_dir=/home/mohamad/BOT/Binance/binacpp
libcurl_dir="${root_dir}/lib/libcurl-7.56.0"
libcurl_lib="${libcurl_dir}/lib"
jsoncpp_dir="${root_dir}/lib/jsoncpp-1.8.3"
jsoncpp_src="${jsoncpp_dir}/src"
libwebsockets_dir="${root_dir}/lib/libwebsockets-2.4.0"
libwebsockets_lib="${libwebsockets_dir}/lib"
libbinacpp_dir="${root_dir}/lib/libbinacpp"
libbinacpp_lib="${libbinacpp_dir}/lib"

BINARY=bot

# Grep related parameters
TorGrepPattern1="Not Found"
TorGrepPattern2="Total"
TorGrepPattern3="--:--:--"

# Execute current binary
export SSL_CERT_FILE=`pwd`/cacert.pem
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$libcurl_lib:$libwebsockets_lib:$libbinacpp_lib

if [ -f $BINARY ]
then
	echo "Successfully Run the $BINARY\n\n\n\n"
    torify ./$BINARY 3>&1 1>&2 2>&3 | grep -v -e "$TorGrepPattern1" -e "$TorGrepPattern2" -e "$TorGrepPattern3"
	
else
	echo "Cannot Run the $BINARY"
fi





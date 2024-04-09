#!/bin/bash

function SetMakeJobsCount {
	if ! [ -z "$1" ];
	then
		if ! [ "$1" -eq "$1" ] 2> /dev/null
		then
			echo "$1 is not a valid number of jobs!"
			exit
		fi
	fi
}

if [ "$1" == "--help" ];
then
	echo "first argument must have number of jobs which will execute the make command"
	exit
fi

SetMakeJobsCount

make_script="make -j$1 && make install"

rm -rf sdk
mkdir sdk
cd sdk
mkdir compiled
mkdir compiled/Debug
mkdir compiled/Release

git clone https://github.com/oatpp/oatpp.git
git clone https://github.com/oatpp/oatpp-swagger.git

mkdir oatpp/build
mkdir oatpp/build/Debug
mkdir oatpp/build/Release

cd oatpp/build/Debug
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="../../../compiled/Debug" ../../
eval $make_script

cd ../Release
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="../../../compiled/Release" ../../
eval $make_script

cd ../../../
mkdir oatpp-swagger/build
mkdir oatpp-swagger/build/Debug
mkdir oatpp-swagger/build/Release

cd oatpp-swagger/build/Debug
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="../../../compiled/Debug" ../../
eval $make_script

cd ../Release
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="../../../compiled/Release" ../../
eval $make_script

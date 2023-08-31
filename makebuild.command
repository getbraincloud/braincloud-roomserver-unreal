#!/bin/bash
# usage:
#      autobuild/makebuild.sh RelayTestApp MAC
#      autobuild/makebuild.sh RelayTestApp IOS
#      autobuild/makebuild.sh RelayTestApp ANDROID

# make sure to set the install path of your engine version
#       export UE_VERSION=5.1
#       export UE_INSTALL_PATH='/Users/Shared/Epic Games/UE_5.1'
#		 export WORKSPACE=$PWD #(this is examples root folder)

# use options nosign for android and distribution for ios

PROJECTNAME=DedicatedDemo
TARGET=MAC

if [[ "${TARGET}" ==  "IOS" ]];
then
	ModeString='-distribution'
fi

cd "`dirname "$0"`"
export WORKSPACE=$PWD

if [ -z "$UE_INSTALL_PATH" ];
then
    export UE_INSTALL_PATH='/Users/Shared/Epic Games/UE_5.1.1_Source'
fi

mkdir -p "$WORKSPACE/artifacts/"
mkdir -p "$WORKSPACE/artifacts/${PROJECTNAME}-${TARGET}"

"${UE_INSTALL_PATH}/Engine/Build/BatchFiles/Mac/Build.sh" ${PROJECTNAME}Client Mac Development -Project="$WORKSPACE/$PROJECTNAME.uproject" 
 
"${UE_INSTALL_PATH}/Engine/Build/BatchFiles/RunUAT.sh" BuildCookRun -rocket -nocompile -compileeditor -installed -nop4 -project="$WORKSPACE/$PROJECTNAME.uproject" -cook -stage -archive -archivedirectory="$WORKSPACE/artifacts/${PROJECTNAME}-${TARGET}" -package -clientconfig=Development -clean -pak -prereqs  ${ModeString}  -nodebuginfo -targetplatform=${TARGET} -build -target=${PROJECTNAME} -utf8output 
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
TARGET=${1:-MAC}

if [[ "${TARGET}" ==  "IOS" ]];
then
	ModeString='-distribution'
fi

#"${UE_INSTALL_PATH}/Engine/Build/BatchFiles/Mac/Build.sh" ${PROJECTNAME}Client Mac Development -Project="$WORKSPACE/$PROJECTNAME.uproject"
 
"${UE_INSTALL_PATH}/Engine/Build/BatchFiles/RunUAT.sh" BuildCookRun -rocket -nocompile -nocompileeditor -installed -nop4 -project="$WORKSPACE/$PROJECTNAME.uproject" -cook -stage -archive -archivedirectory="$WORKSPACE/ClientBuild" -package -clientconfig=Development -pak -prereqs  ${ModeString}  -allmaps -nodebuginfo -targetplatform=${TARGET} -build -target=${PROJECTNAME}Client -utf8output

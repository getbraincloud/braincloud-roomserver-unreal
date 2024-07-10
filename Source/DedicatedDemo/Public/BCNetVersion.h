// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"

class DEDICATEDDEMO_API UBCNetVersion : public FNetworkVersion
{
	public:
		static bool IsNetworkCompatibleOverride(uint32 LocalNetworkVersion, uint32 RemoteNetworkVersion);
		static uint32 GetLocalNetworkVersionOverride();
};

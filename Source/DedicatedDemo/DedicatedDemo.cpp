// Copyright Epic Games, Inc. All Rights Reserved.

#include "DedicatedDemo.h"
#include "Modules/ModuleManager.h"
#include "BCNetVersion.h"

class FDedicatedDemoModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override {
		FNetworkVersion::GetLocalNetworkVersionOverride.BindStatic(&UBCNetVersion::GetLocalNetworkVersionOverride);
	}

	virtual void ShutdownModule() override {
		FNetworkVersion::GetLocalNetworkVersionOverride.Unbind();
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FDedicatedDemoModule, DedicatedDemo, "DedicatedDemo" );
 
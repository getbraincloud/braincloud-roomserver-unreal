// Fill out your copyright notice in the Description page of Project Settings.


#include "BZ_BlueprintFunctionLibrary.h"
#include <WebSockets/Public/IWebSocket.h>
#include <WebSockets/Public/IWebSocketsManager.h>
#include <WebSockets/Public/WebSocketsModule.h>
#include "Math/RandomStream.h"

UBrainCloudAppData* UBZ_BlueprintFunctionLibrary::GetBCAppData()
{
    UBrainCloudAppData* appData = NewObject<UBrainCloudAppData>();

    FString ConfigPath = FConfigCacheIni::NormalizeConfigIniPath(FPaths::ProjectConfigDir() + TEXT("BrainCloudSettings.ini"));

    if (GConfig) {
        FString Section = "Credentials";
        FConfigSection* ConfigSection = GConfig->GetSectionPrivate(*Section, false, true, ConfigPath);
        FConfigFile* ConfigFile = GConfig->FindConfigFile(*ConfigPath);

        appData->AppId = ConfigSection->FindRef(TEXT("AppId")).GetValue();
        appData->AppSecret = ConfigSection->FindRef(TEXT("AppSecret")).GetValue();
        appData->ServerUrl = ConfigSection->FindRef(TEXT("ServerUrl")).GetValue();
        appData->Version = ConfigSection->FindRef(TEXT("Version")).GetValue();
    }

    return appData;
}

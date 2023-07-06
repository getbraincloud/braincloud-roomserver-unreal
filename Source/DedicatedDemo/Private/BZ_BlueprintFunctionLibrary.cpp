// Fill out your copyright notice in the Description page of Project Settings.


#include "BZ_BlueprintFunctionLibrary.h"
#include <WebSockets/Public/IWebSocket.h>
#include <WebSockets/Public/IWebSocketsManager.h>
#include <WebSockets/Public/WebSocketsModule.h>
#include "Math/RandomStream.h"


void UBZ_BlueprintFunctionLibrary::TestWebSocket(FString ServerURL)
{
	TSharedPtr<IWebSocket> WebSocket = FWebSocketsModule::Get().CreateWebSocket(ServerURL);

    WebSocket->OnMessage().AddLambda([](const FString& message)
        {
            UE_LOG(LogTemp, Log, TEXT("[WebSocket] Message received: %s"), *message);
        });

    WebSocket->OnConnected().AddLambda([WebSocket]()
        {
            UE_LOG(LogTemp, Log, TEXT("[WebSocket] connected"));

            FRandomStream RandStream;
            

            for (int i = 0; i <= 10; i++) {
                FString packetToSend = FString::Printf(TEXT("Testing %d"), RandStream.RandRange(0, 100));
                WebSocket->Send(packetToSend);
            }
        });

    WebSocket->OnMessageSent().AddLambda([](const FString& message)
        {
            UE_LOG(LogTemp, Log, TEXT("[WebSocket] Message sent: %s"), *message);
        });


    if (WebSocket.IsValid())
    {
        WebSocket->Connect();
        UE_LOG(LogTemp, Log, TEXT("[WebSocket] Connecting..."));
    }
}

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

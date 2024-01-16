// Copyright Epic Games, Inc. All Rights Reserved.

#include "DedicatedDemoGameMode.h"
#include <ConvertUtilities.h>
#include <Kismet/GameplayStatics.h>

DEFINE_LOG_CATEGORY(DedicatedServerLog);

void ADedicatedDemoGameMode::BeginPlay()
{
    if (GetNetMode() == NM_DedicatedServer && HasAuthority()) // Make sure we are dedicated
    {
        // Load environment variables passed in by brainCloud to our container.
        appId = getenv("APP_ID");
        serverName = getenv("SERVER_NAME");
        serverSecret = getenv("SERVER_SECRET");
        lobbyId = getenv("LOBBY_ID");

        FString serverUrl = "https://api.internal.braincloudservers.com/s2sdispatcher";

        // Create S2S context
        pS2S = NewObject<US2SRTTComms>();
        pS2S->AddToRoot();
        pS2S->InitializeS2S(appId, serverName, serverSecret, serverUrl, true, true);

        if (HasAuthority()) {
            PrimaryActorTick.bCanEverTick = true;
        }
    }

    Super::BeginPlay();
}

void ADedicatedDemoGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ADedicatedDemoGameMode::S2SRequest(const FString& requestJson, FS2SCallbackDelegate Callback)
{
    if (GetNetMode() == NM_DedicatedServer && HasAuthority()) {
        if (pS2S != nullptr) {
            pS2S->request(requestJson,
                [this, Callback](const FString& result)
                {
                    Callback.ExecuteIfBound(result);
                });
        }
    }
}

void ADedicatedDemoGameMode::EnableRTT(FS2SCallbackDelegate OnSuccess, FS2SCallbackDelegate OnFailure)
{
    if (pS2S != nullptr) {
        pS2S->enableRTT([this, OnSuccess](const FString& message) {
                OnSuccess.ExecuteIfBound(message);
            }, [this, OnFailure](const FString& message) {
                OnFailure.ExecuteIfBound(message);
            });
    }
}

void ADedicatedDemoGameMode::RegisterS2SRTTCallback(FS2SRTTCallbackDelegate Callback)
{
    if (pS2S != nullptr) {
        pS2S->registerRTTCallback(
            [this, Callback](const FString& message) {
                Callback.ExecuteIfBound(message);
            });
    }
}

void ADedicatedDemoGameMode::DeregisterS2SRTTCallback()
{
    if (pS2S != nullptr) {
        pS2S->deregisterRTTCallback();
    }
}

void ADedicatedDemoGameMode::RunCallbacks()
{
    if (GetNetMode() == NM_DedicatedServer && HasAuthority()) {
        if (pS2S != nullptr) {
            pS2S->runCallbacks();
        }
    }
}

void ADedicatedDemoGameMode::ShutdownServer()
{
    UE_LOG(DedicatedServerLog, Log, TEXT("Server shutting down"));
    GIsRequestingExit = true;
}

void ADedicatedDemoGameMode::JoinSystemChannel(FS2SRTTCallbackDelegate Callback)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

    JsonObject->SetStringField("service", "chat");
    JsonObject->SetStringField("operation", "SYS_CHANNEL_CONNECT");

    TSharedPtr<FJsonObject> dataObject = MakeShareable(new FJsonObject);
    dataObject->SetStringField("channelId", appId + ":sy:mysyschannel");
    dataObject->SetNumberField("maxReturn", 0);

    JsonObject->SetObjectField("data", dataObject);

    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);


    pS2S->request(JsonString,
        [this, Callback](const FString& result)
        {
            Callback.ExecuteIfBound(result);
        });
}

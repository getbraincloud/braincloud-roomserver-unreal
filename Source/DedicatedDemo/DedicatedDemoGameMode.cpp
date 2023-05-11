// Copyright Epic Games, Inc. All Rights Reserved.

#include "DedicatedDemoGameMode.h"


void ADedicatedDemoGameMode::BeginPlay()
{
    if (GetNetMode() == NM_DedicatedServer) // Make sure we are dedicated
    {
        // Load environment variables passed in by brainCloud to our container.
        FString appId = getenv("APP_ID");
        FString serverName = getenv("SERVER_NAME");
        FString serverSecret = getenv("SERVER_SECRET");
        lobbyId = getenv("LOBBY_ID");

        FString serverUrl = "https://api.internal.braincloudservers.com/s2sdispatcher";

        // Create S2S context
        pS2S = MakeShareable(new UBrainCloudS2S(appId,
            serverName,
            serverSecret,
            serverUrl, true));

        pS2S->Init(appId, serverName, serverSecret, serverUrl, true);

        // Verbose log
        pS2S->setLogEnabled(true);

        if (HasAuthority()) {
            PrimaryActorTick.bCanEverTick = true;
            UE_LOG(LogBrainCloudS2S, Log, TEXT("Tick enabled"));
        }

    }

    Super::BeginPlay();
}

void ADedicatedDemoGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ADedicatedDemoGameMode::S2SRequest(const FString& requestJson, FCustomCallbackDelegate Callback)
{
    pS2S->request(requestJson,
        [this, Callback](const FString& result)
        {
            Callback.ExecuteIfBound(result);
        });
}

void ADedicatedDemoGameMode::RunCallbacks()
{
    if (GetNetMode() == NM_DedicatedServer && HasAuthority()) {
        if (pS2S.IsValid()) {
            pS2S->runCallbacks();
        }
    }
}

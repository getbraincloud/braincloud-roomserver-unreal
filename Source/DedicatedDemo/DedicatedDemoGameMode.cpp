// Copyright Epic Games, Inc. All Rights Reserved.

#include "DedicatedDemoGameMode.h"


void ADedicatedDemoGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (GetNetMode() == NM_DedicatedServer) // Make sure we are dedicated
    {
        // Load environment variables passed in by brainCloud to our container.
        FString appId = getenv("APP_ID");
        FString serverName = getenv("SERVER_NAME");
        FString serverSecret = getenv("SERVER_SECRET");
        FString lobbyId = getenv("LOBBY_ID");

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

        // Request Lobby data.
        // Check out S2S Explorer in the portal to know more about calls that
        // can be done.
        pS2S->request("{\"service\":\"lobby\",\"operation\":\"GET_LOBBY_DATA\",\"data\":{\"lobbyId\":\"" + lobbyId + "\"}}",
            [this, lobbyId](const FString& result)
            {
                // ... Do server setup, decode result json, 
                // cache user/passcode for validation, etc.
                //UE_LOG(LogBrainCloudS2S, Log, TEXT("S2S lobbyData: %s"), *result);

                // Notify brainCloud that our server is ready
                pS2S->request("{\"service\":\"lobby\",\"operation\":\"SYS_ROOM_READY\",\"data\":{\"lobbyId\":\"" + lobbyId + "\"}}", nullptr);
            });
    }

}

void ADedicatedDemoGameMode::Tick(float DeltaTime)
{
    UE_LOG(LogBrainCloudS2S, Log, TEXT("GAME MODE CPP Tick"));
    if (GetNetMode() == NM_DedicatedServer && HasAuthority()) {
        if (pS2S.IsValid()) {
            pS2S->runCallbacks();
        }
    }

    Super::Tick(DeltaTime);
}

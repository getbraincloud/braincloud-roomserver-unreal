// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <S2SRTTComms.h>
#include "DedicatedDemoGameMode.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FS2SCallbackDelegate, const FString&, OutputString);
DECLARE_DYNAMIC_DELEGATE_OneParam(FS2SRTTCallbackDelegate, const FString&, JsonData);

DECLARE_LOG_CATEGORY_EXTERN(DedicatedServerLog, Log, All);

UCLASS(minimalapi)
class ADedicatedDemoGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "BrainCloud S2S")
		void S2SRequest(const FString& requestJson, FS2SCallbackDelegate Callback);

	UFUNCTION(BlueprintCallable, Category = "BrainCloud S2S")
		void EnableRTT(FS2SCallbackDelegate OnSuccess, FS2SCallbackDelegate OnFailure);

	UFUNCTION(BlueprintCallable, Category = "BrainCloud S2S")
		void RegisterS2SRTTCallback(FS2SRTTCallbackDelegate Callback);

	UFUNCTION(BlueprintCallable, Category = "BrainCloud S2S")
		void JoinSystemChannel(FS2SRTTCallbackDelegate Callback);

	UFUNCTION(BlueprintCallable, Category = "BrainCloud S2S")
		void DeregisterS2SRTTCallback();

	UFUNCTION(BlueprintCallable, Category = "BrainCloud S2S")
		void RunCallbacks();

	UFUNCTION(BlueprintCallable, Category = "Dedicated Server")
		void ShutdownServer();

	UPROPERTY(BlueprintReadOnly)
		FString lobbyId;

private:

	US2SRTTComms *pS2S;

	FString serverName;
	FString serverSecret;
	FString appId;
};






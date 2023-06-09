// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <BrainCloudS2S.h>
#include "DedicatedDemoGameMode.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FCustomCallbackDelegate, const FString&, OutputString);

UCLASS(minimalapi)
class ADedicatedDemoGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "BrainCloud S2S")
		void S2SRequest(const FString& requestJson, FCustomCallbackDelegate Callback);

	UFUNCTION(BlueprintCallable, Category = "BrainCloud S2S")
		void RunCallbacks();

	UFUNCTION(BlueprintCallable, Category = "Dedicated Server")
		void ShutdownServer();

	UPROPERTY(BlueprintReadOnly)
		FString lobbyId;

private:
	TSharedPtr<UBrainCloudS2S> pS2S;

	
};






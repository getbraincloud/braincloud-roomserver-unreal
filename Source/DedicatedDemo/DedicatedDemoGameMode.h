// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <BrainCloudS2S.h>
#include "DedicatedDemoGameMode.generated.h"


UCLASS(minimalapi)
class ADedicatedDemoGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	TSharedPtr<UBrainCloudS2S> pS2S;
};




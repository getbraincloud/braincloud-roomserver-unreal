// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../DedicatedDemoGameMode.h"
#include "BC_PC.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDDEMO_API ABC_PC : public APlayerController
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "Dedicated Server")
	BCNetMode GetNetModeEnum() const;
};

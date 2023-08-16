// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BrainCloudAppData.h"
#include "BZ_BlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDDEMO_API UBZ_BlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "BrainCloud Initialization")
		static UBrainCloudAppData* GetBCAppData();
};

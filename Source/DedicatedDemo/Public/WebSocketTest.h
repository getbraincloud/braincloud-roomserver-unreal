// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <WebSockets/Public/IWebSocket.h>
#include <WebSockets/Public/IWebSocketsManager.h>
#include <WebSockets/Public/WebSocketsModule.h>
#include "WebSocketTest.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FMessageCallbackDelegate, const FString&, OutputString);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDataCallbackDelegate, const TArray<uint8>&, Data);

UCLASS()
class DEDICATEDDEMO_API AWebSocketTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWebSocketTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "WebSocket Test")
	void ConnectSocket(const FString& url, FMessageCallbackDelegate OnMessageCallback, FDataCallbackDelegate OnDataCallback);

	UFUNCTION(BlueprintCallable, Category = "WebSocket Test")
	void SendMessage(const FString& message);

	UFUNCTION(BlueprintCallable, Category = "WebSocket Test")
	void SendData(const TArray<uint8>& data);

	TSharedPtr<IWebSocket> WebSocket;

};

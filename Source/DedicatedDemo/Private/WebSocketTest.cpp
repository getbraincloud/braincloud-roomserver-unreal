// Fill out your copyright notice in the Description page of Project Settings.


#include "WebSocketTest.h"

// Sets default values
AWebSocketTest::AWebSocketTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWebSocketTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWebSocketTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWebSocketTest::ConnectSocket(const FString& url, FMessageCallbackDelegate OnMessageCallback, FDataCallbackDelegate OnDataCallback)
{
	WebSocket = FWebSocketsModule::Get().CreateWebSocket(url);

	WebSocket->OnMessage().AddLambda([OnMessageCallback](const FString& message)
		{
			UE_LOG(LogTemp, Log, TEXT("[WebSocket] Message received: %s"), *message);
			OnMessageCallback.ExecuteIfBound(message);
		});

	WebSocket->OnRawMessage().AddLambda([OnDataCallback](const void* Data, SIZE_T Size, SIZE_T)
		{
			UE_LOG(LogTemp, Log, TEXT("[WebSocket] Data received"));
			TArray<uint8> DataArray;
			DataArray.Append((const uint8*)Data, Size);

			OnDataCallback.ExecuteIfBound(DataArray);
		});

	WebSocket->OnMessageSent().AddLambda([](const FString& message) 
		{
			UE_LOG(LogTemp, Log, TEXT("[WebSocket] Message sent %s"), *message);
		});

	WebSocket->OnConnected().AddLambda([]()
		{
			UE_LOG(LogTemp, Log, TEXT("[WebSocket] Connected"));
		});

	WebSocket->Connect();

	UE_LOG(LogTemp, Log, TEXT("[WebSocket] Connecting to URL %s"), *url);
}

void AWebSocketTest::SendMessage(const FString& message)
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		WebSocket->Send(message);
	}
}

void AWebSocketTest::SendData(const TArray<uint8>& data)
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		WebSocket->Send(data.GetData(), data.Num(), true);
	}
}


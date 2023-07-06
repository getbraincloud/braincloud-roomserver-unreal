// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <WebSockets/Public/IWebSocket.h>
#include <WebSockets/Public/IWebSocketsManager.h>
#include <WebSockets/Public/WebSocketsModule.h>

/**
 * 
 */

class WinWebSocket : public IWebSocket
{

public:
    WinWebSocket(const FString& ServerURL)
        : ServerURL(ServerURL)
    {
        // Create the WebSocket instance
        WebSocket = FWebSocketsModule::Get().CreateWebSocket(ServerURL);

        // Register the message receive callback
        WebSocket->OnMessage().AddLambda([this](const FString& Message)
            {
                OnMessageReceived(Message);
            });
    }

    // IWebSocket interface implementation
    virtual void Connect() override
    {
        if (WebSocket.IsValid())
        {
            WebSocket->Connect();
        }
    }

    virtual void Close(int32 Code, const FString& Reason) override
    {
        if (WebSocket.IsValid())
        {
            WebSocket->Close();
            UE_LOG(LogTemp, Log, TEXT("[WebSocket] Closed socket:%d: %s"), Code, *Reason);
        }
    }

    virtual void Send(const FString& Data) override
    {
        if (WebSocket.IsValid() && WebSocket->IsConnected())
        {
            WebSocket->Send(Data);
        }
    }

    virtual void Send(const void* Data, SIZE_T Size, bool bIsBinary) override
    {
        if (WebSocket.IsValid() && WebSocket->IsConnected())
        {
            WebSocket->Send(Data, Size, bIsBinary);
        }
    }

    virtual FWebSocketMessageEvent& OnMessage() override
    {
        if (WebSocket.IsValid() && WebSocket->IsConnected())
        {
            return WebSocket->OnMessage();
        }
    }

    // Callback function for message receive
    void OnMessageReceived(FString Message)
    {
        UE_LOG(LogTemp, Log, TEXT("[WebSocket] Message received: %s"), *Message);

    }

private:
    FString ServerURL;
    TSharedPtr<IWebSocket> WebSocket;
};

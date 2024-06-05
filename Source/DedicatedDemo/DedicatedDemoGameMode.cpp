// Copyright Epic Games, Inc. All Rights Reserved.

#include "DedicatedDemoGameMode.h"
#include <ConvertUtilities.h>
#include <Kismet/GameplayStatics.h>
#include <BrainCloudFunctionLibrary.h>

DEFINE_LOG_CATEGORY(DedicatedServerLog);

void ADedicatedDemoGameMode::BeginPlay()
{
    FModuleManager::Get().LoadModuleChecked(TEXT("WebSockets"));
    lobbyId = getenv("LOBBY_ID");

    if (!lobbyId.IsEmpty()) {
        //If lobbyId is provided by environment variables, this typically means we are not running the RSM in debug mode
        appId = getenv("APP_ID");
        serverName = getenv("SERVER_NAME");
        serverSecret = getenv("SERVER_SECRET");

        InitS2S(appId, serverName, serverSecret);
        OnLobbyAssigned();
    }
    //If lobbyId is empty this means the RSM tool is running in debug mode, and will wait for a websocket connection from this server to then send the lobbyId

    if (HasAuthority()) {
        PrimaryActorTick.bCanEverTick = true;
    }

    Super::BeginPlay();
}

void ADedicatedDemoGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ADedicatedDemoGameMode::InitS2S(const FString& AppID, const FString& ServerName, const FString& ServerSecret)
{
    if ((GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer) && HasAuthority()) // Make sure we are dedicated
    {
        // Load environment variables passed in by brainCloud to our container.
        this->appId = AppID;
        this->serverName = ServerName;
        this->serverSecret = ServerSecret;

        FBrainCloudAppDataStruct appData = UBrainCloudFunctionLibrary::GetBCAppData();
        FString serverUrl = appData.S2SUrl;

        // Create S2S context
        pS2S = NewObject<US2SRTTComms>();
        pS2S->AddToRoot();
        pS2S->InitializeS2S(appId, serverName, serverSecret, serverUrl, true, true);
        S2SInitialized = true;
    }
}

void ADedicatedDemoGameMode::ConnectToRSMTool()
{
    setupWebSocket("ws://127.0.0.1:8888");
}

void ADedicatedDemoGameMode::S2SRequest(const FString& requestJson, FS2SCallbackDelegate Callback)
{
    if ((GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer) && HasAuthority()) {
        if (pS2S != nullptr) {
            pS2S->request(requestJson,
                [this, Callback](const FString& result)
                {
                    Callback.ExecuteIfBound(result);
                });
        }
    }
}

void ADedicatedDemoGameMode::EnableRTT(FS2SCallbackDelegate OnSuccess, FS2SCallbackDelegate OnFailure)
{
    if (pS2S != nullptr) {
        pS2S->enableRTT([this, OnSuccess](const FString& message) {
                OnSuccess.ExecuteIfBound(message);
            }, [this, OnFailure](const FString& message) {
                OnFailure.ExecuteIfBound(message);
            });
    }
}

void ADedicatedDemoGameMode::RegisterS2SRTTCallback(FS2SRTTCallbackDelegate Callback)
{
    if (pS2S != nullptr) {
        pS2S->registerRTTCallback(
            [this, Callback](const FString& message) {
                Callback.ExecuteIfBound(message);
            });
    }
}

void ADedicatedDemoGameMode::DeregisterS2SRTTCallback()
{
    if (pS2S != nullptr) {
        pS2S->deregisterRTTCallback();
    }
}

void ADedicatedDemoGameMode::RunCallbacks()
{
    if ((GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer) && HasAuthority()) {
        if (pS2S != nullptr) {
            pS2S->runCallbacks();
        }
    }
}

void ADedicatedDemoGameMode::CloseWebsocketConnection()
{
    if (m_connectedSocket && m_connectedSocket->IsConnected()) {
        m_connectedSocket->Close();
    }
}

void ADedicatedDemoGameMode::ShutdownServer()
{
    UE_LOG(DedicatedServerLog, Log, TEXT("Server shutting down"));
    CloseWebsocketConnection();
    GIsRequestingExit = true;
}

BCNetMode ADedicatedDemoGameMode::GetNetModeEnum() const
{
    UWorld* world = GetWorld();
    ENetMode nm = world->GetNetMode();

    BCNetMode result;
    switch (nm) {
    case NM_Standalone:
        result = Standalone;
        break;
    case NM_Client:
        result = Client;
        break;
    case NM_DedicatedServer:
        result = DedicatedServer;
        break;
    case NM_ListenServer:
        result = ListenServer;
        break;
    case NM_MAX:
        result = MAX;
        break;
    }

    return result;
}

void ADedicatedDemoGameMode::OnLobbyAssigned_Implementation()
{
    UE_LOG(DedicatedServerLog, Log, TEXT("OnLobbyAssigned_Implementation"));
}

void ADedicatedDemoGameMode::OnRSMConnectError_Implementation(const FString& message)
{
}

void ADedicatedDemoGameMode::OnRSMConnectComplete_Implementation()
{
}

void ADedicatedDemoGameMode::setupWebSocket(const FString& in_url)
{
    if (m_connectedSocket == nullptr)
    {
        m_connectedSocket = NewObject<UWinWebSocketBase>();
        m_connectedSocket->SetupSocket(in_url, nullptr, true);

        m_connectedSocket->mCallbacks = this;

        m_connectedSocket->AddToRoot();
    }

    m_connectedSocket->Connect();
}

void ADedicatedDemoGameMode::OnConnectError(const FString& error)
{
    UE_LOG(DedicatedServerLog, Error, TEXT("Error connecting to RSM Websocket: %s"), *error);
    OnRSMConnectError(error);
}

void ADedicatedDemoGameMode::OnClosed()
{
    UE_LOG(DedicatedServerLog, Log, TEXT("Connection to RSM Websocket closed"));

    m_connectedSocket->RemoveFromRoot();

    m_connectedSocket = nullptr;
    delete m_connectedSocket;
}

void ADedicatedDemoGameMode::OnConnectComplete()
{
    UE_LOG(DedicatedServerLog, Log, TEXT("Successfully connected to RSM Websocket"));
    OnRSMConnectComplete();
}

void ADedicatedDemoGameMode::OnReceiveData(const TArray<uint8>& data)
{
    UE_LOG(DedicatedServerLog, Log, TEXT("Received Data from RSM websocket"));

    FString parsedMessage = ConvertUtilities::BCBytesToString(data.GetData(), data.Num());
    OnDataProcessed(parsedMessage);
}

void ADedicatedDemoGameMode::OnDataProcessed(const FString& message) {

    TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(message);
    TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject());

    if (!FJsonSerializer::Deserialize(reader, json))
    {
        UE_LOG(DedicatedServerLog, Error, TEXT("Invalid Json in RSM Websocket packet"));
        return;
    }

    auto op = json->Values["op"]->AsString();

    if (op == "InitS2S" && !S2SInitialized) {
        TSharedPtr<FJsonObject> data = json->Values["data"]->AsObject();
        
        this->appId = data->Values["appId"]->AsString();
        this->serverName = data->Values["serverName"]->AsString();
        this->serverSecret = data->Values["serverSecret"]->AsString();
        
        UE_LOG(DedicatedServerLog, Log, TEXT("Initializing S2S with appID: %s serverName: %s serverSecret: %s"), *appId, *serverName, *serverSecret);

        InitS2S(appId, serverName, serverSecret);
    }
    if (op == "AssignLobby" && lobbyId.IsEmpty()) {
        TSharedPtr<FJsonObject> data = json->Values["data"]->AsObject();

        this->lobbyId = data->Values["lobbyId"]->AsString();

        UE_LOG(DedicatedServerLog, Log, TEXT("Assigned lobbyId: %s"), *lobbyId);

        OnLobbyAssigned();
    }
}

void ADedicatedDemoGameMode::JoinSystemChannel(FS2SRTTCallbackDelegate Callback)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

    JsonObject->SetStringField("service", "chat");
    JsonObject->SetStringField("operation", "SYS_CHANNEL_CONNECT");

    TSharedPtr<FJsonObject> dataObject = MakeShareable(new FJsonObject);
    dataObject->SetStringField("channelId", appId + ":sy:mysyschannel");
    dataObject->SetNumberField("maxReturn", 0);

    JsonObject->SetObjectField("data", dataObject);

    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);


    pS2S->request(JsonString,
        [this, Callback](const FString& result)
        {
            Callback.ExecuteIfBound(result);
        });
}

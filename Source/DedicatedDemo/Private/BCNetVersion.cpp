// Fill out your copyright notice in the Description page of Project Settings.


#include "BCNetVersion.h"

bool UBCNetVersion::IsNetworkCompatibleOverride(uint32 LocalNetworkVersion, uint32 RemoteNetworkVersion)
{
    // Extract major and minor version numbers from the version integers
    uint32 LocalMajor = (LocalNetworkVersion >> 16) & 0xFFFF;
    uint32 LocalMinor = LocalNetworkVersion & 0xFFFF;
    uint32 RemoteMajor = (RemoteNetworkVersion >> 16) & 0xFFFF;
    uint32 RemoteMinor = RemoteNetworkVersion & 0xFFFF;

    UE_LOG(LogTemp, Log, TEXT("Comparing Local version: %d.%d Remote version: %d.%d"), LocalMajor, LocalMinor, RemoteMajor, RemoteMinor);
    UE_LOG(LogTemp, Log, TEXT("Full Local Version: %d Full Remote Version: %d"), LocalNetworkVersion, RemoteNetworkVersion);
    // Compare major and minor versions
    return (LocalMajor == RemoteMajor) && (LocalMinor == RemoteMinor);
}

uint32 UBCNetVersion::GetLocalNetworkVersionOverride()
{
    TArray<FString> Parts;
    FNetworkVersion::GetProjectVersion().ParseIntoArray(Parts, TEXT("."));

    FString minimalVersionString;

    if (Parts.Num() >= 2) 
    {
        minimalVersionString = Parts[0] + TEXT(".") + Parts[1];
    }
    else if (Parts.Num() == 1)
    {
        minimalVersionString = Parts[0];
    }

    FString VersionString = FString::Printf(TEXT("%s %s, NetCL: %d, EngineNetVer: %d, GameNetVer: %d"),
        FApp::GetProjectName(),
        *minimalVersionString,
        GetNetworkCompatibleChangelist(),
        FNetworkVersion::GetEngineNetworkProtocolVersion(),
        FNetworkVersion::GetGameNetworkProtocolVersion());

    CachedNetworkChecksum = FCrc::StrCrc32(*VersionString.ToLower());

    UE_LOG(LogNetVersion, Log, TEXT("%s (Checksum: %u)"), *VersionString, CachedNetworkChecksum);

    bHasCachedNetworkChecksum = true;

    return CachedNetworkChecksum;
}

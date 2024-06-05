// Fill out your copyright notice in the Description page of Project Settings.


#include "BC_PC.h"

BCNetMode ABC_PC::GetNetModeEnum() const
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

#pragma once
#include <Windows.h>

#define GWorld 0xEA84A78 // gworld is a pointer of uworld. 

namespace Offsets
{
    enum External : uint64_t
    {
        // TeamIndex - TeamID - ActorID
        TeamId = 0x10c8,

        // Local
        LocalActorPos = 0x128,
        LocalPawn = 0x330,
        LocalPlayers = 0x38,

        // Player - BoneArray
        BoneArray = 0x600,
        PlayerState = 0x2A8,
        PlayerArray = 0x02a0,
        PlayerController = 0x30,
        PlayerCameraManager = 0x340,

        // Game - World
        Gameinstance = 0x1B8,
        GameState = 0x158,
        ComponetToWorld = 0x240,

        // Actor
        ActorCount = 0xA0,
        AActor = 0x98,
        CurrentActor = 0x300,

        // Other
        RootComponet = 0x190,
        PersistentLevel = 0x30,
        Mesh = 0x310,
        Velocity = 0xb8,
        PawnPrivate = 0x300,
        ReviveFromDBNOTime = 0x45b0,

        // Visible Check
        LastSubmitTime = 0x360,
        LastRenderTimeOnScreen = 0x368,
    };
}


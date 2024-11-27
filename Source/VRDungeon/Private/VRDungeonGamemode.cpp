#include "VRDungeonGamemode.h"

#include "VRDungeonGameState.h"
#include "VRPawn.h"

AVRDungeonGamemode::AVRDungeonGamemode()
{
	DefaultPawnClass = AVRPawn::StaticClass();
	GameStateClass = AVRDungeonGameState::StaticClass();
}

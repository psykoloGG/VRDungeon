#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VRPawn.generated.h"

class AMovementWaypoint;

UCLASS()
class VRDUNGEON_API AVRPawn : public APawn
{
	GENERATED_BODY()

public:
	AVRPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void GenerateWaypoints();
	void CheckIfLookingInWaypointDirection();

	UFUNCTION()
	void Move();

	float MoveGridSize = 300.0f;

	UPROPERTY()
	TArray<AMovementWaypoint*> AvailableWaypoints;
	
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* VRRoot;
	
	UPROPERTY(EditAnywhere)
	class UCameraComponent* VRCamera;

	UPROPERTY()
	class UVRGesturesComponent* VRGesturesComponent;
};

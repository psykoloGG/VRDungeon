#pragma once

#include "CoreMinimal.h"
#include "SpeechRecognition.h"
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

	// On words spoken we go trough phrases our game supports and pick event
	UFUNCTION()
	void WordsSpoken(FRecognisedPhrases Text);

	UFUNCTION()
	void Move();

	void CastFireball();
	void CastLight();

	float MoveGridSize = 300.0f;
	bool bIsMoving = false;

	UPROPERTY()
	TArray<AMovementWaypoint*> AvailableWaypoints;
	
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* VRRoot;
	
	UPROPERTY(EditAnywhere)
	class UCameraComponent* VRCamera;

	UPROPERTY(VisibleAnywhere)
	class UVRGesturesComponent* VRGesturesComponent;

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<AActor> BPProjectileToSpawn;

	UPROPERTY(VisibleAnywhere)
	class UPointLightComponent* PointLight;

	UPROPERTY(VisibleAnywhere)
	class AMovementWaypoint* SelectedWaypoint;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VRPawn.generated.h"

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
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* VRRoot;
	
	UPROPERTY(EditAnywhere)
	class UCameraComponent* VRCamera;

	UPROPERTY(EditAnywhere)
	class UVRGesturesComponent* VRGesturesComponent;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovementWaypoint.generated.h"

UCLASS()
class VRDUNGEON_API AMovementWaypoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovementWaypoint();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Called when looked at to signal player that it is selected
	UFUNCTION()
	void Activate();
	
	// Called when looked away from to signal that it is not selected
	UFUNCTION()
	void Deactivate();

	UFUNCTION()
	bool IsActive() const { return bIsActive; }

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshComponent;

private:
	bool bIsActive = false;
};

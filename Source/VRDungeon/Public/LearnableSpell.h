#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LearnableSpell.generated.h"

UCLASS()
class VRDUNGEON_API ALearnableSpell : public AActor
{
	GENERATED_BODY()
	
public:
	ALearnableSpell();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	bool bSpellLearned = false;

	UPROPERTY(EditAnywhere)
	FString SpellName;

	// Area to which player has to enter to learn the spell
	UPROPERTY(EditAnywhere)
	class UBoxComponent* LearnableAreaBox;

protected:
	virtual void BeginPlay() override;

};

#include "LearnableSpell.h"

#include "SpeechRecognition.h"
#include "SpeechRecognitionSubsystem.h"
#include "VRDungeonGameState.h"
#include "VRPawn.h"
#include "Components/BoxComponent.h"

ALearnableSpell::ALearnableSpell()
{
	PrimaryActorTick.bCanEverTick = true;

	LearnableAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LearnableAreaBox"));
	RootComponent = LearnableAreaBox;

	LearnableAreaBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LearnableAreaBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LearnableAreaBox->SetCollisionProfileName(TEXT("OverlapAll"));
	LearnableAreaBox->OnComponentBeginOverlap.AddDynamic(this, &ALearnableSpell::OnOverlapBegin);
	
}

void ALearnableSpell::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Player learnt spell: %s"), *OtherActor->GetName());
	if (bSpellLearned)
	{
		return;
	}

	if (OtherActor->IsA(AVRPawn::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player learnt spell: %s"), *SpellName);

		FRecognitionPhrase Phrase;
		Phrase.phrase = SpellName;
		Phrase.tolerance = EPhraseRecognitionTolerance::VE_6;
		AVRDungeonGameState* GameState = GetWorld()->GetGameState<AVRDungeonGameState>();
		GameState->AddPhraseToRecognize(Phrase);
		bSpellLearned = true;
	}
}

void ALearnableSpell::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALearnableSpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


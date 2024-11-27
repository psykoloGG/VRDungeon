#pragma once

#include "CoreMinimal.h"
#include "SpeechRecognitionGameStateBase.h"
#include "VRDungeonGameState.generated.h"

/**
 * 
 */
UCLASS()
class VRDUNGEON_API AVRDungeonGameState : public ASpeechRecognitionGameStateBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	void AddPhraseToRecognize(FRecognitionPhrase Phrase);

private:
	TArray<FRecognitionPhrase> RecognizedPhrases;
};


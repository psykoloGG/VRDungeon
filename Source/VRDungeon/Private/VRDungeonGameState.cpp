#include "VRDungeonGameState.h"

#include "SpeechRecognitionSubsystem.h"

void AVRDungeonGameState::BeginPlay()
{
	Super::BeginPlay();
}

void AVRDungeonGameState::AddPhraseToRecognize(FRecognitionPhrase Phrase)
{
	RecognizedPhrases.Add(Phrase);
	USpeechRecognitionSubsystem* SpeechRecognitionSubsystem = GetWorld()->GetSubsystem<USpeechRecognitionSubsystem>();
	SpeechRecognitionSubsystem->EnableKeywordMode(RecognizedPhrases);
}

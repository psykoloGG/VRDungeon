#include "VRPawn.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "VRGesturesComponent.h"

AVRPawn::AVRPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	RootComponent = VRRoot;

	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(VRRoot);

	VRGesturesComponent = CreateDefaultSubobject<UVRGesturesComponent>(TEXT("VRGesturesComponent"));
	VRGesturesComponent->VRCamera = VRCamera;
	
}

void AVRPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		VRCamera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		VRCamera->bLockToHmd = true;
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Stage);
	}
}

void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


#include "VRPawn.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "MovementWaypoint.h"
#include "Camera/CameraComponent.h"
#include "VRGesturesComponent.h"
#include "Kismet/KismetMathLibrary.h"

AVRPawn::AVRPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	RootComponent = VRRoot;

	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(VRRoot);

	VRGesturesComponent = CreateDefaultSubobject<UVRGesturesComponent>(TEXT("VRGesturesComponent"));
	VRGesturesComponent->VRCamera = VRCamera;
	VRGesturesComponent->OnNodYes.AddDynamic(this, &AVRPawn::Move);
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

	GenerateWaypoints();
}

void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckIfLookingInWaypointDirection();
}

void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AVRPawn::GenerateWaypoints()
{
	for (AMovementWaypoint* MovementWaypoint : AvailableWaypoints)
	{
		MovementWaypoint->Destroy();
	}
	AvailableWaypoints.Empty();
	
	FVector FirstWaypointLocation = GetActorLocation();
	FirstWaypointLocation.X += MoveGridSize;
	FirstWaypointLocation.Z = 0.0f;

	FVector SecondWaypointLocation = GetActorLocation();
	SecondWaypointLocation.X -= MoveGridSize;
	SecondWaypointLocation.Z = 0.0f;

	FVector ThirdWaypointLocation = GetActorLocation();
	ThirdWaypointLocation.Y += MoveGridSize;
	ThirdWaypointLocation.Z = 0.0f;

	FVector FourthWaypointLocation = GetActorLocation();
	FourthWaypointLocation.Y -= MoveGridSize;
	FourthWaypointLocation.Z = 0.0f;

	AvailableWaypoints.Add(GetWorld()->SpawnActor<AMovementWaypoint>(FirstWaypointLocation, FRotator::ZeroRotator));
	AvailableWaypoints.Add(GetWorld()->SpawnActor<AMovementWaypoint>(SecondWaypointLocation, FRotator::ZeroRotator));
	AvailableWaypoints.Add(GetWorld()->SpawnActor<AMovementWaypoint>(ThirdWaypointLocation, FRotator::ZeroRotator));
	AvailableWaypoints.Add(GetWorld()->SpawnActor<AMovementWaypoint>(FourthWaypointLocation, FRotator::ZeroRotator));
}

void AVRPawn::CheckIfLookingInWaypointDirection()
{
	FVector ForwardVector = VRCamera->GetForwardVector();
	for (AMovementWaypoint* MovementWaypoint : AvailableWaypoints)
	{
		FVector WaypointLocation = MovementWaypoint->GetActorLocation();
		FVector DirectionToWaypoint = WaypointLocation - VRCamera->GetComponentLocation();
		DirectionToWaypoint.Normalize();
		float DotProduct = FVector::DotProduct(ForwardVector, DirectionToWaypoint);
		float AngleInDegrees = FMath::Acos(DotProduct) * 180.0f / PI;
		if (AngleInDegrees < 45.0f)
		{
			MovementWaypoint->Activate();
		}
		else
		{
			MovementWaypoint->Deactivate();
		}
	}
}

void AVRPawn::Move()
{
	// TODO: something with fade to black to prevent motion sickness
	for (AMovementWaypoint* MovementWaypoint : AvailableWaypoints)
	{
		if (MovementWaypoint && MovementWaypoint->IsActive())
		{
			FVector WaypointLocation = MovementWaypoint->GetActorLocation();

			// Rotate player towards new direction
			FVector DirectionToWaypoint = WaypointLocation - GetActorLocation();
			DirectionToWaypoint.Normalize();
			FRotator NewRotation = UKismetMathLibrary::MakeRotFromX(DirectionToWaypoint);
			NewRotation.Pitch = 0.0f;
			NewRotation.Roll = 0.0f;
			SetActorRotation(NewRotation);

			// Teleport player to next target
			WaypointLocation.Z = GetRootComponent()->GetComponentLocation().Z;
			SetActorLocation(WaypointLocation, false, nullptr, ETeleportType::TeleportPhysics);
			
			GenerateWaypoints();
		}
	}
}

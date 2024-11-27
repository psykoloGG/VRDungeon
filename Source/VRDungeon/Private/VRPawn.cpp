#include "VRPawn.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "MovementWaypoint.h"
#include "SpeechRecognitionSubsystem.h"
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

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(VRRoot);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	CapsuleComponent->SetCollisionProfileName(TEXT("OverlapAll"));

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetupAttachment(VRCamera);
	PointLight->SetIntensity(3000.0f);
	PointLight->AttenuationRadius = 1000.0f;
	PointLight->SetVisibility(false);

	static ConstructorHelpers::FClassFinder<AActor> ProjectileClassFinder(TEXT("/Game/Magic_Projectiles/MagicProjectiles/Blueprints/Fire/BP_Fire_Projectile.BP_Fire_Projectile_C"));
	if (ProjectileClassFinder.Succeeded())
	{
		BPProjectileToSpawn = ProjectileClassFinder.Class;
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find BP_Fire_Projectile"));
	}
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

	USpeechRecognitionSubsystem* SpeechRecognitionSubsystem = GetWorld()->GetSubsystem<USpeechRecognitionSubsystem>();
	SpeechRecognitionSubsystem->OnWordsSpoken.AddDynamic(this, &AVRPawn::WordsSpoken);

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
	if (bIsMoving)
	{
		return;
	}
	
	FVector ForwardVector = VRCamera->GetForwardVector();
	for (AMovementWaypoint* MovementWaypoint : AvailableWaypoints)
	{
		// Artificially increase height
		FVector WaypointLocation = MovementWaypoint->GetActorLocation();
		WaypointLocation.Z = VRCamera->GetComponentLocation().Z - 50.0f;
		
		FVector DirectionToWaypoint = WaypointLocation - VRCamera->GetComponentLocation();
		DirectionToWaypoint.Normalize();
		float DotProduct = FVector::DotProduct(ForwardVector, DirectionToWaypoint);
		float AngleInDegrees = FMath::Acos(DotProduct) * 180.0f / PI;
		if (AngleInDegrees < 30.0f)
		{
			MovementWaypoint->Activate();
		}
		else
		{
			MovementWaypoint->Deactivate();
		}
	}
}

void AVRPawn::WordsSpoken(FRecognisedPhrases Text)
{
	for (FString Phrase : Text.phrases)
	{
		if(Phrase == "flame")
		{
			CastFireball();
		}
		if(Phrase == "lux")
		{
			CastLight();
		}
	}
}

void AVRPawn::Move()
{
	for (AMovementWaypoint* MovementWaypoint : AvailableWaypoints)
	{
		// This is not the way to do it, but whatever for now
		if (!MovementWaypoint)
		{
			return;
		}
		
		if (MovementWaypoint->IsActive())
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

			
		}
	}
	GenerateWaypoints();
}

void AVRPawn::CastFireball()
{
	if (BPProjectileToSpawn)
	{
		FVector SpawnLocation = VRCamera->GetComponentLocation() + VRCamera->GetForwardVector() * 100.0f;
		FRotator SpawnRotation = VRCamera->GetComponentRotation();
		GetWorld()->SpawnActor<AActor>(BPProjectileToSpawn, SpawnLocation, SpawnRotation);
	}
}

void AVRPawn::CastLight()
{
	UE_LOG(LogTemp, Warning, TEXT("Casting light"));
	if (!PointLight->IsVisible())
	{
		PointLight->SetVisibility(true);
	}
	else
	{
		PointLight->SetVisibility(false);
	}
}

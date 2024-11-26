#include "MovementWaypoint.h"

#include "Components/CapsuleComponent.h"

AMovementWaypoint::AMovementWaypoint()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	UStaticMesh* WaypointMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/NewContent/Assets/Waypoints/waypoint.waypoint'"));
	StaticMeshComponent->SetStaticMesh(WaypointMesh);
	StaticMeshComponent->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.15f));
	Deactivate();
	
}

void AMovementWaypoint::BeginPlay()
{
	Super::BeginPlay();
}

void AMovementWaypoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMovementWaypoint::Activate()
{
	UMaterialInterface* WaypointMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/NewContent/Assets/Waypoints/M_Active.M_Active'"));
	StaticMeshComponent->SetMaterial(0, WaypointMaterial);
	StaticMeshComponent->SetMaterial(1, WaypointMaterial);
	bIsActive = true;
}

void AMovementWaypoint::Deactivate()
{
	UMaterialInterface* WaypointMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/NewContent/Assets/Waypoints/M_NotActive.M_NotActive'"));
	StaticMeshComponent->SetMaterial(0, WaypointMaterial);
	StaticMeshComponent->SetMaterial(1, WaypointMaterial);
	bIsActive = false;
}


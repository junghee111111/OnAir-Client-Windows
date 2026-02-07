// Copyright 2025, Ducksoo Games. All RIghts Reserved.


#include "DungeonRoom.h"


// Sets default values
ADungeonRoom::ADungeonRoom()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create DefaultSceneRoot
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	// Create and attach Wall Meshes
	WallFrontMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallFrontMesh"));
	WallFrontMesh->SetupAttachment(DefaultSceneRoot);
	WallFrontMesh->SetRelativeLocation(FVector(0.0f, -500.0f, 0.0f)); // 앞쪽 벽
	WallFrontMesh->SetRelativeRotation(FRotator(90.0f, 90.0f, 0.0f));

	WallBackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallBackMesh"));
	WallBackMesh->SetupAttachment(DefaultSceneRoot);
	WallBackMesh->SetRelativeLocation(FVector(0.0f, 500.0f, 0.0f)); // 뒤쪽 벽
	WallBackMesh->SetRelativeRotation(FRotator(90.0f, 90.0f, 0.0f));

	WallLeftMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallLeftMesh"));
	WallLeftMesh->SetupAttachment(DefaultSceneRoot);
	WallLeftMesh->SetRelativeLocation(FVector(-050.0f, 0.0f, 0.0f)); // 왼쪽 벽
	WallLeftMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	WallRightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallRightMesh"));
	WallRightMesh->SetupAttachment(DefaultSceneRoot);
	WallRightMesh->SetRelativeLocation(FVector(500.0f, 0.0f, 0.0f)); // 오른쪽 벽
	WallRightMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	WallFloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	WallFloorMesh->SetupAttachment(DefaultSceneRoot);
	WallFloorMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f)); // 바닥

	WallCeilingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ceiling"));
	WallCeilingMesh->SetupAttachment(DefaultSceneRoot);
	WallCeilingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 500.0f)); // 천장
}

// Called when the game starts or when spawned
void ADungeonRoom::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADungeonRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


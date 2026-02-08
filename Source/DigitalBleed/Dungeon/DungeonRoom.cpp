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
	WallLeftMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallLeftMesh"));
	WallLeftMesh->SetupAttachment(DefaultSceneRoot);
	WallLeftMesh->SetRelativeLocation(FVector(0.0f, -250.0f, 250.0f)); // 왼쪽 벽
	WallLeftMesh->SetRelativeRotation(FRotator(90.0f, 90.0f, 0.0f));

	WallRightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallRightMesh"));
	WallRightMesh->SetupAttachment(DefaultSceneRoot);
	WallRightMesh->SetRelativeLocation(FVector(0.0f, 250.0f, 250.0f)); // 오른쪽 벽
	WallRightMesh->SetRelativeRotation(FRotator(90.0f, 90.0f, 0.0f));

	WallBackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallBackMesh"));
	WallBackMesh->SetupAttachment(DefaultSceneRoot);
	WallBackMesh->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f)); // 뒤쪽 벽
	WallBackMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 90.0f));

	WallFrontMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallFrontMesh"));
	WallFrontMesh->SetupAttachment(DefaultSceneRoot);
	WallFrontMesh->SetRelativeLocation(FVector(250.0f, 0.0f, 250.0f)); // 앞쪽 벽
	WallFrontMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 90.0f));

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

	// this->WallLeftMesh->SetVisibility(false);
	// this->WallLeftMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// this->WallFrontMesh->SetVisibility(false);
	// this->WallFrontMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// this->WallRightMesh->SetVisibility(false);
	// this->WallRightMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// this->WallBackMesh->SetVisibility(false);
	// this->WallBackMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

// Called every frame
void ADungeonRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADungeonRoom::ApplyBranch(int32 Dir)
{
	this->BranchDir = Dir;
	switch (Dir)
	{
	case 0://up
		this->WallLeftMesh->SetVisibility(false, true);
		this->WallLeftMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case 1://right
		this->WallFrontMesh->SetVisibility(false, true);
		this->WallFrontMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case 2://down
		this->WallRightMesh->SetVisibility(false, true);
		this->WallRightMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case 3://left
		this->WallBackMesh->SetVisibility(false, true);
		this->WallBackMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

void ADungeonRoom::RestoreBranch()
{
	switch (this->BranchDir)
	{
	case 0://up
		this->WallLeftMesh->SetVisibility(true, true);
		this->WallLeftMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case 1://right
		this->WallFrontMesh->SetVisibility(true, true);
		this->WallFrontMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case 2://down
		this->WallRightMesh->SetVisibility(true, true);
		this->WallRightMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case 3://left
		this->WallBackMesh->SetVisibility(true, true);
		this->WallBackMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	}
}

void ADungeonRoom::SetDirection()
{
	UE_LOG(LogTemp, Log, TEXT("%d : %d => %d"), this->MapType, this->PrevDirection, this->Direction)


	if (this->MapType != 3 && this->MapType != 6)
	{
		switch (this->Direction)
		{
		case 0://up
			this->WallLeftMesh->SetVisibility(false, true);
			this->WallLeftMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		case 1://right
			this->WallFrontMesh->SetVisibility(false, true);
			this->WallFrontMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		case 2://down
			this->WallRightMesh->SetVisibility(false, true);
			this->WallRightMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		case 3://left
			this->WallBackMesh->SetVisibility(false, true);
			this->WallBackMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		}
	}

	
	
	
	if (this->MapType!=2) //맨 처음 방이 아니면 적어도 두 벽이 뚫려있어야함.
	{
		switch (this->PrevDirection)
		{
		case 0://up
			this->WallRightMesh->SetVisibility(false, true);
			this->WallRightMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		case 1://right
			this->WallBackMesh->SetVisibility(false, true);
			this->WallBackMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		case 2://down
			this->WallLeftMesh->SetVisibility(false, true);
			this->WallLeftMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		case 3://left
			this->WallFrontMesh->SetVisibility(false, true);
			this->WallFrontMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		}
	}
}


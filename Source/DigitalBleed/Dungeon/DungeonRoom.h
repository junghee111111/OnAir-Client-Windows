// Copyright 2025, Ducksoo Games. All RIghts Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonRoom.generated.h"

UCLASS()
class DIGITALBLEED_API ADungeonRoom : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADungeonRoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRoot = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map|Wall")
	UStaticMeshComponent* WallFrontMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map|Wall")
	UStaticMeshComponent* WallBackMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map|Wall")
	UStaticMeshComponent* WallLeftMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map|Wall")
	UStaticMeshComponent* WallRightMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map|Wall")
	UStaticMeshComponent* WallCeilingMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Map|Wall")
	UStaticMeshComponent* WallFloorMesh = nullptr;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	int32 MapType = 0;

	UPROPERTY()
	int32 Direction = 0;

	UPROPERTY()
	int32 PrevDirection = 0;

	UFUNCTION(BlueprintCallable)
	void SetDirection();
};

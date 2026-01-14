// 2016.01.14, Written By Junghee Wang


#include "LifeHuman.h"

#include "Kismet/KismetMathLibrary.h"


// Sets default values
ALifeHuman::ALifeHuman()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALifeHuman::BeginPlay()
{
	Super::BeginPlay();

	FRotator InitLookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector(0,0,0));
	this->SetActorRotation(InitLookAt);
}

// Called every frame
void ALifeHuman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


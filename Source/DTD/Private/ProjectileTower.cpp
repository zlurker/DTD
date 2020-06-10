// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileTower.h"

// Sets default values
AProjectileTower::AProjectileTower()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectileTower::BeginPlay()
{
	Super::BeginPlay();
	AGameplayLevel* gameplayLevel = (AGameplayLevel*)UGameplayStatics::GetGameMode(GetWorld());

	if (gameplayLevel != nullptr)
		gameplayLevel->ReduceCash();
}


void AProjectileTower::TowerAction() {
	UE_LOG(LogTemp, Warning, TEXT("Tower firing."));

	FActorSpawnParameters sP;
	sP.Template = projectile;

	AProjectile* inst = (AProjectile*)GetWorld()->SpawnActor<AProjectile>(GetActorLocation(), GetActorRotation(), sP);

	ATower::TowerAction();
}

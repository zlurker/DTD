// Fill out your copyright notice in the Description page of Project Settings.


#include "CreepBase.h"

// Sets default values
ACreepBase::ACreepBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACreepBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACreepBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACreepBase::ModifyHealth(float value) {
	health -= value;

	if (health <= 0)
		Destroy();
}


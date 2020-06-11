// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATower::ATower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float currTime = UGameplayStatics::GetRealTimeSeconds((UObject*)GetWorld());

	if (prevAction + attackPeriod <= currTime) {
		prevAction = currTime;
		TowerAction();
	}

}

void ATower::TowerAction() {
	//UE_LOG(LogTemp, Warning, TEXT("Tower actions."));
}


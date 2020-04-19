// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTowerBase::UTowerBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTowerBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTowerBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	float currTime = UGameplayStatics::GetRealTimeSeconds((UObject*)GetWorld());

	if (prevAction + attackPeriod <= currTime) {
		prevAction = currTime;
		TowerAction();
	}
}

void UTowerBase::TowerAction() {
	UE_LOG(LogTemp, Warning, TEXT("Tower actions."));
}

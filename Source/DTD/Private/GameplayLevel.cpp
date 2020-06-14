// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayLevel.h"

AGameplayLevel::AGameplayLevel() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AGameplayLevel::BeginPlay() {
	Super::BeginPlay();

	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), StartingWidgetClass);
	CurrentWidget->AddToViewport();
	//ChangeMenuWidget(StartingWidgetClass);
	selectedTowerType = -1;
}

/*void AGameplayLevel::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass) {

	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}*/

void AGameplayLevel::BuildTower(int towerType) {
	UE_LOG(LogTemp, Warning, TEXT("Tower clicked"));
	selectedTowerType = towerType;
}

void AGameplayLevel::ReduceCash() {
	//UE_LOG(LogTemp, Warning, TEXT("Cash has been reduced."));
}

void AGameplayLevel::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	
}

void AGameplayLevel::TriggerBuildTower(FVector towerLoc) {
	if (selectedTowerType > -1) {
		UE_LOG(LogTemp, Warning, TEXT("Tower build triggered."));
		ATower* inst = (ATower*) GetWorld()->SpawnActor(towers[selectedTowerType], &towerLoc);
		 
		selectedTowerType = 0;
	}
	
}
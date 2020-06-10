// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayLevel.h"

void AGameplayLevel::BeginPlay() {
	Super::BeginPlay();
	ChangeMenuWidget(StartingWidgetClass);
}

void AGameplayLevel::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass) {

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
}

void AGameplayLevel::ReduceCash() {
	UE_LOG(LogTemp, Warning, TEXT("Cash has been reduced."));
}
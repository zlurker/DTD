// Fill out your copyright notice in the Description page of Project Settings.


#include "UMGTest.h"

void AUMGTest::BeginPlay() {
	Super::BeginPlay();
	SetInputMode(FInputModeGameAndUI());
}

void AUMGTest::SetupInputComponent() {
	Super::SetupInputComponent();

	InputComponent->BindAction("BuildTower", IE_Pressed,this, &AUMGTest::BuildTower);
}

void AUMGTest::BuildTower() {
	AGameplayLevel* gameplayLevel = (AGameplayLevel*)UGameplayStatics::GetGameMode(GetWorld());
	gameplayLevel->TriggerBuildTower();
	//if (gameplayLevel != nullptr)
		//gameplayLevel->ReduceCash();
}



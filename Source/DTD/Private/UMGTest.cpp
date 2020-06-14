// Fill out your copyright notice in the Description page of Project Settings.


#include "UMGTest.h"

void AUMGTest::BeginPlay() {
	Super::BeginPlay();
	SetInputMode(FInputModeGameAndUI());
}

void AUMGTest::SetupInputComponent() {
	Super::SetupInputComponent();

	InputComponent->BindAction("BuildTower", IE_Pressed, this, &AUMGTest::BuildTower);
}

void AUMGTest::BuildTower() {
	AGameplayLevel* gameplayLevel = (AGameplayLevel*)UGameplayStatics::GetGameMode(GetWorld());
	gameplayLevel->TriggerBuildTower(CameraRaycastToObject());
	//if (gameplayLevel != nullptr)
		//gameplayLevel->ReduceCash();
}

FVector AUMGTest::CameraRaycastToObject() {
	// FVector worldPos, worldDir;
	// DeprojectMousePositionToWorld(worldPos, worldDir);

	// float sFactor = worldPos.Z / worldDir.Z;
	// worldPos - (worldDir * sFactor);

	FHitResult hitpoint;
	GetHitResultUnderCursor(ECC_WorldStatic, false, hitpoint);
	return hitpoint.ImpactPoint;
}



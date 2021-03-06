// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayLevel.h"
#include "Kismet/GameplayStatics.h"
#include "UMGTest.generated.h"

/**
 * 
 */
UCLASS()
class DTD_API AUMGTest : public APlayerController
{
	GENERATED_BODY()
	
	public:
		virtual void BeginPlay() override;
		virtual void SetupInputComponent() override;
		FVector CameraRaycastToObject();
		void BuildTower();
};

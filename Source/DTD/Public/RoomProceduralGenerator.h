// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
#include "RoomProceduralGenerator.generated.h"

/**
 * 
 */
UCLASS()
class DTD_API ARoomProceduralGenerator : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Wall")
	TSubclassOf<AActor> wall;

protected:
	/** Called when the game starts. */
	virtual void BeginPlay() override;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tower.h"
#include "Engine/World.h"
#include "Projectile.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h "
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayLevel.h"
#include "ProjectileTower.generated.h"

UCLASS()
class DTD_API AProjectileTower : public ATower
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileTower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void TowerAction() override;

public:	

	// Common projectile tower stats
	UPROPERTY(EditAnywhere, Category = "Stats")
	class AActor* projectile;
};

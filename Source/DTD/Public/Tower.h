// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Tower.generated.h"

UCLASS()
class DTD_API ATower : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void TowerAction();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// Common tower stats
	UPROPERTY(EditAnywhere, Category = "Stats")
		float attackPeriod;

	UPROPERTY(EditAnywhere, Category = "Stats")
		float towerCost;

	UPROPERTY(EditAnywhere, Category = "Stats")
		float towerAttackRadius;

	UPROPERTY(EditAnywhere, Category = "Tower Behaviour")
		bool enableTargetting;

	
private:
	// Internal variables
	float prevAction;
	USphereComponent* attackRadius;
};

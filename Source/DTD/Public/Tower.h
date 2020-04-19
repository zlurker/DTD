// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

private:
	// Internal variables
	float prevAction;

};

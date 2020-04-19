// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TowerBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DTD_API UTowerBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTowerBase();

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TowerAction();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	// Common tower stats
	UPROPERTY(EditAnywhere, Category = "Stats")
	float attackPeriod;


private:
	// Internal variables
	float prevAction;		
};

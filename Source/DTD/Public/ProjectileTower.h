// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TowerBase.h"
#include "ProjectileTower.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DTD_API UProjectileTower : public UTowerBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProjectileTower();

protected:
	// Called when the game starts
	virtual void TowerAction() override;

		
};

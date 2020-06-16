// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectileEffectBase.h"
#include "DestroyActorOnCalledXTimes.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DTD_API UDestroyActorOnCalledXTimes : public UProjectileEffectBase
{
	GENERATED_BODY()

public:
	virtual void ApplyEffect(AActor* target) override;

public:
	UPROPERTY(EditAnywhere, Category = "Destroy Settings")
		int destroyWhenCalledXTimes =1;

private:
	int internalCounter;

};

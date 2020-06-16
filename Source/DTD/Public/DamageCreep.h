// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectileEffectBase.h"
#include "CreepBase.h"
#include "DamageCreep.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DTD_API UDamageCreep : public UProjectileEffectBase
{
	GENERATED_BODY()

public:
	virtual void ApplyEffect(AActor* target) override;

public:
	UPROPERTY(EditAnywhere, Category = "Damage Settings")
		float damage;
};

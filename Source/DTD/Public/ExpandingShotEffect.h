// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectileEffectBase.h"
#include "ExpandingShotEffect.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DTD_API UExpandingShotEffect : public UProjectileEffectBase
{
	GENERATED_BODY()

public:
	virtual void ApplyEffect(AActor* target) override;

public:
	UPROPERTY(EditAnywhere, Category = "Expand Settings")
		FVector expandSize;

};
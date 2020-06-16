// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectileEffectBase.generated.h"

UENUM()
enum EffectType {
	Update, OnHit
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DTD_API UProjectileEffectBase : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void ApplyEffect(AActor* target);

public:
	UPROPERTY(EditAnywhere, Category = "Effect Type")
		TEnumAsByte<EffectType> projectileEffectType;

};

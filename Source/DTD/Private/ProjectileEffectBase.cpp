// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEffectBase.h"


// Called when the game starts
void UProjectileEffectBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UProjectileEffectBase::ApplyEffect(AActor* target) {
	GetOwner()->SetActorScale3D(GetOwner()->GetActorScale3D() + FVector::ForwardVector);
}


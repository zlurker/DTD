// Fill out your copyright notice in the Description page of Project Settings.


#include "ExpandingShotEffect.h"

void UExpandingShotEffect::ApplyEffect(AActor* target) {
	GetOwner()->SetActorScale3D(GetOwner()->GetActorScale3D() + expandSize);
}


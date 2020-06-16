// Fill out your copyright notice in the Description page of Project Settings.


#include "LinearMovementEffect.h"

void ULinearMovementEffect::ApplyEffect(AActor* target) {
	if (target != nullptr) {

		FVector dir;

		if (seeking) {
			dir = target->GetActorLocation() - GetOwner()->GetActorLocation();
			dir.Normalize(0);
		}
		else {
			if (!initialDirectionCalculated) {
				direction = target->GetActorLocation() - GetOwner()->GetActorLocation();
				direction.Normalize();
				initialDirectionCalculated = true;
			}

			dir = direction;
		}

		GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + dir);
	}
}

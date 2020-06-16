// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyActorOnCalledXTimes.h"

void UDestroyActorOnCalledXTimes::ApplyEffect(AActor* target) {
	internalCounter++;

	if (internalCounter >= destroyWhenCalledXTimes)
		GetOwner()->Destroy();
}


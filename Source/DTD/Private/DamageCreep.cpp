// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageCreep.h"

void UDamageCreep::ApplyEffect(AActor* target) {
	ACreepBase* creep = Cast<ACreepBase>(target);

	//UE_LOG(LogTemp, Log, TEXT("Something has left range of tower."));

	if (creep != nullptr) 
		creep->ModifyHealth(damage);
}


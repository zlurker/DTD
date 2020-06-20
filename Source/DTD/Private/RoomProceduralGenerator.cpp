// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomProceduralGenerator.h"

void ARoomProceduralGenerator::BeginPlay() {
	float min = -1250;
	float max = 1250;
	UE_LOG(LogTemp, Log, TEXT("Rand Test: %f"), FMath::RandRange(min, max));
}
	
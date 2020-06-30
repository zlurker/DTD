// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class DTD_API BiomesData
{
public:
	BiomesData();
	~BiomesData();
	void AddBiomeVertice(int vertId, float height);

public:
	TArray<int> biomeVertices;
	int peakVertice;

private:
	float prevPeak;
};

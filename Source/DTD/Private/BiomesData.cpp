// Fill out your copyright notice in the Description page of Project Settings.


#include "BiomesData.h"

BiomesData::BiomesData()
{
	prevPeak = 0;
}

BiomesData::~BiomesData()
{
}

void BiomesData::AddBiomeVertice(int vertId, float height) {
	if (height > prevPeak) {
		prevPeak = height;
		peakVertice = vertId;
	}

	biomeVertices.Add(vertId);
}

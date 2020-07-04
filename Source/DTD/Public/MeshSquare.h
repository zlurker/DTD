// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */
class DTD_API MeshSquare
{
public:
	MeshSquare(int bL, int uL, int bR, int uR, float p);
	~MeshSquare();

public:
	int bottomLeft;
	int upperLeft;
	int bottomRight;
	int upperRight;
	float peak;
	bool sqrOccupied;

};

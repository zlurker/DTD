// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshSquare.h"

MeshSquare::MeshSquare(int bL, int uL, int bR, int uR, float p)
{
	bottomLeft = bL;
	upperLeft = uL;
	bottomRight = bR;
	upperRight = uR;
	peak = p;
}

MeshSquare::~MeshSquare()
{
}

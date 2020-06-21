// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainGenerator.h"

ATerrainGenerator::ATerrainGenerator()
{
	PrimaryActorTick.bCanEverTick = true;

	pm = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	pm->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

}

void ATerrainGenerator::OnConstruction(const FTransform& Transform)
{
	ClearMeshData();


	float chasmRatio = 0.2f;
	int chasmCount = 1;
	int totalVertices = verticeDimensionX * verticeDimensionY;

	// Populates vertices.
	for (int i = 0; i < verticeDimensionX; i++)
		for (int j = 0; j < verticeDimensionY; j++)
			// Perlin noise to give terrain a more rocky feel
			vertices.Add(FVector(plotSpace * i, plotSpace * j, FMath::PerlinNoise2D(FVector2D(i * 0.05f, j * 0.05f)) * 200));


	// Adds chasms
	int remainingPoints = totalVertices * chasmRatio;

	for (int i = 0; i < chasmCount; i++) {
		int pointsUsed = i == chasmCount - 1 ? remainingPoints : FMath::RandRange(0, remainingPoints);
		int randPt = FMath::RandRange(0, totalVertices);
		TArray<int> expansion;
		expansion.Init(randPt, 8);
		UE_LOG(LogTemp, Log, TEXT("Building chasm. Points used: %d/%d"), pointsUsed, remainingPoints);

		for (int j = 0; j < pointsUsed; j++) {

			int randDir = FMath::RandRange(0, 7);
			int currCoor[2];
			GetCoordinatePosition(expansion[randDir], currCoor);
			int nextCoord = -1;

			//UE_LOG(LogTemp, Log, TEXT("CoordX: %d, CoordY: %d"), currCoor[0], currCoor[1]);

			switch (randDir) {
			case 0:
				currCoor[1] += 1;
				nextCoord = GetIndex(currCoor);
				break;

			case 1:
				currCoor[0] += 1;
				currCoor[1] += 1;
				nextCoord = GetIndex(currCoor);
				break;

			case 2:
				currCoor[0] += 1;
				nextCoord = GetIndex(currCoor);
				break;

			case 3:
				currCoor[0] += 1;
				currCoor[1] += -1;
				nextCoord = GetIndex(currCoor);
				break;

			case 4:
				currCoor[1] += -1;
				nextCoord = GetIndex(currCoor);
				break;

			case 5:
				currCoor[0] += -1;
				currCoor[1] += -1;
				nextCoord = GetIndex(currCoor);
				break;

			case 6:
				currCoor[0] += -1;
				nextCoord = GetIndex(currCoor);
				break;

			case 7:
				currCoor[0] += -1;
				currCoor[1] += 1;
				nextCoord = GetIndex(currCoor);
				break;
			}

			if (nextCoord < 0 || nextCoord >= totalVertices)
				continue;

			vertices[nextCoord].Z = -500;
			//UE_LOG(LogTemp, Log, TEXT("This coord has been modified: %d"), nextCoord);
			expansion[randDir] = nextCoord;
		}

		remainingPoints -= pointsUsed;
	}

	// Adds hilly terrain

	// Begins linkage creation
	for (int i = 0; i < verticeDimensionX - 1; i++)
		for (int j = 0; j < verticeDimensionY - 1; j++) {

			int index0 = (i * verticeDimensionY) + j;
			int index1 = index0 + 1;
			int index2 = index0 + verticeDimensionY;
			int index3 = index2 + 1;

			//vertices[i] = -1;

			triangles.Add(index0);
			triangles.Add(index1);
			triangles.Add(index2);

			triangles.Add(index3);
			triangles.Add(index2);
			triangles.Add(index1);
		}

	/*vertices.Add(FVector(0.0f, 0.0f, 0.0f));
	vertices.Add(FVector(0.0f, 100.0f, 0.0f));
	vertices.Add(FVector(100.0f, 0.0f, 0.0f));
	vertices.Add(FVector(100.0f, 100.0f, 0.0f));

	triangles.Add(0);
	triangles.Add(1);
	triangles.Add(2);

	triangles.Add(3);
	triangles.Add(2);
	triangles.Add(1);*/

	normals.Init(FVector(1, 0.0f, 0.0f), 4);

	uvs.Add(FVector2D(0.0f, 0.0f));
	uvs.Add(FVector2D(1.0f, 0.0f));
	uvs.Add(FVector2D(0.0f, 1.0f));
	uvs.Add(FVector2D(1.0f, 1.0f));

	//uvs.Init(FVector2D(0.0f, 0.0f), 3);

	//vertexColors.Init(FLinearColor(0.75f, 0.0f, 0.0f, 1.0f), 4);

	vertexColors.Add(FLinearColor(1, 0, 0, 1.0));
	vertexColors.Add(FLinearColor(1, 0, 0, 1.0));
	vertexColors.Add(FLinearColor(0, 1, 0, 1.0));                              // the 4th argument determines alpha value (0,1)
	vertexColors.Add(FLinearColor(1, 1, 0, 1.0));

	tangents.Init(FProcMeshTangent(0.0f, 1.0f, 0.0f), 4);

	//Function that creates mesh section
	pm->CreateMeshSection_LinearColor(1, vertices, triangles, normals, uvs, vertexColors, tangents, false);
}

void ATerrainGenerator::GetCoordinatePosition(int index, int* coordinates) {
	coordinates[1] = index % verticeDimensionX;
	coordinates[0] = (index - coordinates[1]) / verticeDimensionX;
}

int ATerrainGenerator::GetIndex(int* coordinates) {
	return (coordinates[0] * verticeDimensionX) + coordinates[1];
}


void ATerrainGenerator::ClearMeshData()
{
	vertices.Empty();
	triangles.Empty();
	uvs.Empty();
	normals.Empty();
	vertexColors.Empty();
	tangents.Empty();
}


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


	int chasmCount = 5;
	float hillRatio = 0.2f;
	int hillCount = 5;

	int totalVertices = verticeDimensionX * verticeDimensionY;

	// Populates vertices.
	for (int i = 0; i < verticeDimensionX; i++)
		for (int j = 0; j < verticeDimensionY; j++) {
			UE_LOG(LogTemp, Log, TEXT("Perlin Float Test : %f"), FMath::PerlinNoise2D(FVector2D(i * 0.05f, j * 0.05f)) * 1000);
			vertices.Add(FVector(plotSpace * i, plotSpace * j, FMath::PerlinNoise2D(FVector2D(i * 0.05f, j * 0.05f)) * 1000));
		}

	// Adds chasms
	//int remainingPoints = totalVertices * hillRatio;

	//for (int i = 0; i < chasmCount; i++) {
		//int pointsUsed = FMath::RandRange(0, remainingPoints);


		//remainingPoints -= pointsUsed;
	//}

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


void ATerrainGenerator::ClearMeshData()
{
	vertices.Empty();
	triangles.Empty();
	uvs.Empty();
	normals.Empty();
	vertexColors.Empty();
	tangents.Empty();
}


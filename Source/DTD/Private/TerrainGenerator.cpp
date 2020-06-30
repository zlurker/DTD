// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainGenerator.h"

ATerrainGenerator::ATerrainGenerator()
{
	PrimaryActorTick.bCanEverTick = true;

	pm = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	pm->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

}

void ATerrainGenerator::GeneratePerlinShapeIsland() {
	int radius = 40;
	int seed = 10;
	float noiseMultiplier = 20;

	int centerX = verticeDimensionX / 2;
	int centerY = verticeDimensionY / 2;

	UE_LOG(LogTemp, Log, TEXT("Curr Seed 0: %d"), seed);

	//int totalcount = 0;

	// Populates vertices.
	for (int i = 0; i < verticeDimensionX; i++)
		for (int j = 0; j < verticeDimensionY; j++) {
			int reposX = i - centerX;
			int reposY = j - centerY;

			float rad = FMath::Atan2(radius, 0) - FMath::Atan2(reposY, reposX);
			float radiusLimit = radius + (FMath::PerlinNoise1D(seed + rad) * noiseMultiplier);

			FVector vectorDiff = FVector(centerX, centerY, 0) - FVector(i, j, 0);

			float pointZ = -500;

			if (vectorDiff.SizeSquared() <= (radiusLimit * radiusLimit)) {
				float freqMultiplier = 1;
				int internalFreq = 1;
				int freq = 5;

				for (int k = 0; k < freq; k++) {
					pointZ = FMath::PerlinNoise2D(FVector2D(seed + i, seed + j) * 0.075f * internalFreq) * freqMultiplier * 1000;
					freqMultiplier /= 2;
					internalFreq *= 2;
				}

				UE_LOG(LogTemp, Log, TEXT("Curr Z: %f"), pointZ);
			}



			// Perlin noise to give terrain a more rocky feel
			vertices.Add(FVector(plotSpace * i, plotSpace * j, pointZ));

			//int coordinatesX = centerX + i;
			//int coordinatesY = centerY + j;

			//float dot = (coordinatesX * coordinatesY) + (centerX * (centerY + radius));
			//float det = (coordinatesX * coordinatesY) - (centerX * (centerY + radius));
			//FVector posFromCentre = FVector(i - centerX, j - centerY,0);
			//float angle = posFromCentre.HeadingAngle();

			//float dP = (reposX * 0) + (reposY * radius);
			//float cP = (reposY * 0) - (reposX * radius);

			//float angle = atan2(centerY + radius, centerX) - atan2(j, i);

			//if (angle < 0)
				//angle += PI * 2;

			//float angle = FMath::RadiansToDegrees(FMath::Atan2(FMath::Abs(cP), dP));//FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(dp1, dp2)));


			//if (angle < 0)
				//totalcount++;

			//UE_LOG(LogTemp, Log, TEXT("Angle0: %f, Given Coord set: %d, %d"), angle, reposX, reposY);
			//FMath::P
		}
}

void ATerrainGenerator::GeneratePushedElevationIsland() {
	int centralX = verticeDimensionX / 2;
	int centralY = verticeDimensionY / 2;
	int seed = 20193;
	currentBiomes = 0;

	UE_LOG(LogTemp, Log, TEXT("Formula Test: %d, Seed: %d"), 27, seed);
	for (int i = 0; i < verticeDimensionX; i++) {
		int xFromCentral = FMath::Abs(centralX - i);

		for (int j = 0; j < verticeDimensionY; j++) {
			int yFromCentral = FMath::Abs(centralY - j);
			float elevation = ElevationClamp(FVector(xFromCentral, yFromCentral, 0), FMath::PerlinNoise2D(FVector2D(seed + i, seed + j) * 0.1f));

			vertices.Add(FVector(plotSpace * i, plotSpace * j, elevation * 500));
			int currVertice = vertices.Num() - 1;
			/*
			Checks neighbouring vertices that has been spawned.
			Where X is before, O is after and T is target.
			XOO
			XTO
			XXO
			*/
			if (BiomeCheckNeighbourVertice(currVertice, FVector2D(0.f, -1.f))) continue;
			if (BiomeCheckNeighbourVertice(currVertice, FVector2D(-1.f, -1.f))) continue;
			if (BiomeCheckNeighbourVertice(currVertice, FVector2D(-1.f, 0.f))) continue;
			if (BiomeCheckNeighbourVertice(currVertice, FVector2D(-1.f, 1.f))) continue;

			// If none of the above direction gives us anything, it means we need to start a new biome data.
			//biomesData.Add(&BiomeDataCreator());
			//int assignedBiome = biomesData.Num() - 1;
			verticeAssignedBiome.Add(currentBiomes);
			//biomesData[assignedBiome]->AddBiomeVertice(currVertice, vertices[currVertice].Z);

			UE_LOG(LogTemp, Log, TEXT("Vertice %d has created a new biome %d"), currVertice, currentBiomes);
			currentBiomes++;
		}
	}
}

BiomesData ATerrainGenerator::BiomeDataCreator() {
	return BiomesData();
}

float ATerrainGenerator::ElevationClamp(FVector vectorFromCenter, float elevation) {

	float totalElevation = elevation + (0.5f - ((vectorFromCenter.SizeSquared() / (FVector(verticeDimensionX, verticeDimensionY, 0).SizeSquared())) * 10));//(((vectorFromCenter.SizeSquared() / (FVector(verticeDimensionX, verticeDimensionY, 0).SizeSquared()/2)) * 2) - 1);

	if (totalElevation < -1)
		totalElevation = -1;
	else if (totalElevation > 1)
		totalElevation = 1;

	//UE_LOG(LogTemp, Log, TEXT("%f ini elevation,%f fin elevation"), elevation, totalElevation);

	return totalElevation;
}

void ATerrainGenerator::OnConstruction(const FTransform& Transform)
{
	ClearMeshData();
	GeneratePushedElevationIsland();

	for (int i = 0; i < verticeDimensionX; i++) {
		int uvX = i % 2;
		for (int j = 0; j < verticeDimensionY; j++) {
			//vertices.Add(FVector(plotSpace * i, plotSpace * j, 0));
			int uvY = j % 2;
			//float x = ((float)i / (float)verticeDimensionX);
			//float y = ((float)j / (float)verticeDimensionY);
			FVector2D coords = FVector2D(i, j);

			float baseFloat = (float)(verticeAssignedBiome[GetIndex(FVector2D(i, j))] % 5) / 5.f;
			UE_LOG(LogTemp, Log, TEXT("BF: %f"), baseFloat);
			FVector2D base = FVector2D(baseFloat, 0);
			uvs.Add(base + (FVector2D(uvX *0.2f, uvY)));
			//UE_LOG(LogTemp, Log, TEXT("X: %d, Y: %d"), uvX, uvY);
		}
	}



	//UE_LOG(LogTemp, Log, TEXT("TC: %d"), totalcount);



	/*for (float rad = 0; rad < 6.28318531; rad += 0.1) {
		float fR = radius + (FMath::PerlinNoise1D(seed + rad) * 20);



		//float value = FMath::PerlinNoise2D(FVector2D(i * 0.01f, j * 0.01f));

		//float finalZ = value >= 0 ? 500 : -500;
		int x = fR * FMath::Sin(rad);
		int y = fR * FMath::Cos(rad);
		int coordinates[2];

		coordinates[0] = centerX + x;
		coordinates[1] = centerY + y;

		FVector dp1 = FVector(coordinates[0], coordinates[1], 0);
		FVector dp2 = FVector(centerX, (centerY + radius), 0);

		//float dot = (coordinates[0] *  coordinates[1]) + (centerX * (centerY + radius));
		//float det = (coordinates[0] * coordinates[1]) - (centerX * (centerY + radius));
		float angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(dp1,dp2)));

		vertices[GetIndex(coordinates)].Z = -500;
	}*/


	// Adds chasms
	/*int remainingPoints = totalVertices * chasmRatio;

	for (int i = 0; i < chasmCount; i++) {
		int pointsUsed = i == chasmCount - 1 ? remainingPoints : FMath::RandRange(0, remainingPoints);
		int randPt = FMath::RandRange(0, totalVertices);
		TArray<int> expansion;
		expansion.Init(randPt, 8);
		UE_LOG(LogTemp, Log, TEXT("Building chasm. Points used: %d/%d"), pointsUsed, remainingPoints);

		int randLineLen = FMath::RandRange(0, verticeDimensionY);
		int randLineLenPosX = FMath::RandRange(0, verticeDimensionX);
		int randLineLenPosY = FMath::RandRange(0, verticeDimensionY);

		for (int j = randLineLenPosY; j < randLineLenPosY + randLineLen; j++) {
			int pointsUsedForChasm = j == randLineLenPosY + randLineLen - 1 ? pointsUsedForChasm : FMath::RandRange(0, pointsUsed);
			//int randOffset = FMath::RandRange(0, pointsUsedForChasm);

			for (int k = randLineLenPosX; k < randLineLenPosX + pointsUsedForChasm; k++) {
				int coords[2];
				coords[0] = k;
				coords[1] = j;

				if (k < 0 || k >= verticeDimensionX || j < 0 || j >= verticeDimensionY)
					continue;
				vertices[GetIndex(coords)].Z = -500;
			}

			pointsUsed -= pointsUsedForChasm;
		}*/


		/*for (int j = 0; j < pointsUsed; j++) {

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
		}*/

		//remainingPoints -= pointsUsed;


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
			//vertexColors.Add(FLinearColor(255.0f, 0.f, 0.f));

			/*int vertexCoords[2];

			vertexCoords[0] = i;
			vertexCoords[1] = j;

			if (vertices[GetIndex(vertexCoords)].Z >= 0)
				vertexColors.Add(FColor(255, 0, 0, 1));
			else
				vertexColors.Add(FColor(255, 0, 0, 1));*/
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
	//uvs.Init(FVector2D(0.0f, 0.0f), verticeDimensionX * verticeDimensionY);

	//uvs.Add(FVector2D(0.0f, 0.0f));
	//uvs.Add(FVector2D(1.0f, 0.0f));
	//uvs.Add(FVector2D(0.0f, 1.0f));
	//uvs.Add(FVector2D(1.0f, 1.0f));
	//uvs.Init(FVector2D(0.0f, 0.0f), 3);	

	/*vertexColors.Add(FLinearColor(0.f, 0.f, 1.f));
	vertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	vertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	vertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	vertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	vertexColors.Add(FLinearColor(1.f, 1.f, 0.f));
	vertexColors.Add(FLinearColor(0.f, 1.f, 1.f));*/

	//vertexColors.Add(FLinearColor(1, 0, 0, 1.0));
	//vertexColors.Add(FLinearColor(0, 1, 0, 1.0));                              // the 4th argument determines alpha value (0,1)
	//vertexColors.Add(FLinearColor(1, 1, 0, 1.0));

	//vertexColors.Init(FLinearColor(255, 0, 0), verticeDimensionX * verticeDimensionY);

	normals.Init(FVector(0.0f, 0.0f, 1.0f), verticeDimensionX * verticeDimensionY);
	vertexColors.Init(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), verticeDimensionX * verticeDimensionY);
	tangents.Init(FProcMeshTangent(1.0f, 0.0f, 0.0f), verticeDimensionX * verticeDimensionY);

	//Function that creates mesh section
	pm->CreateMeshSection_LinearColor(0, vertices, triangles, normals, uvs, vertexColors, tangents, false);
}

bool ATerrainGenerator::BiomeCheckNeighbourVertice(int vertice, FVector2D direction) {

	//UE_LOG(LogTemp, Log, TEXT("PRE: Vertice: %d, Direction X: %f, Y: %f"), vertice, direction.X, direction.Y);

	FVector2D coordinates;
	GetCoordinatePosition(vertice, &coordinates);

	coordinates += direction;

	if (coordinates.X < 0 || coordinates.Y < 0 || coordinates.X >= verticeDimensionX || coordinates.Y >= verticeDimensionY)
		return false;

	int targetVert = GetIndex(coordinates);

	// We can insert a different perlin layer into this after the prototype works.
	int vertSign = vertices[vertice].Z / FMath::Abs(vertices[vertice].Z);
	int tVertSign = vertices[targetVert].Z / FMath::Abs(vertices[targetVert].Z);

	if (vertSign != tVertSign)
		return false;

	int targetVertBiome = verticeAssignedBiome[targetVert];
	//biomesData[targetVertBiome]->AddBiomeVertice(vertice, vertices[vertice].Z);
	verticeAssignedBiome.Add(targetVertBiome);
	//UE_LOG(LogTemp, Log, TEXT("Vertice: %d, Direction X: %f, Y: %f, Index returned: %d, Assigned Biome: %d"), vertice, direction.X, direction.Y, targetVert, targetVertBiome);
	return true;
}

void ATerrainGenerator::GetCoordinatePosition(int index, FVector2D* coordinates) {
	coordinates->Y = index % verticeDimensionX;
	coordinates->X = (index - coordinates->Y) / verticeDimensionX;
}

int ATerrainGenerator::GetIndex(FVector2D coordinates) {
	return (coordinates.X * verticeDimensionX) + coordinates.Y;
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


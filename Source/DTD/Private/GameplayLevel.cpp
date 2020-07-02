// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayLevel.h"

AGameplayLevel::AGameplayLevel() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AGameplayLevel::BeginPlay() {
	Super::BeginPlay();

	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), StartingWidgetClass);
	CurrentWidget->AddToViewport();
	//ChangeMenuWidget(StartingWidgetClass);
	selectedTowerType = -1;

	GenerateLevel();
}

void AGameplayLevel::GenerateLevel() {
	ATerrainChunk* terrain = GetWorld()->SpawnActor<ATerrainChunk>(ATerrainChunk::StaticClass());

	if (directions.Num() == 0) {
		directions.Add(FVector2D(0, 1));
		directions.Add(FVector2D(1, 1));
		directions.Add(FVector2D(1, 0));
		directions.Add(FVector2D(1, -1));
		directions.Add(FVector2D(0, -1));
		directions.Add(FVector2D(-1, -1));
		directions.Add(FVector2D(-1, 0));
		directions.Add(FVector2D(-1, 1));
	}

	if (squareBase.Num() == 0) {
		squareBase.Add(FVector2D(-1, 1));
		squareBase.Add(FVector2D(1, 1));
		squareBase.Add(FVector2D(1, -1));
		squareBase.Add(FVector2D(-1, -1));
	}

	if (squareDir.Num() == 0) {
		squareDir.Add(FVector2D(1, 0));
		squareDir.Add(FVector2D(0, -1));
		squareDir.Add(FVector2D(-1, 0));
		squareDir.Add(FVector2D(0, 1));
	}

	peaks.Empty();

	GeneratePushedElevationIsland();

	uvs.Init(FVector2D(0, 0), verticeDimensionX * verticeDimensionY);
	TArray<bool> set;
	set.Init(false, verticeDimensionX * verticeDimensionY);

	int exp = 0;

	while (peaks.Num() > 0) {
		for (int i = peaks.Num() - 1; i >= 0; i--) {
			int lenOfSquare = 1 + (exp * 2);
			int totalFilled = 0;
			FVector2D peakLocation;

			GetCoordinatePosition(peaks[i], &peakLocation);

			for (int j = 0; j < 4; j++) {
				FVector2D baseOffset = (squareBase[j] * exp);

				for (int k = 0; k < lenOfSquare; k++) {
					FVector2D currPos = peakLocation + baseOffset + (squareDir[j] * k);

					if (!IsCoordinateWithinBounds(currPos))
						continue;

					int vertexIndex = GetIndex(currPos);


					if (!set[vertexIndex]) {
						float baseFloat = (float)(i % 5) / 5.f;
						//UE_LOG(LogTemp, Log, TEXT("BF: %f"), baseFloat);
						FVector2D base = FVector2D(baseFloat, 0);

						int uvX = (int)currPos.X % 2;
						int uvY = (int)currPos.Y % 2;
						uvs[vertexIndex] = (base + (FVector2D(uvX *0.2f, uvY)));
						totalFilled++;
						set[vertexIndex] = true;
					}
				}
			}

			if (totalFilled == 0)
				peaks.RemoveAt(i);
		}

		exp++;
	}

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

	normals.Init(FVector(0.0f, 0.0f, 1.0f), verticeDimensionX * verticeDimensionY);
	vertexColors.Init(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), verticeDimensionX * verticeDimensionY);
	tangents.Init(FProcMeshTangent(1.0f, 0.0f, 0.0f), verticeDimensionX * verticeDimensionY);

	//Function that creates mesh section
	terrain->cachedMesh->CreateMeshSection_LinearColor(0, vertices, triangles, normals, uvs, vertexColors, tangents, false);
	//terrain->AddComponent(
}

void AGameplayLevel::GeneratePushedElevationIsland() {
	int centralX = verticeDimensionX / 2;
	int centralY = verticeDimensionY / 2;
	//currentBiomes = 0;

	UE_LOG(LogTemp, Log, TEXT("Formula Test: %d, Seed: %d"), 41, generationSeed);
	for (int i = 0; i < verticeDimensionX; i++) {
		int xFromCentral = FMath::Abs(centralX - i);

		for (int j = 0; j < verticeDimensionY; j++) {
			int yFromCentral = FMath::Abs(centralY - j);
			float elevation = ElevationClamp(FVector(xFromCentral, yFromCentral, 0), FMath::PerlinNoise2D(FVector2D(generationSeed + i, generationSeed + j) * 0.1f));

			vertices.Add(FVector(plotDistance * i, plotDistance * j, elevation * 500));
			int currVertice = vertices.Num() - 1;
			/*
			Checks prev x-1,y-1 vertice
			Where X is irrelevant, O is current and T is target.
			XXX
			XOX
			XXT
			*/
			FVector2D prev = FVector2D(i, j) - FVector2D(1, 1);

			if (!IsCoordinateWithinBounds(prev))
				continue;

			int prevVerticeId = GetIndex(prev);

			if (CheckIfVerticeIsPeak(prevVerticeId))
				peaks.Add(prevVerticeId);

			//if (BiomeCheckNeighbourVertice(currVertice, FVector2D(0.f, -1.f))) continue;
			//if (BiomeCheckNeighbourVertice(currVertice, FVector2D(-1.f, -1.f))) continue;
			//if (BiomeCheckNeighbourVertice(currVertice, FVector2D(-1.f, 0.f))) continue;
			//if (BiomeCheckNeighbourVertice(currVertice, FVector2D(-1.f, 1.f))) continue;

			// If none of the above direction gives us anything, it means we need to start a new biome data.
			//biomesData.Add(&BiomeDataCreator());
			//int assignedBiome = biomesData.Num() - 1;

			//biomesData[assignedBiome]->AddBiomeVertice(currVertice, vertices[currVertice].Z);

			//UE_LOG(LogTemp, Log, TEXT("Vertice %d has created a new biome %d"), currVertice, currentBiomes);
			//verticeAssignedBiome.Add(currentBiomes);
			//currentBiomes++;
		}
	}
}

/*void AGameplayLevel::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass) {

	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}*/

float AGameplayLevel::ElevationClamp(FVector vectorFromCenter, float elevation) {

	float totalElevation = elevation + (0.5f - ((vectorFromCenter.SizeSquared() / (FVector(verticeDimensionX, verticeDimensionY, 0).SizeSquared())) * 10));//(((vectorFromCenter.SizeSquared() / (FVector(verticeDimensionX, verticeDimensionY, 0).SizeSquared()/2)) * 2) - 1);

	if (totalElevation < -1)
		totalElevation = -1;
	else if (totalElevation > 1)
		totalElevation = 1;

	//UE_LOG(LogTemp, Log, TEXT("%f ini elevation,%f fin elevation"), elevation, totalElevation);

	return totalElevation;
}

bool AGameplayLevel::CheckIfVerticeIsPeak(int vertice) {

	//UE_LOG(LogTemp, Log, TEXT("PRE: Vertice: %d, Direction X: %f, Y: %f"), vertice, direction.X, direction.Y);

	FVector2D coordinates;

	//FVector2D preTargetCoordinates;

	GetCoordinatePosition(vertice, &coordinates);



	for (int i = 0; i < directions.Num(); i++) {

		//UE_LOG(LogTemp, Log, TEXT("Original Coords: X: %f, Y: %f"), coordinates.X, coordinates.Y);
		//UE_LOG(LogTemp, Log, TEXT("Dir Coords: X: %f, Y: %f"), directions[i].X, directions[i].Y);
		FVector2D combinedCoordinates = coordinates + directions[i];
		//UE_LOG(LogTemp, Log, TEXT("Combined Coords: X: %f, Y: %f"), combinedCoordinates.X, combinedCoordinates.Y);

		if (!IsCoordinateWithinBounds(combinedCoordinates))
			continue;

		//UE_LOG(LogTemp, Log, TEXT("Peak test: %d: %f, %d: %f"), GetIndex(combinedCoordinates), vertices[GetIndex(combinedCoordinates)].Z, vertice, vertices[vertice].Z);
		if (vertices[GetIndex(combinedCoordinates)].Z >= vertices[vertice].Z)
			return false;
	}

	return true;
}

void AGameplayLevel::GetCoordinatePosition(int index, FVector2D* coordinates) {
	coordinates->Y = index % verticeDimensionX;
	coordinates->X = (index - coordinates->Y) / verticeDimensionX;
}

int AGameplayLevel::GetIndex(FVector2D coordinates) {
	return (coordinates.X * verticeDimensionX) + coordinates.Y;
}

bool AGameplayLevel::IsCoordinateWithinBounds(FVector2D coordinate) {
	if (coordinate.X < 0 || coordinate.Y < 0 || coordinate.X >= verticeDimensionX || coordinate.Y >= verticeDimensionY)
		return false;

	return true;
}


void AGameplayLevel::BuildTower(int towerType) {
	UE_LOG(LogTemp, Warning, TEXT("Tower clicked"));
	selectedTowerType = towerType;
}

void AGameplayLevel::ReduceCash() {
	//UE_LOG(LogTemp, Warning, TEXT("Cash has been reduced."));
}

void AGameplayLevel::TriggerBuildTower(FVector towerLoc) {
	if (selectedTowerType > -1) {
		UE_LOG(LogTemp, Warning, TEXT("Tower build triggered."));
		ATower* inst = (ATower*)GetWorld()->SpawnActor(towers[selectedTowerType], &towerLoc);

		selectedTowerType = -1;
	}

}
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

	directions.Add(FVector2D(0, 1));
	directions.Add(FVector2D(1, 1));
	directions.Add(FVector2D(1, 0));
	directions.Add(FVector2D(1, -1));
	directions.Add(FVector2D(0, -1));
	directions.Add(FVector2D(-1, -1));
	directions.Add(FVector2D(-1, 0));
	directions.Add(FVector2D(-1, 1));

	squareBase.Add(FVector2D(-1, 1));
	squareBase.Add(FVector2D(1, 1));
	squareBase.Add(FVector2D(1, -1));
	squareBase.Add(FVector2D(-1, -1));

	squareDir.Add(FVector2D(1, 0));
	squareDir.Add(FVector2D(0, -1));
	squareDir.Add(FVector2D(-1, 0));
	squareDir.Add(FVector2D(0, 1));

	GeneratePushedElevationIsland();

	//uvs.Init(FVector2D(0, 0), verticeDimensionX * verticeDimensionY);
	sqrOccupied.Init(false, bottomLeft.Num());

	int exp = 0;
	UE_LOG(LogTemp, Log, TEXT("Total sqrs: %d"), bottomLeft.Num());
	UE_LOG(LogTemp, Log, TEXT("Starting with %d peaks"), peaks.Num());
	int ultiTotal = 0;

	TArray<TArray<FVector2D>> biomeRegions;
	TArray<int> originalMappedId;
	originalMappedId.Init(0, peaks.Num());
	biomeRegions.Init(TArray<FVector2D>(), peaks.Num());
	//biomeRegions.Init(TArray<FVector2D>(), peaks.Num());

	//biomeRegions.Init(TArray<FVector2D>(), peaks.Num());

	while (peaks.Num() > 0) {
		for (int i = peaks.Num() - 1; i >= 0; i--) {

			if (exp == 0)
				originalMappedId[i] = i;

			int lenOfSquare = 1 + (exp * 2);
			int totalFilled = 0;
			FVector2D peakLocation;
			GetCoordinatePosition(peaks[i], &peakLocation, verticeDimensionX - 1);

			for (int j = 0; j < 4; j++) {
				FVector2D baseOffset = peakLocation + (squareBase[j] * exp);

				for (int k = 0; k < lenOfSquare; k++) {
					FVector2D currPos = baseOffset + (squareDir[j] * k);

					//UE_LOG(LogTemp, Log, TEXT("PeakPos X: %f, Y: %f"), peaks[i].X, peaks[i].Y);
					//UE_LOG(LogTemp, Log, TEXT("CurrPos X: %f, Y: %f"), currPos.X, currPos.Y);

					if (!IsCoordinateWithinBounds(currPos, FVector2D(verticeDimensionX - 1, verticeDimensionY - 1)))
						continue;


					//int vertexIndex = GetIndex(currPos);
					//MeshSquare* sqrInst = squares[currPos.X][currPos.Y];
					int selectedSqr = GetIndex(currPos, verticeDimensionX - 1);

					if (!sqrOccupied[selectedSqr]) {

						biomeRegions[originalMappedId[i]].Add(currPos);
						//biomeRegions[actualPeakId[i]].Add(currPos);
						/*float baseFloat = (float)(i % 5) / 5.f;
						//UE_LOG(LogTemp, Log, TEXT("BF: %f"), baseFloat);
						FVector2D base = FVector2D(baseFloat, 0);
						FVector2D bL, uL, bR, uR;

						GetCoordinatePosition(bottomLeft[selectedSqr], &bL, verticeDimensionX - 1);
						GetCoordinatePosition(upperLeft[selectedSqr], &uL, verticeDimensionX - 1);
						GetCoordinatePosition(bottomRight[selectedSqr], &bR, verticeDimensionX - 1);
						GetCoordinatePosition(upperRight[selectedSqr], &uR, verticeDimensionX - 1);

						uvs[bottomLeft[selectedSqr]] = base + FVector2D((int)bL.X % 2 * 0.2f, (int)bL.X % 2);
						uvs[upperLeft[selectedSqr]] = base + FVector2D((int)uL.X % 2 * 0.2f, (int)uL.X % 2);
						uvs[bottomRight[selectedSqr]] = base + FVector2D((int)bR.X % 2 * 0.2f, (int)bR.X % 2);
						uvs[upperRight[selectedSqr]] = base + FVector2D((int)uR.X % 2 * 0.2f, (int)uR.X % 2);

						//uvs[vertexIndex] = (base + (FVector2D(uvX *0.2f, uvY)));*/
						totalFilled++;
						sqrOccupied[selectedSqr] = true;
					}
				}
			}

			ultiTotal += totalFilled;

			if (totalFilled == 0) {
				peaks.RemoveAt(i);
				originalMappedId.RemoveAt(i);
			}
		}

		exp++;
	}




	for (int i = 0; i < biomeRegions.Num(); i++) {

		TArray<FVector> regionVertice;
		TArray<int> regionVerticeTri;
		TArray<FVector> normals;
		TArray<int32> triangles;
		TArray<FVector2D> uvs;
		TArray<FLinearColor> vertexColors;
		TArray<FProcMeshTangent> tangents;
		TMap<int, int> squareVerticeMapper;

		for (int j = 0; j < biomeRegions[i].Num(); j++) {

			int index = GetIndex(biomeRegions[i][j], verticeDimensionX - 1);
			TArray<int> sqrVertices;

			sqrVertices.Add(bottomLeft[index]);
			sqrVertices.Add(upperLeft[index]);
			sqrVertices.Add(bottomRight[index]);
			sqrVertices.Add(upperRight[index]);

			for (int k = 0; k < sqrVertices.Num(); k++) {
				if (!squareVerticeMapper.Contains(sqrVertices[k])) {
					int prev = squareVerticeMapper.Num();
					regionVertice.Add(vertices[sqrVertices[k]]);
					squareVerticeMapper.Add(sqrVertices[k], prev);
					sqrVertices[k] = prev;
				}
				else
					sqrVertices[k] = squareVerticeMapper[sqrVertices[k]];
				//internalIndex = squareVerticeMapper[index];
			}

			regionVerticeTri.Add(sqrVertices[0]);
			regionVerticeTri.Add(sqrVertices[1]);
			regionVerticeTri.Add(sqrVertices[2]);

			regionVerticeTri.Add(sqrVertices[3]);
			regionVerticeTri.Add(sqrVertices[2]);
			regionVerticeTri.Add(sqrVertices[1]);
		}

		normals.Init(FVector(0.0f, 0.0f, 1.0f), regionVertice.Num());
		vertexColors.Init(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), regionVertice.Num());
		tangents.Init(FProcMeshTangent(1.0f, 0.0f, 0.0f), regionVertice.Num());
		uvs.Init(FVector2D(0, 0), regionVertice.Num());

		//Function that creates mesh section
		ATerrainChunk* biomeChunk = GetWorld()->SpawnActor<ATerrainChunk>(ATerrainChunk::StaticClass());
		biomeChunk->cachedMesh->CreateMeshSection_LinearColor(0, regionVertice, regionVerticeTri, normals, uvs, vertexColors, tangents, false);
	}

	/*UE_LOG(LogTemp, Log, TEXT("Total filled: %d"), ultiTotal);

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

	//for (int i = 0; i < squares.Num(); i++)
		//for (int j = 0; j < squares[i].Num(); j++)
			//UE_LOG(LogTemp, Log, TEXT("#2 Current square: %d, %d, %d, %d"), squares[i][j].bottomLeft, squares[i][j].upperLeft, squares[i][j].bottomRight, squares[i][j].upperRight);*/
}

void AGameplayLevel::GeneratePushedElevationIsland() {
	int centralX = verticeDimensionX / 2;
	int centralY = verticeDimensionY / 2;
	//currentBiomes = 0;

	UE_LOG(LogTemp, Log, TEXT("Formula Test: %d, Seed: %d"), 51, generationSeed);

	int totalGenerated = 0;
	for (int i = 0; i < verticeDimensionX; i++) {
		int xFromCentral = FMath::Abs(centralX - i);
		bool squareCanForm = true;

		FVector2D left = FVector2D(i - 1, 0);

		//if (IsCoordinateWithinBounds(left))
			//squares.Add(TArray<MeshSquare*>());

		//int mSqrX = squares.Num() - 1;

		//UE_LOG(LogTemp, Log, TEXT("BL 2: %d, %d"), GetIndex(FVector2D(i, 0)), i);


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
			FVector2D min = FVector2D(i, j) - FVector2D(1, 1);

			if (!IsCoordinateWithinBounds(min, FVector2D(verticeDimensionX, verticeDimensionY)))
				continue;

			// Creates a square if the mesh square is within bounds
			int bL = GetIndex(min, verticeDimensionX);
			int uL = GetIndex(FVector2D(i - 1, j), verticeDimensionX);
			int bR = GetIndex(FVector2D(i, j - 1), verticeDimensionX);
			int uR = GetIndex(FVector2D(i, j), verticeDimensionX);

			float avgYPos = (vertices[bL].Z + vertices[uL].Z + vertices[bR].Z + vertices[uR].Z) / 4.f;
			//UE_LOG(LogTemp, Log, TEXT("AVGYPOS: %f"),avgYPos);

			//MeshSquare* inst = new MeshSquare(bL, uL, bR, uR, avgYPos);
			//squares[mSqrX].Add(inst);

			bottomLeft.Add(bL);
			upperLeft.Add(uL);
			bottomRight.Add(bR);
			upperRight.Add(uR);
			squarePeak.Add(avgYPos);

			FVector2D sqrCoords;

			GetCoordinatePosition(bottomLeft.Num() - 1, &sqrCoords, verticeDimensionX - 1);

			sqrCoords -= FVector2D(1, 1);

			//totalGenerated++;
			//int mSqrY = squares[mSqrX].Num() - 1;

			//FVector2D sqrCoord = FVector2D(mSqrX - 1, mSqrY - 1);

			//UE_LOG(LogTemp, Log, TEXT("PRE-POS X: %f, Y: %f"), sqrCoord.X, sqrCoord.Y);

			if (!IsCoordinateWithinBounds(sqrCoords, FVector2D(verticeDimensionX - 1, verticeDimensionY - 1)))
				continue;

			if (CheckIfVerticeIsPeak(sqrCoords)) {
				//UE_LOG(LogTemp, Log, TEXT("PRE-POS X: %f, Y: %f"), sqrCoord.X, sqrCoord.Y);
				peaks.Add(bottomLeft.Num() - 1);
				//actualPeakId.Add(actualPeakId.Num());
			}

			//if (IsCoordinateWithinBounds(uL) && IsCoordinateWithinBounds(bR) && IsCoordinateWithinBounds(uR))
				//squares.Add(MeshSquare(GetIndex(bL), GetIndex(uL), GetIndex(bR), GetIndex(uR)));


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

		//UE_LOG(LogTemp, Log, TEXT("generated sqr test %d"), totalGenerated);
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

bool AGameplayLevel::CheckIfVerticeIsPeak(FVector2D sqr) {

	//UE_LOG(LogTemp, Log, TEXT("PRE: Vertice: %d, Direction X: %f, Y: %f"), vertice, direction.X, direction.Y);


	for (int i = 0; i < directions.Num(); i++) {

		//UE_LOG(LogTemp, Log, TEXT("Original Coords: X: %f, Y: %f"), coordinates.X, coordinates.Y);
		//UE_LOG(LogTemp, Log, TEXT("Dir Coords: X: %f, Y: %f"), directions[i].X, directions[i].Y);
		FVector2D combinedCoordinates = sqr + directions[i];
		//UE_LOG(LogTemp, Log, TEXT("Combined Coords: X: %f, Y: %f"), combinedCoordinates.X, combinedCoordinates.Y);
		if (!IsCoordinateWithinBounds(combinedCoordinates, FVector2D(verticeDimensionX - 1, verticeDimensionY - 1)))
			continue;

		//UE_LOG(LogTemp, Log, TEXT("Ogpeak: %f DirPeak: %f"), squares[sqr.X][sqr.Y]->peak, squares[combinedCoordinates.X][combinedCoordinates.Y]->peak);
		if (squarePeak[GetIndex(combinedCoordinates, verticeDimensionX - 1)] >= squarePeak[GetIndex(sqr, verticeDimensionX - 1)])
			return false;
	}

	return true;
}

void AGameplayLevel::GetCoordinatePosition(int index, FVector2D* coordinates, int dimensionX) {
	coordinates->Y = index % dimensionX;
	coordinates->X = (index - coordinates->Y) / dimensionX;
}

int AGameplayLevel::GetIndex(FVector2D coordinates, int dimensionX) {
	return (coordinates.X * dimensionX) + coordinates.Y;
}

bool AGameplayLevel::IsCoordinateWithinBounds(FVector2D coordinate, FVector2D dimensions) {
	if (coordinate.X < 0 || coordinate.Y < 0 || coordinate.X >= dimensions.X || coordinate.Y >= dimensions.Y)
		return false;

	return true;
}

/*bool AGameplayLevel::IsSquareWithinBounds(FVector2D coordinate) {
	if (coordinate.X < 0 || coordinate.Y < 0 || coordinate.X >= verticeDimensionX - 1 || coordinate.Y >= verticeDimensionY - 1)
		return false;

	return true;
}*/


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
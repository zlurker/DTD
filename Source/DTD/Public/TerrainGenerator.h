// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "RoomProceduralGenerator.h"
#include "Math/UnrealMathUtility.h"
#include "Math/UnrealMathVectorConstants.h"
#include "BiomesData.h"
#include "TerrainGenerator.generated.h"

UCLASS()
class DTD_API ATerrainGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATerrainGenerator();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MyProceduralMesh")
		UProceduralMeshComponent* pm;

	UPROPERTY()
		TArray<FVector> vertices;
	UPROPERTY()
		TArray<FVector> normals;
	UPROPERTY()
		TArray<int32> triangles;
	UPROPERTY()
		TArray<FVector2D> uvs;
	UPROPERTY()
		TArray<FLinearColor> vertexColors;
	UPROPERTY()
		TArray<FProcMeshTangent> tangents;

	UPROPERTY(EditAnywhere, Category = "Generation Settings")
		int verticeDimensionX = 100;

	UPROPERTY(EditAnywhere, Category = "Generation Settings")
		int verticeDimensionY = 100;

	UPROPERTY(EditAnywhere, Category = "Generation Settings")
		float plotSpace = 100;

public:
	virtual void OnConstruction(const FTransform& Transform) override;
	void GeneratePerlinShapeIsland();
	void GeneratePushedElevationIsland();
	float ElevationClamp(FVector vectorFromCenter, float elevation);
	void GetCoordinatePosition(int index, FVector2D* coordinates);
	int GetIndex(FVector2D coordinates);
	void ClearMeshData();
	bool CheckIfVerticeIsPeak(int vertice);
	BiomesData BiomeDataCreator();

private:
	bool IsCoordinateWithinBounds(FVector2D coordinate);

private:
	TArray<int> peaks;
	TArray<FVector2D> directions;
	//FVector2D directions[];
	//int currentBiomes;
	//TArray<BiomesData*> biomesData;

	//TArray<int> verticeAssignedBiome;
};

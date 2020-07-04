// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Tower.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "TerrainChunk.h"
#include "MeshSquare.h"
#include "GameplayLevel.generated.h"

/**
 *
 */
UCLASS()
class DTD_API AGameplayLevel : public AGameModeBase
{
	GENERATED_BODY()

		/** General settings. */
public:

	AGameplayLevel();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void BuildTower(int towerType);
	//void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);
	void GenerateLevel();
	void ReduceCash();
	void TriggerBuildTower(FVector towerLoc);

public:
	int selectedTowerType;

	UPROPERTY(EditAnywhere, Category = "Economy")
		float playerCash;

	/* PG Settings/Dependencies */



public:
	UPROPERTY(EditAnywhere, Category = "Generation Settings")
		int generationSeed;

	UPROPERTY(EditAnywhere, Category = "Generation Settings")
		int verticeDimensionX = 100;

	UPROPERTY(EditAnywhere, Category = "Generation Settings")
		int verticeDimensionY = 100;

	UPROPERTY(EditAnywhere, Category = "Generation Settings")
		float plotDistance = 100;

private:
	void GeneratePushedElevationIsland();
	float ElevationClamp(FVector vectorFromCenter, float elevation);
	void GetCoordinatePosition(int index, FVector2D* coordinates);
	int GetIndex(FVector2D coordinates);
	bool CheckIfVerticeIsPeak(FVector2D sqr);
	bool IsCoordinateWithinBounds(FVector2D coordinate);
	bool IsSquareWithinBounds(FVector2D coordinate);

private:
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

	TArray<FVector2D> peaks;
	//TArray<int> actualPeakId;
	//TArray<TArray<FVector2D>> biomeRegions;

	TArray<FVector2D> directions;
	TArray<FVector2D> squareBase;
	TArray<FVector2D> squareDir;

	TArray<int> bottomLeft;
	TArray<int> upperLeft;
	TArray<int> bottomRight;
	TArray<int> upperRight;
	//TArray<TArray<MeshSquare*>> squares;
	

protected:
	/** Called when the game starts. */
	virtual void BeginPlay() override;

	/** The widget class we will use as our menu when the game starts. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UUserWidget> StartingWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG Game")
		TArray<TSubclassOf<class ATower>> towers;

	/** The widget instance that we are using as our menu. */
	UPROPERTY()
		UUserWidget* CurrentWidget;

};

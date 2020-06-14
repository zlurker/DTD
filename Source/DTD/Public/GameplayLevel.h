// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Tower.h"
#include "GameplayLevel.generated.h"


/**
 *
 */
UCLASS()
class DTD_API AGameplayLevel : public AGameModeBase
{
	GENERATED_BODY()

public:
	/** Remove the current menu widget and create a new one from the specified class, if provided. */
	AGameplayLevel();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void BuildTower(int towerType);
	//void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);
	void ReduceCash();
	void TriggerBuildTower();
	int selectedTowerType;


protected:
	/** Called when the game starts. */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** The widget class we will use as our menu when the game starts. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UUserWidget> StartingWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG Game")
		TArray<TSubclassOf<class ATower>> towers;

	/** The widget instance that we are using as our menu. */
	UPROPERTY()
		UUserWidget* CurrentWidget;

};

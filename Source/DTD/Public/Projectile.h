// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h "
#include "Tower.h"
#include "ProjectileEffectBase.h"
#include "CreepBase.h"
#include "Projectile.generated.h"

UCLASS()
class DTD_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetTarget(AActor* towerTarget);

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, Category = "Stats")
		float damage;

	UPROPERTY(EditAnywhere, Category = "Projectile Behaviour")
		bool seeking;

private:
	AActor* target;

	TArray<UProjectileEffectBase*> onUpdate;
	TArray<UProjectileEffectBase*> onHit;
};

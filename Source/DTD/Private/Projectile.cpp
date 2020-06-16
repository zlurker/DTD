// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "ProjectileEffectBase.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	TArray<UProjectileEffectBase*> projectileEffects;
	GetComponents<UProjectileEffectBase>(projectileEffects);

	for (UProjectileEffectBase* effect : projectileEffects) {

		switch (effect->projectileEffectType) {
		case EffectType::Update:
			onUpdate.Add(effect);
			//effect->ApplyEffect(target);
			break;

		case EffectType::OnHit:
			onHit.Add(effect);
			break;
		}
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (target != nullptr) {

		FVector dir = direction;

		if (seeking) {
			dir = target->GetActorLocation() - GetActorLocation();
			dir.Normalize(0);
		}

		SetActorLocation(GetActorLocation() + dir);
	}

	for (UProjectileEffectBase* effect : onUpdate)
		effect->ApplyEffect(target);
}

void AProjectile::SetTarget(AActor* towerTarget) {
	target = towerTarget;

	if (!seeking && target != nullptr) {
		direction = target->GetActorLocation() - GetActorLocation();
		direction.Normalize();
	}
}


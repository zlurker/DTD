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

	TArray<UShapeComponent*> projectileColliders;
	GetComponents<UShapeComponent>(projectileColliders);

	for (UShapeComponent* collider : projectileColliders) {
		UE_LOG(LogTemp, Log, TEXT("Collision set up."));

		if (collider != nullptr) {
			UE_LOG(LogTemp, Log, TEXT("Collision exist."));
			collider->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
		}
	}
}

void AProjectile::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ACreepBase* creep = Cast<ACreepBase>(OtherActor);

	UE_LOG(LogTemp, Log, TEXT("Something came in contact with projectile."));

	if (creep != nullptr) {
		//aqquiredTargets.Add(OtherActor);
		for (UProjectileEffectBase* effect : onHit)
			effect->ApplyEffect(target);
		UE_LOG(LogTemp, Log, TEXT("Performing hit actions."));
	}

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (UProjectileEffectBase* effect : onUpdate)
		effect->ApplyEffect(target);
}

void AProjectile::SetTarget(AActor* towerTarget) {
	target = towerTarget;
}


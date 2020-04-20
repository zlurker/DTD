// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UStaticMeshComponent* projectileArt = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Art"));
	projectileArt->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> projectileAsset(TEXT
	("/Game/Meshes/Gun_Turret01.Gun_Turret01"));

	if (projectileAsset.Succeeded()) {
		projectileArt->SetStaticMesh(projectileAsset.Object);
	}
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::SetMesh(UStaticMesh* mesh) {
	projectileMeshComponent->SetStaticMesh(mesh);
}


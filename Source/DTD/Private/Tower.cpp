// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

// Sets default values
ATower::ATower()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Sets up attack radius for later use.
	attackRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Tower Attack Radius"));
	attackRadius->SetupAttachment(RootComponent);
	attackRadius->bHiddenInGame = false;

}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();
	attackRadius->SetSphereRadius(towerAttackRadius);
	attackRadius->OnComponentBeginOverlap.AddDynamic(this, &ATower::OnOverlapBegin);
	attackRadius->OnComponentEndOverlap.AddDynamic(this, &ATower::OnOverlapEnd);


	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	TArray<AActor*> ignoreActors;

	UClass* seekClass = ACreepBase::StaticClass();
	

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), towerAttackRadius, traceObjectTypes, seekClass, ignoreActors, aqquiredTargets);

	UE_LOG(LogTemp, Log, TEXT("Tower initialised... Current creep aqquired: %d"), aqquiredTargets.Num());
	//for (AActor* overlappedActor : aqquiredTargets)
		//UE_LOG(LogTemp, Log, TEXT("OverlappedActor: %s"), *overlappedActor->GetName());
}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float currTime = UGameplayStatics::GetRealTimeSeconds((UObject*)GetWorld());

	if (prevAction + attackPeriod <= currTime) {
		prevAction = currTime;
		TowerAction();
	}
}

void ATower::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ACreepBase* creep = Cast<ACreepBase>(OtherActor);

	UE_LOG(LogTemp, Log, TEXT("Something came in contact."));

	if (creep != nullptr) {
		aqquiredTargets.Add(OtherActor);
		UE_LOG(LogTemp, Log, TEXT("Creep is in range. Adding to array... Current creep aqquired: %d"), aqquiredTargets.Num());	
	}

}

void ATower::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ACreepBase* creep = Cast<ACreepBase>(OtherActor);

	UE_LOG(LogTemp, Log, TEXT("Something has left range of tower."));

	if (creep != nullptr) {
		aqquiredTargets.Remove(OtherActor);
		UE_LOG(LogTemp, Log, TEXT("Creep is out of range. Removing from array... Current creep aqquired: %d"), aqquiredTargets.Num());
	}
}

void ATower::TowerAction() {
	//UE_LOG(LogTemp, Warning, TEXT("Tower actions."));
}


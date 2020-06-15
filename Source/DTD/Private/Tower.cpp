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

		if (enableTargetting)
			if (currentTarget == nullptr) {
				if (!SelectTarget())
					return;
			}

		prevAction = currTime;
		TowerAction();
	}
}

bool ATower::SelectTarget() {

	if (aqquiredTargets.Num() > 0) {
		int nearestTarget = 0;
		float lowestDotProduct = FVector::DotProduct(aqquiredTargets[0]->GetActorLocation(), GetActorLocation());

		if (aqquiredTargets.Num() > 1) 
			for (int i = 1; i < aqquiredTargets.Num(); i++) {
				float currDp = FVector::DotProduct(aqquiredTargets[i]->GetActorLocation(), GetActorLocation());

				if (currDp < lowestDotProduct)
					lowestDotProduct = currDp;
			}
		
		currentTarget = aqquiredTargets[nearestTarget];
		UE_LOG(LogTemp, Log, TEXT("Target Aqquired: %s"), *currentTarget->GetName());
		return true;
	}

	UE_LOG(LogTemp, Log, TEXT("No target aqquired."));
	return false;
}

void ATower::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ACreepBase* creep = Cast<ACreepBase>(OtherActor);

	//UE_LOG(LogTemp, Log, TEXT("Something came in contact."));

	if (creep != nullptr) {
		aqquiredTargets.Add(OtherActor);
		UE_LOG(LogTemp, Log, TEXT("Creep is in range. Adding to array... Current creep aqquired: %d"), aqquiredTargets.Num());
	}

}

void ATower::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ACreepBase* creep = Cast<ACreepBase>(OtherActor);

	//UE_LOG(LogTemp, Log, TEXT("Something has left range of tower."));

	if (creep != nullptr) {
		if (creep == currentTarget)
			currentTarget = nullptr;

		aqquiredTargets.Remove(OtherActor);
		UE_LOG(LogTemp, Log, TEXT("Creep is out of range. Removing from array... Current creep aqquired: %d"), aqquiredTargets.Num());
	}
}

void ATower::TowerAction() {
	//UE_LOG(LogTemp, Warning, TEXT("Tower actions."));
}


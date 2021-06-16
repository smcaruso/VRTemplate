// Fill out your copyright notice in the Description page of Project Settings.


#include "MaterialSelector.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h" 
#include "Sound/SoundCue.h" 
#include "VRChar.h"

// Sets default values
AMaterialSelector::AMaterialSelector()
{

	PrimaryActorTick.bCanEverTick = true;

	BoxCollisionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision Volume"));
	BoxCollisionVolume->SetupAttachment(GetRootComponent());
	BoxCollisionVolume->SetGenerateOverlapEvents(true);

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object Mesh"));
	ObjectMesh->SetupAttachment(GetRootComponent());

	PlacementRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Swatch Placement Root"));
	PlacementRoot->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AMaterialSelector::BeginPlay()
{
	Super::BeginPlay();

	if (CounterMaterials[0] != nullptr) {
		ObjectMesh->SetMaterial(0, CounterMaterials[0]);
	}

	if (CabinetMaterials[0] != nullptr) {
		ObjectMesh->SetMaterial(1, CabinetMaterials[0]);
	}

	BoxCollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AMaterialSelector::BeginOverlap);
	BoxCollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AMaterialSelector::EndOverlap);
	
}

// Called every frame
void AMaterialSelector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMaterialSelector::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	if (! OtherActor->ActorHasTag(TEXT("PlayerCapsule"))) return;

	FVector SwatchSpawnPoint = PlacementRoot->GetComponentLocation();
	int32 Index = 0;

	if (OpenSound != nullptr) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Cast<USoundBase>(OpenSound), this->GetActorLocation());
	}

	UE_LOG(LogTemp, Warning, TEXT("Registering overlap with %s"), *OtherActor->GetDebugName(OtherActor));

	for (UMaterialInterface* CounterMaterial : CounterMaterials) {
		SwatchSpawnPoint += SwatchSpawnOffset;
		ASelectorSwatch* NewSwatch = GetWorld()->SpawnActor<ASelectorSwatch>(SwatchClass, SwatchSpawnPoint, PlacementRoot->GetComponentRotation());

		SelectorSwatches.Emplace(NewSwatch);
		
		if (Index == 0) {
			SelectorSwatches[Index]->SetupNewSwatch(CounterMaterial, this, true, 0);
		}

		if (Index > 0 && Index <= CounterMaterials.Num()) {
			SelectorSwatches[Index]->SetupNewSwatch(CounterMaterial, this, false, 0);
		}

		Index++;
	}

	SwatchSpawnPoint = PlacementRoot->GetComponentLocation();
	SwatchSpawnPoint += FVector(-5, -15, 0);


	for (UMaterialInterface* CabinetMaterial : CabinetMaterials) {
		SwatchSpawnPoint += SwatchSpawnOffset;
		ASelectorSwatch* NewSwatch = GetWorld()->SpawnActor<ASelectorSwatch>(SwatchClass, SwatchSpawnPoint, PlacementRoot->GetComponentRotation());

		SelectorSwatches.Emplace(NewSwatch);

		if (Index == CounterMaterials.Num() - 1) {
			SelectorSwatches[Index]->SetupNewSwatch(CabinetMaterial, this, true, 1);
		}

		if (Index > 0 && Index <= CabinetMaterials.Num() + CounterMaterials.Num()) {
			SelectorSwatches[Index]->SetupNewSwatch(CabinetMaterial, this, false, 1);
		}

		Index++;
	}

}

void AMaterialSelector::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	if (!OtherActor->ActorHasTag(TEXT("PlayerCapsule"))) return;

	for (ASelectorSwatch* Swatch : SelectorSwatches) {
		Swatch->Destroy();
	}

	if (CloseSound != nullptr) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Cast<USoundBase>(CloseSound), this->GetActorLocation());
	}

	SelectorSwatches.Empty();

}

void AMaterialSelector::SetMaterial(int32 Element, class UMaterialInterface* MaterialToSet, ASelectorSwatch* CallingSwatch) {

	for (ASelectorSwatch* Swatch : SelectorSwatches) {
		if (Swatch->ElementIndex == CallingSwatch->ElementIndex) {
			Swatch->TurnOffLight();
		}
	}

	ObjectMesh->SetMaterial(Element, MaterialToSet);

}



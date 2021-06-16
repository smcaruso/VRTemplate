// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectorSwatch.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h" 
#include "MaterialSelector.h"

// Sets default values
ASelectorSwatch::ASelectorSwatch() {

	PrimaryActorTick.bCanEverTick = true;

	SwatchTile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Swatch Tile Mesh"));
	SetRootComponent(SwatchTile);

	Tags.Add(TEXT("Swatch"));

}

// Called when the game starts or when spawned
void ASelectorSwatch::BeginPlay()
{
	Super::BeginPlay();

	if (SwatchActiveMaterialBase != nullptr) {

		SwatchActiveMaterial = UMaterialInstanceDynamic::Create(SwatchActiveMaterialBase, this);
		SwatchTile->SetMaterial(1, SwatchActiveMaterial);

	}
	
}

// Called every frame
void ASelectorSwatch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASelectorSwatch::SetupNewSwatch(class UMaterialInterface* NewMaterial, class AMaterialSelector* ParentSelector, bool bIsActive, int32 SetupElementIndex) {
	
	ElementIndex = SetupElementIndex;
	SwatchMaterial = NewMaterial;
	SwatchTile->SetMaterial(0, SwatchMaterial);

	MaterialSelector = ParentSelector;

	if (bIsActive && SwatchActiveMaterial != nullptr) {
		SwatchActiveMaterial->SetScalarParameterValue(TEXT("Bright"), ActiveIndicatorBrightness);
	}

}

void ASelectorSwatch::PickedUp() {

	SwatchTile->SetSimulatePhysics(false);
	MaterialSelector->SetMaterial(ElementIndex, SwatchTile->GetMaterial(0), this);
	SwatchActiveMaterial->SetScalarParameterValue(TEXT("Bright"), ActiveIndicatorBrightness);

}

void ASelectorSwatch::PutDown() {

	SwatchTile->SetSimulatePhysics(true);


}

void ASelectorSwatch::TurnOffLight() {

	SwatchActiveMaterial->SetScalarParameterValue(TEXT("Bright"), 0);

}

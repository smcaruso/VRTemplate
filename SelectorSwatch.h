// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SelectorSwatch.generated.h"

UCLASS()
class VRTEMPLATE_API ASelectorSwatch : public AActor
{
	GENERATED_BODY()
	
public:	

	ASelectorSwatch();
	virtual void Tick(float DeltaTime) override;

	void SetupNewSwatch(class UMaterialInterface* NewMaterial, class AMaterialSelector* ParentSelector, bool bIsActive, int32 SetupElementIndex);

	void PickedUp();
	void PutDown();

	void TurnOffLight();

	UPROPERTY()
	int32 ElementIndex = 0;

protected:

	virtual void BeginPlay() override;

private:	

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* SwatchTile;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* SwatchMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* SwatchActiveMaterialBase;

	UPROPERTY()
	class UMaterialInstanceDynamic* SwatchActiveMaterial;

	UPROPERTY()
	AMaterialSelector* MaterialSelector;

	UPROPERTY(EditAnywhere)
	float ActiveIndicatorBrightness = 10.f;


};

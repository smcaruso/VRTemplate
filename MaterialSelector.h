// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SelectorSwatch.h"
#include "MaterialSelector.generated.h"

UCLASS()
class VRTEMPLATE_API AMaterialSelector : public AActor
{
	GENERATED_BODY()
	
public:	

	AMaterialSelector();
	virtual void Tick(float DeltaTime) override;

	void SetMaterial(int32 Element, class UMaterialInterface* MaterialToSet, ASelectorSwatch* CallingSwatch);

protected:

	virtual void BeginPlay() override;

private:	

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollisionVolume;

	UPROPERTY(EditAnywhere)
	class USceneComponent* PlacementRoot;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* ObjectMesh;

	UPROPERTY(EditAnywhere)
	TArray<UMaterialInterface*> CounterMaterials;

	UPROPERTY(EditAnywhere)
	TArray<UMaterialInterface*> CabinetMaterials;

	UPROPERTY(EditAnywhere)
	TArray<ASelectorSwatch*> SelectorSwatches;

	UPROPERTY(EditDefaultsOnly)
	FVector SwatchSpawnOffset = FVector(-10, 0, 0);

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASelectorSwatch> SwatchClass;

	UPROPERTY(EditDefaultsOnly)
	class USoundCue* OpenSound;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* CloseSound;
	
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseObjectViewer.generated.h"

UCLASS()
class VRTEMPLATE_API ABaseObjectViewer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseObjectViewer();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ObjectMesh;

	UPROPERTY(EditAnywhere)
	class UCurveFloat* InfoPanelOpenCurve;

	UPROPERTY(EditDefaultsOnly)
	float InfoOpenSpeed = 50.f;

	float InfoOpenPercent = 0.f;

	bool bInfoOpen = false;

private:	
	
	// Components

	UPROPERTY(EditAnywhere)
	USceneComponent* RootSceneComponent;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollisionVolume;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* InfoPanelMesh;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* InfoPanelWidget;

	// Configuration

	UPROPERTY(EditDefaultsOnly)
	float InfoPanelXWidth = 1.f;

	UPROPERTY(EditDefaultsOnly)
	float InfoPanelZHeight = 1.f;

	UPROPERTY(EditDefaultsOnly)
	class USoundCue* OpenSound;
	
	UPROPERTY(EditDefaultsOnly)
	USoundCue* CloseSound;


	AActor* Overlapper;

	// Overlap event

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	// Functions

	UFUNCTION()
	void OpenInfo(float DeltaTime);

	UFUNCTION()
	void CloseInfo(float DeltaTime);
};

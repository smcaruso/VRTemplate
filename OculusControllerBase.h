// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MotionControllerComponent.h"
#include "OculusControllerBase.generated.h"

UCLASS()
class VRTEMPLATE_API AOculusControllerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this actor's properties
	AOculusControllerBase();

	// Functions

	void SetLeftHand() { MotionController->SetTrackingSource(EControllerHand::Left); }
	void SetRightHand() { MotionController->SetTrackingSource(EControllerHand::Right); }

	void SetMeshVisible(bool bMeshVisibility) { ControllerMesh->SetVisibility(bMeshVisibility, false); }

	void SetMessageText(FString NewMessage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	

	// Components

	UPROPERTY(VisibleAnywhere)
	UMotionControllerComponent* MotionController;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* ControllerMesh;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* InfoWidget;

	void FadeOutMessage();

	// Configuration

};

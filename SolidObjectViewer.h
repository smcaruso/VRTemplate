// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObjectViewer.h"
#include "SolidObjectViewer.generated.h"

/**
 * 
 */
UCLASS()
class VRTEMPLATE_API ASolidObjectViewer : public ABaseObjectViewer
{
	GENERATED_BODY()

public:

	ASolidObjectViewer();
	virtual void Tick(float DeltaTime) override;

	void RotateLeft();
	void RotateRight();

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ControlPanelMesh;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* ControlPanelWidget;

	float ControlPanelYHeight = 1.f;
	float ControlPanelZDepth = 1.f;

	// Functions

	UFUNCTION()
	void OpenControls(float DeltaTime);

	UFUNCTION()
	void CloseControls(float DeltaTime);

	UPROPERTY()
	FTransform ObjectInitialTransform;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObjectViewer.h"
#include "SlideshowObjectViewer.generated.h"

/**
 * 
 */
UCLASS()
class VRTEMPLATE_API ASlideshowObjectViewer : public ABaseObjectViewer
{
	GENERATED_BODY()
public:

	ASlideshowObjectViewer();
	virtual void Tick(float DeltaTime) override;

	void NextImage();
	void PrevImage();

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ControlPanelMesh;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* ControlPanelWidget;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* SlideshowBaseMaterial;

	UPROPERTY()
	class UMaterialInstanceDynamic* SlideshowMaterial;

	UPROPERTY(EditAnywhere)
	TArray<class UTexture*> SlideshowImages;

	float ControlPanelYHeight = 1.f;
	float ControlPanelZDepth = 1.f;

	int32 ImageIndex = 0;

	// Functions

	UFUNCTION()
	void OpenControls(float DeltaTime);

	UFUNCTION()
	void CloseControls(float DeltaTime);
};

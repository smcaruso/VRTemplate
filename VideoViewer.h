// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObjectViewer.h"
#include "VideoViewer.generated.h"

/**
 * 
 */
UCLASS()
class VRTEMPLATE_API AVideoViewer : public ABaseObjectViewer
{
	GENERATED_BODY()

public:

	AVideoViewer();
	virtual void Tick(float DeltaTime) override;

	void PlayPause();
	void Reset();

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ControlPanelMesh;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* ControlPanelWidget;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* PlayerBaseMaterial;

	UPROPERTY(EditAnywhere)
	class UMediaPlayer* VideoPlayer;

	UPROPERTY(EditAnywhere)
	class UMediaSource* VideoSource;


	float ControlPanelYHeight = 1.f;
	float ControlPanelZDepth = 1.f;


	// Functions

	UFUNCTION()
	void OpenControls(float DeltaTime);

	UFUNCTION()
	void CloseControls(float DeltaTime);
	
};

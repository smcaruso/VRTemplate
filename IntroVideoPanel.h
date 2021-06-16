// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IntroVideoPanel.generated.h"

UCLASS()
class VRTEMPLATE_API AIntroVideoPanel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIntroVideoPanel();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ClosePanel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* InfoPanel;

	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* InfoPanelWidget;

	UPROPERTY(EditDefaultsOnly)
	class UCurveFloat* InfoPanelOpenCurve;

	UPROPERTY(EditDefaultsOnly)
	float InfoOpenSpeed = 50.f;

	UFUNCTION()
	void CloseInfo(float DeltaTime);

	float InfoOpenPercent = 0.f;

	bool bIsClosed = false;

};

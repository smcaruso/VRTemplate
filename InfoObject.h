// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InfoObject.generated.h"

UCLASS()
class VRTEMPLATE_API AInfoObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInfoObject();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PickedUp();
	void PutDown();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ObjectMesh;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* InfoPanel;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* InfoPanelWidget;

	UPROPERTY(EditAnywhere)
	class UCurveFloat* InfoPanelOpenCurve;

	UPROPERTY(EditDefaultsOnly)
	class USoundCue* OpenSound;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* CloseSound;

	UPROPERTY(EditDefaultsOnly)
	float InfoOpenSpeed = 50.f;

	UPROPERTY(EditDefaultsOnly)
	float InfoPanelXWidth = 0.085f;

	UPROPERTY(EditDefaultsOnly)
	float InfoPanelZHeight = 0.125;

	UFUNCTION()
	void OpenInfo(float DeltaTime);

	UFUNCTION()
	void CloseInfo(float DeltaTime);

	float InfoOpenPercent = 0.f;

	bool bIsPickedUp = false;

};

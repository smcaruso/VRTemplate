// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OculusControllerBase.h"
#include "PointerController.generated.h"

/**
 * 
 */
UCLASS()
class VRTEMPLATE_API APointerController : public AOculusControllerBase
{
	GENERATED_BODY()

public:

	APointerController();

	virtual void Tick(float DeltaTime) override;
	
	// Interaction Functions

	void PressGrip();
	void ReleaseGrip();

	void PullTrigger();
	void ReleaseTrigger();
	void Scroll(float ScrollAmt);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:

	AActor* GrabbedActor;

	UPROPERTY(VisibleAnywhere)
	class UWidgetInteractionComponent* WidgetInteraction;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* PointerCursor;
};

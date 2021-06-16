// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OculusControllerBase.h"
#include "TeleporterController.generated.h"

/**
 * 
 */
UCLASS()
class VRTEMPLATE_API ATeleporterController : public AOculusControllerBase
{
	GENERATED_BODY()

public:

	ATeleporterController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};

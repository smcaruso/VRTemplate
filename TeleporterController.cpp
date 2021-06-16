// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleporterController.h"

ATeleporterController::ATeleporterController() {

}

void ATeleporterController::BeginPlay()
{
	Super::BeginPlay();
	Super::SetLeftHand();
}
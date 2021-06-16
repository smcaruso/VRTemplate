// Fill out your copyright notice in the Description page of Project Settings.


#include "OculusControllerBase.h"
#include "Components/WidgetComponent.h"
#include "ControllerInfoWidget.h"
#include "TimerManager.h"

// Sets default values
AOculusControllerBase::AOculusControllerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Motion Controller"));
	SetRootComponent(MotionController);

	ControllerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Controller Mesh"));
	ControllerMesh->SetupAttachment(MotionController);

	InfoWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Controller Info Widget"));
	InfoWidget->SetupAttachment(ControllerMesh);
}

// Called when the game starts or when spawned
void AOculusControllerBase::BeginPlay()
{
	Super::BeginPlay();
	InfoWidget->SetVisibility(false, false);
	
}

// Called every frame
void AOculusControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOculusControllerBase::SetMessageText(FString NewMessage) {

	Cast<UControllerInfoWidget>(InfoWidget->GetUserWidgetObject())->SetMessageText(NewMessage);
	InfoWidget ->SetVisibility(true, false);
	GetWorldTimerManager().ClearAllTimersForObject(this);

	FTimerHandle WaitASecond;

	GetWorldTimerManager().SetTimer(
		WaitASecond, // timer handle
		this, // user class
		&AOculusControllerBase::FadeOutMessage, // delegate
		1 // rate
		// loop? false default
		// first delay
	);

}

void AOculusControllerBase::FadeOutMessage() {

	InfoWidget->SetVisibility(false, false);

}


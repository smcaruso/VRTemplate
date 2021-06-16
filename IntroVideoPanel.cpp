// Fill out your copyright notice in the Description page of Project Settings.


#include "IntroVideoPanel.h"
#include "Components/WidgetComponent.h"
#include "IntroVideoWidget.h"
#include "Curves/CurveFloat.h"

// Sets default values
AIntroVideoPanel::AIntroVideoPanel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InfoPanel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Info Panel Mesh"));
	SetRootComponent(InfoPanel);

	InfoPanelWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Info Panel Widget"));
	InfoPanelWidget->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AIntroVideoPanel::BeginPlay()
{
	Super::BeginPlay();

	UIntroVideoWidget* InfoPanelWidgetPointer = Cast< UIntroVideoWidget>(InfoPanelWidget->GetUserWidgetObject());

	if (InfoPanelWidget) {
		InfoPanelWidgetPointer->SetParentPanel(this);
	}
	
}

// Called every frame
void AIntroVideoPanel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InfoOpenPercent < 100 && bIsClosed) {
		CloseInfo(DeltaTime);
	}

}

void AIntroVideoPanel::ClosePanel() {

	InfoPanelWidget->SetVisibility(false, true);
	bIsClosed = true;

	UE_LOG(LogTemp, Warning, TEXT("Closing Panel Now"));

}


void AIntroVideoPanel::CloseInfo(float DeltaTime) {

	if (InfoPanelOpenCurve != nullptr) {

		InfoPanel->SetWorldScale3D(FVector(1, InfoPanelOpenCurve->GetFloatValue(InfoOpenPercent), 1));
		InfoOpenPercent += InfoOpenSpeed * DeltaTime;

	}

}


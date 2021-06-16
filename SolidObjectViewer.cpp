// Fill out your copyright notice in the Description page of Project Settings.


#include "SolidObjectViewer.h"
#include "Components/WidgetComponent.h"
#include "SolidObjectControls.h"
#include "Curves/CurveFloat.h"

ASolidObjectViewer::ASolidObjectViewer() {

	PrimaryActorTick.bCanEverTick = true;

	ControlPanelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Control Panel Mesh"));
	ControlPanelMesh->SetupAttachment(GetRootComponent());
	ControlPanelMesh->SetGenerateOverlapEvents(false);

	ControlPanelWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Control Panel Widget"));
	ControlPanelWidget->SetupAttachment(ControlPanelMesh);
	ControlPanelWidget->SetGenerateOverlapEvents(false);

}

void ASolidObjectViewer::BeginPlay() {
	Super::BeginPlay();

	ControlPanelMesh->SetWorldScale3D(FVector(0.f, ControlPanelYHeight, ControlPanelZDepth));
	ControlPanelMesh->SetVisibility(false, true);

	if (ControlPanelWidget) {

		Cast<USolidObjectControls>(ControlPanelWidget->GetUserWidgetObject())->SetParentViewer(this);

	}

}

void ASolidObjectViewer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (InfoOpenPercent < 100 && bInfoOpen) {
		OpenControls(DeltaTime);
	}

	if (InfoOpenPercent > 0 && !bInfoOpen) {
		CloseControls(DeltaTime);
	}

	if (InfoOpenPercent > 85 && bInfoOpen) {
		ControlPanelWidget->SetVisibility(true);
	}

}

void ASolidObjectViewer::OpenControls(float DeltaTime) {

	if (InfoPanelOpenCurve != nullptr) {

		ControlPanelMesh->SetVisibility(true);
		ControlPanelMesh->SetWorldScale3D(FVector(InfoPanelOpenCurve->GetFloatValue(InfoOpenPercent) * .25, ControlPanelYHeight, ControlPanelZDepth));
		InfoOpenPercent += InfoOpenSpeed * DeltaTime;

	}

}

void ASolidObjectViewer::CloseControls(float DeltaTime) {

	if (InfoPanelOpenCurve != nullptr) {

		ControlPanelWidget->SetVisibility(false);
		ControlPanelMesh->SetWorldScale3D(FVector(InfoPanelOpenCurve->GetFloatValue(InfoOpenPercent) * .25, ControlPanelYHeight, ControlPanelZDepth));
		InfoOpenPercent -= (InfoOpenSpeed * 2) * DeltaTime;

	}

}

void ASolidObjectViewer::RotateLeft() {

	ObjectMesh->AddLocalRotation(FRotator(0, 90, 0));

}

void ASolidObjectViewer::RotateRight() {

	ObjectMesh->AddLocalRotation(FRotator(0, -90, 0));

}
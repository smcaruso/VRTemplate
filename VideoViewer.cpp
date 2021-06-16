// Fill out your copyright notice in the Description page of Project Settings.


#include "VideoViewer.h"
#include "Components/WidgetComponent.h"
#include "Curves/CurveFloat.h"
#include "MediaPlayer.h" 
#include "MediaSource.h" 
#include "VideoControls.h"

AVideoViewer::AVideoViewer() {

	PrimaryActorTick.bCanEverTick = true;

	ControlPanelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Control Panel Mesh"));
	ControlPanelMesh->SetupAttachment(GetRootComponent());
	ControlPanelMesh->SetGenerateOverlapEvents(false);

	ControlPanelWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Control Panel Widget"));
	ControlPanelWidget->SetupAttachment(ControlPanelMesh);
	ControlPanelWidget->SetGenerateOverlapEvents(false);

}

void AVideoViewer::BeginPlay() {
	Super::BeginPlay();

	if (PlayerBaseMaterial != nullptr) {
		ObjectMesh->SetMaterial(0, PlayerBaseMaterial);
	}

	ControlPanelMesh->SetWorldScale3D(FVector(0.f, ControlPanelYHeight, ControlPanelZDepth));
	ControlPanelMesh->SetVisibility(false, true);

	if (ControlPanelWidget) {

		Cast<UVideoControls>(ControlPanelWidget->GetUserWidgetObject())->SetParentVideoViewer(this);

	}

	if (VideoPlayer && VideoSource) {

		VideoPlayer->OpenSource(VideoSource);
		VideoPlayer->Rewind();
		VideoPlayer->Pause();

	}
}

void AVideoViewer::Tick(float DeltaTime) {
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

void AVideoViewer::OpenControls(float DeltaTime) {

	if (InfoPanelOpenCurve != nullptr) {

		ControlPanelMesh->SetVisibility(true);
		ControlPanelMesh->SetWorldScale3D(FVector(InfoPanelOpenCurve->GetFloatValue(InfoOpenPercent) * .25, ControlPanelYHeight, ControlPanelZDepth));
		InfoOpenPercent += InfoOpenSpeed * DeltaTime;

	}

}

void AVideoViewer::CloseControls(float DeltaTime) {

	if (InfoPanelOpenCurve != nullptr) {

		ControlPanelWidget->SetVisibility(false);
		ControlPanelMesh->SetWorldScale3D(FVector(InfoPanelOpenCurve->GetFloatValue(InfoOpenPercent) * .25, ControlPanelYHeight, ControlPanelZDepth));
		InfoOpenPercent -= (InfoOpenSpeed * 2) * DeltaTime;

	}

}

void AVideoViewer::PlayPause() {

	if (VideoPlayer->IsPlaying()) {
		VideoPlayer->Pause();
	}
	else {
		VideoPlayer->Play();
	}

}

void AVideoViewer::Reset() {

	VideoPlayer->Rewind();

}
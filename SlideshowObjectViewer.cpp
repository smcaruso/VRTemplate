// Fill out your copyright notice in the Description page of Project Settings.


#include "SlideshowObjectViewer.h"
#include "Components/WidgetComponent.h"
#include "Curves/CurveFloat.h"
#include "Engine/Texture.h" 
#include "SlideshowControls.h"
#include "Materials/MaterialInstanceDynamic.h"

ASlideshowObjectViewer::ASlideshowObjectViewer() {

	PrimaryActorTick.bCanEverTick = true;

	ControlPanelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Control Panel Mesh"));
	ControlPanelMesh->SetupAttachment(GetRootComponent());
	ControlPanelMesh->SetGenerateOverlapEvents(false);

	ControlPanelWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Control Panel Widget"));
	ControlPanelWidget->SetupAttachment(ControlPanelMesh);
	ControlPanelWidget->SetGenerateOverlapEvents(false);

}

void ASlideshowObjectViewer::BeginPlay() {
	Super::BeginPlay();

	if (SlideshowBaseMaterial != nullptr) {
		SlideshowMaterial = UMaterialInstanceDynamic::Create(SlideshowBaseMaterial, this);
		ObjectMesh->SetMaterial(0, SlideshowMaterial);
		if (SlideshowImages[ImageIndex] != nullptr) {
			SlideshowMaterial->SetTextureParameterValue(TEXT("Image"), SlideshowImages[ImageIndex]);
		}
	}

	ControlPanelMesh->SetWorldScale3D(FVector(0.f, ControlPanelYHeight, ControlPanelZDepth));
	ControlPanelMesh->SetVisibility(false, true);

	if (ControlPanelWidget) {

		Cast<USlideshowControls>(ControlPanelWidget->GetUserWidgetObject())->SetParentSlideshow(this);

	}
}

void ASlideshowObjectViewer::Tick(float DeltaTime) {
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

void ASlideshowObjectViewer::OpenControls(float DeltaTime) {

	if (InfoPanelOpenCurve != nullptr) {

		ControlPanelMesh->SetVisibility(true);
		ControlPanelMesh->SetWorldScale3D(FVector(InfoPanelOpenCurve->GetFloatValue(InfoOpenPercent) * .25, ControlPanelYHeight, ControlPanelZDepth));
		InfoOpenPercent += InfoOpenSpeed * DeltaTime;

	}

}

void ASlideshowObjectViewer::CloseControls(float DeltaTime) {

	if (InfoPanelOpenCurve != nullptr) {
		
		ControlPanelWidget->SetVisibility(false);
		ControlPanelMesh->SetWorldScale3D(FVector(InfoPanelOpenCurve->GetFloatValue(InfoOpenPercent) * .25, ControlPanelYHeight, ControlPanelZDepth));
		InfoOpenPercent -= (InfoOpenSpeed * 2) * DeltaTime;

	}

}

void ASlideshowObjectViewer::NextImage() {

	if (ImageIndex < SlideshowImages.Num() -1) {
		ImageIndex++;
	}
	else {
		ImageIndex = 0;
	}

	SlideshowMaterial->SetTextureParameterValue(TEXT("Image"), SlideshowImages[ImageIndex]);

}

void ASlideshowObjectViewer::PrevImage() {

	if (ImageIndex <= SlideshowImages.Num() && ImageIndex >= 1) {
		ImageIndex--;
	}
	else {
		ImageIndex = SlideshowImages.Num() -1;
	}

	SlideshowMaterial->SetTextureParameterValue(TEXT("Image"), SlideshowImages[ImageIndex]);


}
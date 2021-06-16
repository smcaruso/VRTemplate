// Fill out your copyright notice in the Description page of Project Settings.


#include "SolidObjectControls.h"

bool USolidObjectControls::Initialize() {

	if (!Super::Initialize()) return false;

	if (!LeftButton || !RightButton) return false;

	LeftButton->OnClicked.AddDynamic(this, &USolidObjectControls::LeftButtonClicked);
	RightButton->OnClicked.AddDynamic(this, &USolidObjectControls::RightButtonClicked);

	return true;
}

void USolidObjectControls::LeftButtonClicked() {

	if (ParentViewer != nullptr) {
		ParentViewer->RotateLeft();
	}

}

void USolidObjectControls::RightButtonClicked() {
	if (ParentViewer != nullptr) {
		ParentViewer->RotateRight();
	}
}
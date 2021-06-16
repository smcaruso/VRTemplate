// Fill out your copyright notice in the Description page of Project Settings.


#include "IntroVideoWidget.h"

bool UIntroVideoWidget::Initialize() {

	if (!Super::Initialize()) return false;

	if (!ClosePanelButton) return false;

	ClosePanelButton->OnClicked.AddDynamic(this, &UIntroVideoWidget::ClosePanelClicked);
	return true;
}

void UIntroVideoWidget::ClosePanelClicked() {

	if (ParentPanel) {

		ParentPanel->ClosePanel();

	}

}
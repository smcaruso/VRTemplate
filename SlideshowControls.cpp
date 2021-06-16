// Fill out your copyright notice in the Description page of Project Settings.


#include "SlideshowControls.h"

bool USlideshowControls::Initialize() {

	if (!Super::Initialize()) return false;

	if (!NextButton || !PrevButton) return false;

	NextButton->OnClicked.AddDynamic(this, &USlideshowControls::NextButtonClicked);
	PrevButton->OnClicked.AddDynamic(this, &USlideshowControls::PrevButtonClicked);

	return true;
}

void USlideshowControls::NextButtonClicked() {

	ParentSlideshow->NextImage();

}

void USlideshowControls::PrevButtonClicked() {

	ParentSlideshow->PrevImage();

}
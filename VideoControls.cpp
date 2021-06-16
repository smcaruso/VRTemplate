// Fill out your copyright notice in the Description page of Project Settings.


#include "VideoControls.h"

bool UVideoControls::Initialize() {

	if (!Super::Initialize()) return false;

	if (!PlayPauseButton || !ResetButton) return false;

	PlayPauseButton->OnClicked.AddDynamic(this, &UVideoControls::PlayPausePressed);
	ResetButton->OnClicked.AddDynamic(this, &UVideoControls::ResetPressed);

	return true;
}

void UVideoControls::PlayPausePressed() {

	ParentVideoViewer->PlayPause();

}

void UVideoControls::ResetPressed() {

	ParentVideoViewer->Reset();

}
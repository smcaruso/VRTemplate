// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerInfoWidget.h"

void UControllerInfoWidget::SetMessageText(FString NewMessage) {

	MessageText->SetText(FText::FromString(NewMessage));
	
}
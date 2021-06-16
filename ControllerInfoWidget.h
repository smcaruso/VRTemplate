// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ControllerInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class VRTEMPLATE_API UControllerInfoWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetMessageText(FString NewMessage);

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget)) // THIS binds the variable here to an object in UMG designer
	UTextBlock* MessageText;

private:

};

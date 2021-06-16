// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "SlideshowObjectViewer.h"
#include "SlideshowControls.generated.h"

/**
 * 
 */
UCLASS()
class VRTEMPLATE_API USlideshowControls : public UUserWidget
{
	GENERATED_BODY()

public:

	bool Initialize() override;

	void SetParentSlideshow(ASlideshowObjectViewer* NewParent) { ParentSlideshow = NewParent; }

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget)) // THIS binds the variable here to an object in UMG designer
	UButton* NextButton;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget)) // THIS binds the variable here to an object in UMG designer
	UButton* PrevButton;

private:

	UPROPERTY()
	ASlideshowObjectViewer* ParentSlideshow;

	UFUNCTION()
	void NextButtonClicked();

	UFUNCTION()
	void PrevButtonClicked();
	
};

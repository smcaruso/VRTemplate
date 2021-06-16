// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "VideoViewer.h"
#include "VideoControls.generated.h"

/**
 * 
 */
UCLASS()
class VRTEMPLATE_API UVideoControls : public UUserWidget
{
	GENERATED_BODY()

public:

	bool Initialize() override;

	void SetParentVideoViewer(AVideoViewer* NewParent) { ParentVideoViewer = NewParent; }

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget)) // THIS binds the variable here to an object in UMG designer
	UButton* PlayPauseButton;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget)) // THIS binds the variable here to an object in UMG designer
	UButton* ResetButton;

private:

	UPROPERTY()
	AVideoViewer* ParentVideoViewer;

	UFUNCTION()
	void PlayPausePressed();

	UFUNCTION()
	void ResetPressed();

};

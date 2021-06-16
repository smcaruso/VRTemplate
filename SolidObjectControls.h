// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "SolidObjectViewer.h"
#include "SolidObjectControls.generated.h"

/**
 * 
 */
UCLASS()
class VRTEMPLATE_API USolidObjectControls : public UUserWidget
{
	GENERATED_BODY()
	
public:

	bool Initialize() override;

	void SetParentViewer(ASolidObjectViewer* NewParent) { ParentViewer = NewParent; }

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget)) // THIS binds the variable here to an object in UMG designer
	UButton* LeftButton;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget)) // THIS binds the variable here to an object in UMG designer
	UButton* RightButton;

private:

	UPROPERTY()
	ASolidObjectViewer* ParentViewer;

	UFUNCTION()
	void RightButtonClicked();

	UFUNCTION()
	void LeftButtonClicked();

};

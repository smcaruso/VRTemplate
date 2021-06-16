// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "IntroVideoPanel.h"
#include "IntroVideoWidget.generated.h"

/**
 * 
 */
UCLASS()
class VRTEMPLATE_API UIntroVideoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	bool Initialize() override;
	
	void SetParentPanel(AIntroVideoPanel* NewParent) { ParentPanel = NewParent; }

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget)) // THIS binds the variable here to an object in UMG designer
	UButton* ClosePanelButton;

private:

	UPROPERTY()
	AIntroVideoPanel* ParentPanel;

	UFUNCTION()
	void ClosePanelClicked();

};

// Fill out your copyright notice in the Description page of Project Settings.


#include "PointerController.h"
#include "InfoObject.h"
#include "SelectorSwatch.h"
#include "Components/WidgetInteractionComponent.h"
#include "ControllerInfoWidget.h"

APointerController::APointerController() {

	PrimaryActorTick.bCanEverTick = true;

	WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Widget Interaction Component"));
	WidgetInteraction->SetupAttachment(GetRootComponent());

	PointerCursor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Widget Pointer Cursor"));
	PointerCursor->SetupAttachment(GetRootComponent());

}

void APointerController::BeginPlay()
{
	Super::BeginPlay();
	Super::SetRightHand();
}

void APointerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GrabbedActor != nullptr) {

		GrabbedActor->SetActorTransform(GetActorTransform());

	}

	if (WidgetInteraction->IsOverFocusableWidget()) {
		PointerCursor->SetVisibility(true);
		FHitResult WidgetInteractionPoint = WidgetInteraction->GetLastHitResult();
		PointerCursor->SetWorldLocation(WidgetInteractionPoint.ImpactPoint);
		SetMessageText(TEXT("Touch buttons with trigger."));

	}
	else {
		PointerCursor->SetVisibility(false);
	}
}

void APointerController::PressGrip() {

	TArray<AActor*> OverlappedActors;

	GetOverlappingActors(OverlappedActors);

	if (OverlappedActors.Num() < 1) {
		SetMessageText(TEXT("Nothing here to pick up."));
	}

	for (AActor* OverlappedActor : OverlappedActors) {

		if (OverlappedActor == nullptr) return;

		if (OverlappedActor->ActorHasTag(TEXT("Grabbable"))) {
			GrabbedActor = OverlappedActor;
			Cast<AInfoObject>(GrabbedActor)->PickedUp();
			SetMeshVisible(false); // hide controller mesh
			break;
		}

		if (OverlappedActor->ActorHasTag(TEXT("Swatch"))) {
			GrabbedActor = OverlappedActor;
			Cast<ASelectorSwatch>(GrabbedActor)->PickedUp();
			SetMeshVisible(false); // hide controller mesh
			break;
		}

	}
	

}

void APointerController::ReleaseGrip() {


	if (GrabbedActor != nullptr && GrabbedActor->ActorHasTag(TEXT("Grabbable"))) {
		Cast<AInfoObject>(GrabbedActor)->PutDown();
		GrabbedActor = nullptr;
	}

	if (GrabbedActor != nullptr && GrabbedActor->ActorHasTag(TEXT("Swatch"))) {
		Cast<ASelectorSwatch>(GrabbedActor)->PutDown();
		GrabbedActor = nullptr;
	}

	SetMeshVisible(true); // show controller mesh

}

void APointerController::PullTrigger() {

	WidgetInteraction->PressPointerKey(EKeys::LeftMouseButton);

}

void APointerController::ReleaseTrigger() {

	WidgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);

}

void APointerController::Scroll(float ScrollAmt) {

	WidgetInteraction->ScrollWheel(ScrollAmt);

}
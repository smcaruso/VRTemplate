// Fill out your copyright notice in the Description page of Project Settings.


#include "InfoObject.h"
#include "Components/WidgetComponent.h"
#include "Curves/CurveFloat.h"
#include "Kismet/GameplayStatics.h" 
#include "Sound/SoundCue.h" 

// Sets default values
AInfoObject::AInfoObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object Mesh"));
	SetRootComponent(ObjectMesh);

	InfoPanel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Info Panel Mesh"));
	InfoPanel->SetupAttachment(ObjectMesh);

	InfoPanelWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Info Panel Widget"));
	InfoPanelWidget->SetupAttachment(ObjectMesh);

}

// Called when the game starts or when spawned
void AInfoObject::BeginPlay()
{
	Super::BeginPlay();
	
	InfoPanel->SetVisibility(false, true);
	InfoPanelWidget->SetVisibility(false, true);

}

// Called every frame
void AInfoObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InfoOpenPercent < 100 && bIsPickedUp) {
		OpenInfo(DeltaTime);
	}

	if (InfoOpenPercent > 0 && !bIsPickedUp) {
		CloseInfo(DeltaTime);
	}

	if (InfoOpenPercent > 75 && bIsPickedUp) {
		InfoPanelWidget->SetVisibility(true, true);
	}

}

void AInfoObject::PickedUp() {

	ObjectMesh->SetSimulatePhysics(false);
	bIsPickedUp = true;
	InfoOpenPercent = 0.f;
	InfoPanel->SetVisibility(true, true);

	if (OpenSound != nullptr) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Cast<USoundBase>(OpenSound), this->GetActorLocation());
	}

}

void AInfoObject::PutDown() {

	ObjectMesh->SetSimulatePhysics(true);
	InfoPanelWidget->SetVisibility(false, true);

	bIsPickedUp = false;

	if (CloseSound != nullptr) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Cast<USoundBase>(CloseSound), this->GetActorLocation());
	}

}

void AInfoObject::OpenInfo(float DeltaTime) {

	if (InfoPanelOpenCurve != nullptr) {

		InfoPanel->SetWorldScale3D(FVector(InfoPanelXWidth, InfoPanelOpenCurve->GetFloatValue(InfoOpenPercent), InfoPanelZHeight));
		InfoOpenPercent += InfoOpenSpeed * DeltaTime;

	}

}

void AInfoObject::CloseInfo(float DeltaTime) {

	if (InfoPanelOpenCurve != nullptr) {

		InfoPanel->SetWorldScale3D(FVector(InfoPanelXWidth, InfoPanelOpenCurve->GetFloatValue(InfoOpenPercent), InfoPanelZHeight));
		InfoOpenPercent -= (InfoOpenSpeed * 2) * DeltaTime;

	}

}
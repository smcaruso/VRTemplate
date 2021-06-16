// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseObjectViewer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Curves/CurveFloat.h"
#include "Kismet/GameplayStatics.h" 
#include "Sound/SoundCue.h" 

// Sets default values
ABaseObjectViewer::ABaseObjectViewer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root Component"));
	SetRootComponent(RootSceneComponent);

	BoxCollisionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision Volume"));
	BoxCollisionVolume->SetupAttachment(GetRootComponent());
	BoxCollisionVolume->SetGenerateOverlapEvents(true);

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object Mesh"));
	ObjectMesh->SetupAttachment(GetRootComponent());
	ObjectMesh->SetGenerateOverlapEvents(false);

	InfoPanelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Info Panel Mesh"));
	InfoPanelMesh->SetupAttachment(GetRootComponent());
	InfoPanelMesh->SetGenerateOverlapEvents(false);


	InfoPanelWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Info Panel Widget"));
	InfoPanelWidget->SetupAttachment(InfoPanelMesh);
	InfoPanelWidget->SetGenerateOverlapEvents(false);

}

// Called when the game starts or when spawned
void ABaseObjectViewer::BeginPlay()
{
	Super::BeginPlay();

	BoxCollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &ABaseObjectViewer::BeginOverlap);
	BoxCollisionVolume->OnComponentEndOverlap.AddDynamic(this, &ABaseObjectViewer::EndOverlap);

	
	InfoPanelMesh->SetWorldScale3D(FVector(0.f, 0.f, 0.f));
	InfoPanelMesh->SetVisibility(false, true);

}

// Called every frame
void ABaseObjectViewer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InfoOpenPercent < 100 && bInfoOpen) {
		OpenInfo(DeltaTime);
	}

	if (InfoOpenPercent > 0 && !bInfoOpen) {
		CloseInfo(DeltaTime);
	}

	if (InfoOpenPercent > 85 && bInfoOpen) {
		InfoPanelWidget->SetVisibility(true);
	}

}

void ABaseObjectViewer::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (!OtherActor->ActorHasTag(TEXT("PlayerCapsule"))) return;

	Overlapper = OtherActor;
	bInfoOpen = true;
	InfoPanelMesh->SetVisibility(true, false);
	if (OpenSound != nullptr) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Cast<USoundBase>(OpenSound), this->GetActorLocation());
	}

}

void ABaseObjectViewer::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	if (!OtherActor->ActorHasTag(TEXT("PlayerCapsule"))) return;

	Overlapper = nullptr;
	bInfoOpen = false;
	InfoPanelWidget->SetVisibility(false);
	if (CloseSound != nullptr) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Cast<USoundBase>(CloseSound), this->GetActorLocation());
	}


}

void ABaseObjectViewer::OpenInfo(float DeltaTime) {

	if (InfoPanelOpenCurve != nullptr) {

		InfoPanelMesh->SetWorldScale3D(FVector(InfoPanelXWidth, InfoPanelOpenCurve->GetFloatValue(InfoOpenPercent), InfoPanelZHeight));
		InfoOpenPercent += InfoOpenSpeed * DeltaTime;

	}

}

void ABaseObjectViewer::CloseInfo(float DeltaTime) {

	if (InfoPanelOpenCurve != nullptr) {

		InfoPanelMesh->SetWorldScale3D(FVector(InfoPanelXWidth, InfoPanelOpenCurve->GetFloatValue(InfoOpenPercent), InfoPanelZHeight));
		InfoOpenPercent -= (InfoOpenSpeed * 2) * DeltaTime;

	}

}


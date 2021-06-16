// Fill out your copyright notice in the Description page of Project Settings.


#include "VRChar.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveFloat.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"
#include "Components/PostProcessComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Math/UnrealMathUtility.h" 
#include "AI/Navigation/NavigationTypes.h"
#include "Camera/PlayerCameraManager.h"
#include "MotionControllerComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

// Sets default values
AVRChar::AVRChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CamMan = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VR Root"));
	VRRoot->SetupAttachment(GetRootComponent());

	// FVector RaiseCam = FVector(0, 0, VRRootOffsetAmount);
	// VRRoot->AddWorldOffset(RaiseCam);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRRoot);

	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("Post-Processing Component"));
	PostProcessComponent->SetupAttachment(GetRootComponent());

	TeleportPath = CreateDefaultSubobject<USplineComponent>(TEXT("Teleport Path Spline"));
	TeleportPath->SetupAttachment(VRRoot);

	DestinationMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Destination Marker Mesh"));
	DestinationMarker->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AVRChar::BeginPlay()
{
	Super::BeginPlay();

	if (BlinderMaterialBase != nullptr) {
		PostProcessMaterial = UMaterialInstanceDynamic::Create(BlinderMaterialBase, this);
		PostProcessComponent->AddOrUpdateBlendable(PostProcessMaterial);
		PostProcessMaterial->SetScalarParameterValue(TEXT("Radius"), .35);
	}
	
	LeftController = GetWorld()->SpawnActor<AOculusControllerBase>(LeftControllerClass);
	if (LeftController != nullptr) {
		LeftController->AttachToComponent(VRRoot, FAttachmentTransformRules::KeepRelativeTransform);
		LeftController->SetOwner(this);
	}

	RightController = GetWorld()->SpawnActor<AOculusControllerBase>(RightControllerClass);
	if (RightController != nullptr) {
		RightController->AttachToComponent(VRRoot, FAttachmentTransformRules::KeepRelativeTransform);
		RightController->SetOwner(this);
	}

}

// Called every frame
void AVRChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CamLocation = Camera->GetComponentLocation();
	FVector NewCamOffset = CamLocation - GetActorLocation();
	FVector HandLocation = LeftController->GetActorLocation();

	HandLocation += LeftController->GetActorForwardVector() * HandStartOffset;
	NewCamOffset.Z = 0;

	AddActorWorldOffset(NewCamOffset);
	VRRoot->AddWorldOffset(-NewCamOffset);

	UpdateDestinationMarker(HandLocation);
	UpdateBlinders();

}

// Called to bind functionality to input
void AVRChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("ForwardAxis", this,	&AVRChar::MoveForward);
	PlayerInputComponent->BindAxis("SideAxis", this, &AVRChar::MoveSide);
	PlayerInputComponent->BindAxis("TurnAxis", this, &AVRChar::LookSide);
	PlayerInputComponent->BindAxis("SpeedMultiplier", this, &AVRChar::SpeedUp);
	PlayerInputComponent->BindAxis("TeleportGrip", this, &AVRChar::TeleportGrip);
	PlayerInputComponent->BindAction("Teleport", EInputEvent::IE_Released, this, &AVRChar::BeginTeleport);
	PlayerInputComponent->BindAction("Grip", EInputEvent::IE_Pressed, this, &AVRChar::PressGrip);
	PlayerInputComponent->BindAction("Grip", EInputEvent::IE_Released, this, &AVRChar::ReleaseGrip);
	PlayerInputComponent->BindAction("Trigger", EInputEvent::IE_Pressed, this, &AVRChar::PullTrigger);
	PlayerInputComponent->BindAction("Trigger", EInputEvent::IE_Released, this, &AVRChar::ReleaseTrigger);
	PlayerInputComponent->BindAxis("Scroll", this, &AVRChar::ScrollStick);


}

// Movement Functions

void AVRChar::SpeedUp(float AxisAmt) {

	SpeedMult = FMath::Clamp(AxisAmt * 5, 1.f, 5.f);

	if (AxisAmt > 0.1) {
		LeftController->SetMessageText(TEXT("Moving Faster."));
	}

}

void AVRChar::MoveForward(float FwdAmt) {

	AddMovementInput(
		LeftController->GetActorForwardVector(), // worlddirection
		FwdAmt * SpeedMult// value
		// bforce
	);

}

void AVRChar::MoveSide(float SideAmt) {

	AddMovementInput(
		LeftController->GetActorRightVector(),
		SideAmt * SpeedMult
	);

}

void AVRChar::LookSide(float LookAmt) {

	float NewYaw = 0;

	if (LookAmt < LookThumbstickThreshhold && LookAmt > -LookThumbstickThreshhold) {
		bDidLook = false;
	}
	if (LookAmt >= LookThumbstickThreshhold && !bDidLook) {
		NewYaw = QuickYawAmount;
		AddControllerYawInput(NewYaw);
		RightController->SetMessageText(TEXT("Look Right"));
		bDidLook = true;
	}

	if (LookAmt <= -LookThumbstickThreshhold && !bDidLook) {
		NewYaw = -QuickYawAmount;
		AddControllerYawInput(NewYaw);
		RightController->SetMessageText(TEXT("Look Left"));
		bDidLook = true;
	}


}

void AVRChar::UpdateBlinders() {

	if (RadiusVsVelocity == nullptr) return;
	PostProcessMaterial->SetScalarParameterValue(TEXT("Radius"), RadiusVsVelocity->GetFloatValue(GetVelocity().Size()));

}

// Teleport Functions

void AVRChar::TeleportGrip(float AxisAmt) {

	TeleportGripAxis = AxisAmt;

}

bool AVRChar::FindTeleportDestination(FVector& Start, FVector& OutLocation, TArray<FVector>& OutPath) {

	FHitResult TraceHit;
	FVector Look = LeftController->GetActorForwardVector();
	Look = Look.RotateAngleAxis(FireAngleOffset, LeftController->GetActorRightVector()); // tips path downward for easier aiming

	FPredictProjectilePathParams PathParams = FPredictProjectilePathParams(
		TeleportParabolaRadius, // float InProjectileRadius,
		Start, // FVector InStartLocation,
		Look * TeleportParabolaSpeed, // FVector InLaunchVelocity,
		TeleportSimulationTime, // float InMaxSimTime,
		ECollisionChannel::ECC_Visibility, // ECollisionChannel InTraceChannel,
		this // AActor * ActorToIgnore
	);

	FPredictProjectilePathResult PathResult;
	
	bool bTraceSuccess = UGameplayStatics::PredictProjectilePath(
		this, // world context object
		PathParams, // FPredictProjectilePathParams
		PathResult // FPredictProjectilePathResult
	);

	if (!bTraceSuccess) { return bTraceSuccess; }

	FNavLocation NavLocation;

	bool bNavProjectSuccess = NavSystem->ProjectPointToNavigation(
		PathResult.HitResult.Location, // point
		NavLocation, // outlocation
		TeleportProjectionExtent // extent
		// nav agent properties
		// queryfilter
	);

	if (!bNavProjectSuccess) { return bNavProjectSuccess; }

	for (FPredictProjectilePathPointData Point : PathResult.PathData) {
		OutPath.Add(Point.Location);
	}

	OutLocation = NavLocation.Location;
	return bTraceSuccess && bNavProjectSuccess;

}

void AVRChar::UpdateSpline(const TArray<FVector>& Path) {

	TeleportPath->ClearSplinePoints(false);

	for (int32 i = 0; i < Path.Num(); ++i) {
		FVector LocalPosition = TeleportPath->GetComponentTransform().InverseTransformPosition(Path[i]);
		FSplinePoint NewPoint = FSplinePoint(i, LocalPosition, ESplinePointType::Curve);
		TeleportPath->AddPoint(NewPoint, false);
	}

	TeleportPath->UpdateSpline();

}

void AVRChar::DrawTeleportPath(const TArray<FVector>& Path) {

	UpdateSpline(Path);

	for (USplineMeshComponent* SplineMesh : TeleportPathMeshArray) {
		SplineMesh->SetVisibility(false);
	}

	int32 SegmentNum = Path.Num() - 1; // we have one fewer segments than points in the spline

	for (int32 i = 0; i < SegmentNum; ++i) {
		
		if (TeleportPathMeshArray.Num() <= i) {

			USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this);
			SplineMesh->SetMobility(EComponentMobility::Movable);
			SplineMesh->AttachToComponent(TeleportPath, FAttachmentTransformRules::KeepRelativeTransform);
			SplineMesh->SetStaticMesh(TeleportArcMesh);
			SplineMesh->SetGenerateOverlapEvents(false);
			SplineMesh->SetMaterial(0, TeleportArcMaterial);
			SplineMesh->RegisterComponent();

			TeleportPathMeshArray.Add(SplineMesh);

		}

		USplineMeshComponent* SplineMesh = TeleportPathMeshArray[i];
		SplineMesh->SetVisibility(true);

		FVector StartLoc, StartTan, EndLoc, EndTan;

		TeleportPath->GetLocalLocationAndTangentAtSplinePoint(
			i, // int32 point index
			StartLoc, // out location
			StartTan // out tangent
		);

		TeleportPath->GetLocalLocationAndTangentAtSplinePoint(
			i + 1, // int32 point index
			EndLoc, // out location
			EndTan // out tangent
		);

		SplineMesh->SetStartAndEnd(
			StartLoc, // start location
			StartTan, // start tangent
			EndLoc, // end location
			EndTan, // end tangent
			true // bool update?
		);

	}

}

void AVRChar::UpdateDestinationMarker(FVector Start) {

	TArray<FVector> Path;
	FVector Location;
	bool bHasDestination = FindTeleportDestination(Start, Location, Path);

	if (bHasDestination && TeleportGripAxis > 0.1) {
		DestinationMarker->SetVisibility(true);
		DestinationMarker->SetWorldLocation(Location);
		DrawTeleportPath(Path);
		LeftController->SetMessageText(TEXT("Release to Teleport."));
	}

	else {
		DestinationMarker->SetVisibility(false);
		TArray<FVector> EmptyPath;
		DrawTeleportPath(EmptyPath);
	}

}

void AVRChar::BeginTeleport() {

	CamMan->StartCameraFade(
		0, // from alpha
		1, // to alpha
		TeleportFadeTime, // duration
		FLinearColor::Black, // fade color FLinearColor
		false, // should fade audio?
		true // hold when finished?
	);

	FTimerHandle WaitASecond;

	GetWorldTimerManager().SetTimer(
		WaitASecond, // timer handle
		this, // user class
		&AVRChar::MoveTeleport, // delegate
		TeleportFadeTime // rate
		// loop? false default
		// first delay
	);

}

void AVRChar::MoveTeleport() {

	FVector Destination = DestinationMarker->GetComponentLocation();
	Destination += GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * GetActorUpVector();

	SetActorLocation(Destination);

	FTimerHandle WaitAnotherSecond;

	GetWorldTimerManager().SetTimer(
		WaitAnotherSecond, // timer handle
		this, // user class
		&AVRChar::EndTeleport, // delegate
		TeleportFadeTime / 2 // rate
		// loop?
		// first delay
	);

}
void AVRChar::EndTeleport() {

	CamMan->StartCameraFade(
		1, // from alpha
		0, // to alpha
		TeleportFadeTime, // duration
		FLinearColor::Black, // FLinearColor
		false, // should fade audio?
		true // hold when finished?
	);

}
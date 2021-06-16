// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OculusControllerBase.h"
#include "PointerController.h"
#include "VRChar.generated.h"

UCLASS()
class VRTEMPLATE_API AVRChar : public ACharacter
{
	GENERATED_BODY()

public:
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Sets default values for this character's properties
	AVRChar();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	

	// Component Declarations

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* VRRoot;

	UPROPERTY(VisibleAnywhere)
	AOculusControllerBase* LeftController;

	UPROPERTY(VisibleAnywhere)
	AOculusControllerBase* RightController;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY()
	class APlayerCameraManager* CamMan;

	UPROPERTY(VisibleAnywhere)
	class USplineComponent* TeleportPath;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* DestinationMarker;

	UPROPERTY()
	class UNavigationSystemV1* NavSystem;

	UPROPERTY(VisibleAnywhere)
	class UPostProcessComponent* PostProcessComponent;

	UPROPERTY()
	class UMaterialInstanceDynamic* PostProcessMaterial;

	UPROPERTY(EditAnywhere)
	class UCurveFloat* RadiusVsVelocity;

	UPROPERTY()
	TArray<class USplineMeshComponent*> TeleportPathMeshArray;

	// Parameter Declarations

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AOculusControllerBase> LeftControllerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AOculusControllerBase> RightControllerClass;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* BlinderMaterialBase;

	UPROPERTY(EditDefaultsOnly)
	float LookThumbstickThreshhold = 0.7;

	// UPROPERTY(EditDefaultsOnly)
	// float VRRootOffsetAmount = 75.f;

	// Teleportation parameter declarations

	UPROPERTY(EditAnywhere)
	float MaxTeleportDistance = 1000.f;

	UPROPERTY(EditAnywhere)
	float TeleportParabolaRadius = 10.f;

	UPROPERTY(EditAnywhere)
	float TeleportParabolaSpeed = 1000.f;

	UPROPERTY(EditAnywhere)
	float TeleportSimulationTime = 10.f;

	UPROPERTY(EditAnywhere)
	float TeleportFadeTime = .25f;

	UPROPERTY(EditAnywhere)
	float HandStartOffset = 1.f;

	UPROPERTY(EditAnywhere)
	float QuickYawAmount = 45.f;

	UPROPERTY(EditAnywhere)
	float FireAngleOffset = -15.f;

	UPROPERTY(EditAnywhere)
	FVector TeleportProjectionExtent = FVector(100, 100, 100);

	UPROPERTY(EditDefaultsOnly)
	class UStaticMesh* TeleportArcMesh;

	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* TeleportArcMaterial;

	// Movement Functions

	void MoveForward(float FwdAmt);
	void SpeedUp(float AxisAmt);
	void MoveSide(float SideAmt);
	void LookSide(float LookAmt);
	void UpdateBlinders();

	// Teleport Functions

	bool FindTeleportDestination(FVector& Start, FVector& OutLocation, TArray<FVector>& OutPath);
	void UpdateSpline(const TArray<FVector>& Path);
	void DrawTeleportPath(const TArray<FVector>& Path);
	void UpdateDestinationMarker(FVector Start);
	void TeleportGrip(float AxisAmt);
	void BeginTeleport();
	void MoveTeleport();
	void EndTeleport();

	// Interaction Functions

	void PressGrip() { Cast<APointerController>(RightController)->PressGrip(); }
	void ReleaseGrip() { Cast<APointerController>(RightController)->ReleaseGrip(); }

	void PullTrigger() { Cast<APointerController>(RightController)->PullTrigger(); }
	void ReleaseTrigger() { Cast<APointerController>(RightController)->ReleaseTrigger(); }
	void ScrollStick(float ScrollAmt) { Cast<APointerController>(RightController)->Scroll(ScrollAmt); }

	// State

	bool bDidLook = false;
	float SpeedMult = 1;
	float TeleportGripAxis = 0;

};

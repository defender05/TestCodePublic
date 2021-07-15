// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

#include "BL_JetBase.generated.h"

//#include "Math/TransformNonVectorized.h"

//Forward declaration
class USpringArmComponent;
class UCameraComponent;
class UBoxComponent;
class UArrowComponent;
class UWidgetComponent;
class UBL_InputFilter;
class ABL_ProjectileBase;

UENUM(BlueprintType)
enum class EWeaponList : uint8
{
	MachineGun,
	AirAirMissle,
	AirGroundMissle,
	Nurs
};

UCLASS()
class BATTLELINES_API ABL_JetBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABL_JetBase();
	
//Components
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USkeletalMeshComponent* Body;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UStaticMeshComponent* FireCone_L;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UStaticMeshComponent* FireCone_R;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UStaticMeshComponent* HeatShape_L;
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UStaticMeshComponent* HeatShape_R;
	
// Custom Actor Components - Start
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBL_InputFilter* ForwardFilter;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBL_InputFilter* RightFilter;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBL_InputFilter* RollFilter;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBL_InputFilter* PitchFilter;
// - End

	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UCameraComponent* ExternalCamera;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UCameraComponent* PilotsCamera;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USceneComponent* SteerForward;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USceneComponent* SteerRear;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USceneComponent* Eleron_L;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USceneComponent* Eleron_R;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USceneComponent* Elevator;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USceneComponent* WingBody;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USceneComponent* Wing_L;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USceneComponent* Wing_R;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USceneComponent* Wing_RL;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USceneComponent* Wing_RR;


	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		USceneComponent* MachineGun_Muzzle;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USceneComponent* BombMuzzle_L;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USceneComponent* BombMuzzle_R;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USceneComponent* NursMuzzle_L;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USceneComponent* NursMuzzle_R;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USceneComponent* AirAirMuzzle;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USceneComponent* AirGroundMuzzle;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UAudioComponent* PilotSound;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UAudioComponent* EngineSound;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UAudioComponent* MachineGunSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UWidgetComponent* WB;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UArrowComponent* ForwardArrow;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* InteractTrigger;

	
	//Delta Time
	UPROPERTY(BlueprintReadOnly, Category = "DeltaTime")
		float DeltaSec;

	//Engine Vars
	UPROPERTY(BlueprintReadWrite)
		bool EngineState;
	UPROPERTY(BlueprintReadWrite)
		float CurrentTorque;
	UPROPERTY(BlueprintReadWrite)
		float TurboTorque;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float FlyMinTorque;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float MaxTorque;
	UPROPERTY(BlueprintReadWrite)
		float CurrentHeight;
	UPROPERTY(BlueprintReadWrite)
		float Velocity_MS;
	UPROPERTY(BlueprintReadWrite)
		float Velocity_KMH;

	// Getter for Speed - KM\H
	UFUNCTION(BlueprintPure, Category = "HUD")
		FORCEINLINE float GetCurrentSpeed() const { return UKismetMathLibrary::VSize(Body->GetPhysicsLinearVelocity() * 0.036f); }
	// Getter for Height - Meters
	UFUNCTION(BlueprintPure, Category = "HUD")
		FORCEINLINE float GetCurrentHeight() const { return CurrentHeight / 100.0f; }
	// Getter for SlipValue - Vector
	UFUNCTION(BlueprintPure, Category = "HUD")
		FVector GetSlipValue() const;

	//Fly Speeds
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Fly")
		float FlyPitchSpeed;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Fly")
		float FlyRollSpeed;

	//Tick Functions
	UFUNCTION(BlueprintCallable, Category = "Tick")
		void GetHeight();
	UFUNCTION(BlueprintCallable, Category = "Tick")
		void ApplyAirForce();
	UFUNCTION(BlueprintCallable, Category = "Tick")
		void LiftForce();
	UFUNCTION(BlueprintCallable, Category = "Tick")
		void SetLinearDamping();
	UFUNCTION(BlueprintCallable, Category = "Tick")
		void SetEngineSoundParameter();
	UFUNCTION(BlueprintCallable, Category = "Tick")
		void SetFireHeatMatParameter();
	
	//Weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
		EWeaponList WeaponState;
	UFUNCTION(BlueprintCallable, Category="Weapon")
		void ChangeWeapon();
	UFUNCTION(BlueprintCallable, Reliable, Server, Category = "Weapon")
		void FireMachineGun();
	UFUNCTION(BlueprintCallable, Reliable, Server, Category = "Weapon")
		void FireAirAirMissle(AActor* TargetActor);
	UFUNCTION(BlueprintCallable, Reliable, Server, Category = "Weapon")
		void FireAirGroundMissle(AActor* TargetActor);
	UFUNCTION(BlueprintCallable, Reliable, Server, Category = "Weapon")
		void FireNurs();
	
	//Firing
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
		FVector CameraTraceHitLoc;
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
		bool bCrosshairTraceDetected;
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
		bool bDoOnceMissleFire;
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
		AActor* CrosshairHitActor;
	
	UFUNCTION(Category = "Weapon")
		void StartMissleAimTimer();
	UFUNCTION(Category = "Weapon")
		void CanFireMissle();
	
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
		int WeaponIndex;
	/**Max count weapon to use**/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
		int MaxWeaponIndex;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
		float ShootDistance;
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
		bool bCanFireMissle;
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
		AActor* MissleAimHitActor;


	//Camera
	UFUNCTION(BlueprintCallable, Category = "Camera")
		void PilotCameraControl(float AxisYaw, float AxisPitch);
	UFUNCTION(BlueprintCallable, Category = "Camera")
		void CameraZoom(float MouseWheelAxis);
	
	//Camera Zoom
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera")
		float ZoomMin;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera")
		float ZoomMax;

	//Camera Restrictions
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera")
		float PilotCameraPitchMin;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera")
		float PilotCameraPitchMax;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera")
		float PilotCameraYawMin;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera")
		float PilotCameraYawMax;
	
	//Movement Force
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Movement")
		float MoveRightForce;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Movement")
		float RollForce;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Movement")
		float PitchForce;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Movement")
		float MinLiftForce;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Movement")
		float MaxLiftForce;
	
	// Optimal speed for turning. Default = 350 km\h
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Movement")
		float OptimalSpeed;

	//Movement and Fly
	UFUNCTION(BlueprintCallable, Category = "Movement")
		void SetCurrentTorque(float AxisValue);
	UFUNCTION(BlueprintCallable, Reliable, Server, Category = "Movement")
		void MoveForward(FName BoneName, bool AccelChange);
	UFUNCTION(BlueprintCallable, Reliable, Server, Category = "Movement")
		void MoveRight(float AxisValue);
	UFUNCTION(BlueprintCallable, Reliable, Server, Category = "Movement")
		void RollControl(float AxisValue);
	UFUNCTION(BlueprintCallable, Reliable, Server, Category = "Movement")
		void PitchControl(float AxisValue);

	
	//Only for AnimBP 
	UPROPERTY(BlueprintReadOnly, Category = "AnimBP")
		float EleronAnim_L;
	UPROPERTY(BlueprintReadOnly, Category = "AnimBP")
		float EleronAnim_R;
	UPROPERTY(BlueprintReadOnly, Category = "AnimBP")
		float RudderAnim;
	UPROPERTY(BlueprintReadOnly, Category = "AnimBP")
		float ElevatorsAnim;
	
	
	//Health
	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxHealth;
	UPROPERTY(ReplicatedUsing = "OnRep_CurrentHealth")
		float CurrentHealth;
	//Bool
	UPROPERTY(BlueprintReadWrite)
		bool bisDead;
	UPROPERTY(BlueprintReadWrite)
		bool RightMousePressed;
	
	UFUNCTION(Category = "Health")
		void OnRep_CurrentHealth();
	/** Property replication */
		void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/** Getter for Max Health.*/
	UFUNCTION(BlueprintPure, Category = "Health")
		FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	/** Getter for Current Health.*/
	UFUNCTION(BlueprintPure, Category = "Health")
		FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	/** Setter for Current Health. Clamps the value between 0 and MaxHealth and calls OnHealthUpdate. Should only be called on the server.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetCurrentHealth(float healthValue);
	/** Event for taking damage. Overridden from APawn.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(Category = "Weapon")
	void CrosshairDetectTrace();
	
	/** Response to health being updated. Called on the server immediately after modification, and on clients in response to a RepNotify*/
	void OnHealthUpdate();

	UPROPERTY(EditAnywhere, Category = "Shooting")
		TSubclassOf<ABL_ProjectileBase> MachineGunProjectileClass;
	UPROPERTY(EditAnywhere, Category = "Shooting")
		TSubclassOf<ABL_ProjectileBase> AirAirProjectileClass;
	UPROPERTY(EditAnywhere, Category = "Shooting")
		TSubclassOf<ABL_ProjectileBase> AirGroundProjectileClass;

	//Weapon Fire Rates
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Shooting")
		float MachineGunFireRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Shooting")
		float AirAirFireRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Shooting")
		float AirGroundFireRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Shooting")
		float NursFireRate;
	
	/** If true, we are in the process of firing projectiles. */
		bool bIsFiringWeapon;
	/** Function for beginning weapon fire.*/
	UFUNCTION(BlueprintCallable, Category = "Shooting")
		void StartFire();
	/** Function for ending weapon fire. Once this is called, the player can use StartFire again.*/
	UFUNCTION(BlueprintCallable, Category = "Shooting")
		void StopFire();
	/** Server function for spawning projectiles.*/
	UFUNCTION(BlueprintCallable, Reliable, Server, Category = "Shooting")
		void HandleFire();
	UFUNCTION(BlueprintCallable, Reliable, Server)
		void Death();
	/** A timer handle used for providing the fire rate delay in-between spawns.*/
		FTimerHandle FiringTimer;
	//Missle Crosshair Aiming Timer
		FTimerHandle MissleAimigTimer;

	//Anim BP - Only for edit by defaults
	UPROPERTY(EditDefaultsOnly, Category = "AnimBP")
		float EleronAngle_L;
	UPROPERTY(EditDefaultsOnly, Category = "AnimBP")
		float EleronAngle_R;
	UPROPERTY(EditDefaultsOnly, Category = "AnimBP")
		float RudderAngle;
	UPROPERTY(EditDefaultsOnly, Category = "AnimBP")
		float ElevatorsAngle;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "AnimBP") //Исключение - логика реализована в бп
		float ChassisAngle;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PossessedBy(AController* NewController) override;
	void UnPossessed() override;
};

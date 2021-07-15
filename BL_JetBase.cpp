// Fill out your copyright notice in the Description page of Project Settings.

#include "BL_JetBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/GameEngine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Math/TransformCalculus3D.h"
#include "Components/AudioComponent.h"
#include "DrawDebugHelpers.h"
#include "BL_InputFilter.h"
#include "BL_ProjectileBase.h"


//#include "InputState.h"



// Sets default values
ABL_JetBase::ABL_JetBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

//Create default component
	Body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Root Mesh"));
	RootComponent = Body;
	FireCone_L = CreateDefaultSubobject<UStaticMeshComponent>("FireCone_L");
	FireCone_R = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireCone_R"));
	HeatShape_L = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeatShape_L"));
	HeatShape_R = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeatShape_R"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 336.0f);
	SpringArm->TargetArmLength = 1500.0f;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	ExternalCamera = CreateDefaultSubobject<UCameraComponent>("ExternalCamera");
	PilotsCamera = CreateDefaultSubobject<UCameraComponent>("PilotCamera");
	PilotsCamera->SetAutoActivate(false);
	PilotsCamera->Deactivate();
	
	SteerForward = CreateDefaultSubobject<USceneComponent>("SteerForward");
	SteerRear = CreateDefaultSubobject<USceneComponent>("SteerRear");
	Eleron_L = CreateDefaultSubobject<USceneComponent>("Eleron_L");
	Eleron_R = CreateDefaultSubobject<USceneComponent>("Eleron_R");
	Elevator = CreateDefaultSubobject<USceneComponent>("Elevator");

	WingBody = CreateDefaultSubobject<USceneComponent>("WingBody");
	Wing_L = CreateDefaultSubobject<USceneComponent>("Wing_L");
	Wing_R = CreateDefaultSubobject<USceneComponent>("Wing_R");
	Wing_RL = CreateDefaultSubobject<USceneComponent>("Wing_RL");
	Wing_RR = CreateDefaultSubobject<USceneComponent>("Wing_RR");
	
	MachineGun_Muzzle = CreateDefaultSubobject<USceneComponent>("MachineGun Muzzle");
	BombMuzzle_L = CreateDefaultSubobject<USceneComponent>("BombMuzzle_L");
	BombMuzzle_R = CreateDefaultSubobject<USceneComponent>("BombMuzzle_R");
	NursMuzzle_L = CreateDefaultSubobject<USceneComponent>("NursMuzzle_L");
	NursMuzzle_R = CreateDefaultSubobject<USceneComponent>("NursMuzzle_R");
	AirAirMuzzle = CreateDefaultSubobject<USceneComponent>("AirAirMuzzle");
	AirGroundMuzzle = CreateDefaultSubobject<USceneComponent>("AirGroundMuzzle");

	EngineSound = CreateDefaultSubobject<UAudioComponent>("EngineSound");
	PilotSound = CreateDefaultSubobject<UAudioComponent>("PilotSound");
	MachineGunSound = CreateDefaultSubobject<UAudioComponent>("MachineGunSound");

	WB = CreateDefaultSubobject<UWidgetComponent>("WB");
	
	ForwardArrow = CreateDefaultSubobject<UArrowComponent>("ForwardArrow");
    InteractTrigger = CreateDefaultSubobject<UBoxComponent>("InteractTrigger");

	//Custom Actor Components
	ForwardFilter = CreateDefaultSubobject<UBL_InputFilter>("ForwardFilter");
	ForwardFilter->CenterSpeed = 1.0f;
	ForwardFilter->RiseSpeed = 1.0f;
	RightFilter = CreateDefaultSubobject<UBL_InputFilter>("RightFilter");
	RollFilter = CreateDefaultSubobject<UBL_InputFilter>("RollFilter");
	PitchFilter = CreateDefaultSubobject<UBL_InputFilter>("PitchFilter");
    
//Hierarchy
	FireCone_L->SetupAttachment(Body);
	FireCone_R->SetupAttachment(Body);
	HeatShape_L->SetupAttachment(FireCone_L);
	HeatShape_R->SetupAttachment(FireCone_R);
	SpringArm->SetupAttachment(Body);
	ExternalCamera->SetupAttachment(SpringArm);
	PilotsCamera->SetupAttachment(Body);
	
	SteerForward->SetupAttachment(Body);
	SteerRear->SetupAttachment(Body);
	Eleron_L->SetupAttachment(Body);
	Eleron_R->SetupAttachment(Body);
	Elevator->SetupAttachment(Body);
	
	WingBody->SetupAttachment(Body);
	Wing_L->SetupAttachment(Body);
	Wing_R->SetupAttachment(Body);
	Wing_RL->SetupAttachment(Body);
	Wing_RR->SetupAttachment(Body);
	
	MachineGun_Muzzle->SetupAttachment(Body);
	BombMuzzle_L->SetupAttachment(Body);
	BombMuzzle_R->SetupAttachment(Body);
	NursMuzzle_L->SetupAttachment(Body);
	NursMuzzle_R->SetupAttachment(Body);
	AirAirMuzzle->SetupAttachment(Body);
	AirGroundMuzzle->SetupAttachment(Body);

	EngineSound->SetupAttachment(Body);
	EngineSound->SetAutoActivate(false);
	PilotSound->SetupAttachment(PilotsCamera);
	PilotSound->SetAutoActivate(false);
	MachineGunSound->SetupAttachment(MachineGun_Muzzle);
	MachineGunSound->SetAutoActivate(false);
	
	WB->SetupAttachment(Body);
	WB->SetVisibility(false);

	ForwardArrow->SetupAttachment(Body);
	
//Initialize variables
	bReplicates = true;
	MaxHealth = 300.0f;
	CurrentHealth = MaxHealth;

	//Engine
	EngineState = false;
	CurrentTorque = 0.0f;
	TurboTorque = 20000.0f;
	FlyMinTorque = 14500.0f;
	MaxTorque = 17000.0f;
	CurrentHeight = 0.0f;
	Velocity_MS = 0.0f;
	Velocity_KMH = 0.0f;

	//Camera
	PilotCameraPitchMin = -45.0f;
	PilotCameraPitchMax = 70.0f;
	PilotCameraYawMin = -130.0f;
	PilotCameraYawMax = 130.0f;

	ZoomMin = 1500.0f;
	ZoomMax = 3000.0f;

	//Fly
	FlyPitchSpeed = 5.0f;
	FlyRollSpeed = 3.5f;
	MoveRightForce = 500000.0f;
	RollForce = 1000000.0f;
	PitchForce = 1000000.0f;
	OptimalSpeed = 350.0f;
	
	MinLiftForce = 10.0f;
	MaxLiftForce = 100.0f;

	//AnimBP
	EleronAnim_L = 0.0f;
	EleronAnim_R = 0.0f;
	ElevatorsAnim = 0.0f;
	RudderAnim = 0.0f;
	
	EleronAngle_L = 15.0f;
	EleronAngle_R = 15.0f;
	ElevatorsAngle = 15.0f;
	RudderAngle = -15.0f;

	//Weapon
	WeaponState = EWeaponList::MachineGun;
	WeaponIndex = 0;
	MaxWeaponIndex = 3;
	ShootDistance = 1000000.f;
	bIsFiringWeapon = false;
	bCanFireMissle = false;
	bCrosshairTraceDetected = false;
	bDoOnceMissleFire =true;
	MissleAimHitActor = nullptr;

	//Firing
	MachineGunFireRate = 0.1f;
	AirAirFireRate = 10.0f;
	AirGroundFireRate = 10.0f;
	NursFireRate = 0.2f;

	bisDead = false;
	RightMousePressed = false;
	
}

void ABL_JetBase::OnRep_CurrentHealth()
{
    OnHealthUpdate();
}

void ABL_JetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //Replicate current health.
    DOREPLIFETIME(ABL_JetBase, CurrentHealth);
}

//Custom Filter Axis for Move Forward 
void ABL_JetBase::SetCurrentTorque(float AxisValue)
{
	if (bisDead==false)
	{
		if (AxisValue!=0.0f)
		{
			CurrentTorque = UKismetMathLibrary::Clamp(
				CurrentTorque + (AxisValue * 50.0f),
				UKismetMathLibrary::SelectFloat(FlyMinTorque, 0.0f, CurrentHeight>1500.0f),
				MaxTorque);
		}
	}
};
//Fly Control 
void ABL_JetBase::MoveForward_Implementation(FName BoneName, bool AccelChange)
{
	//float Value = ForwardFilter -> UBL_InputFilter::GetValue(AxisValue, DeltaSec);
	
    FVector Force = CurrentTorque * ForwardArrow->GetForwardVector();
    Body->AddForce(Force, BoneName, AccelChange);
}
void ABL_JetBase::MoveRight_Implementation(float AxisValue)
{
	if (bisDead==false)
	{
		const float FilterAxis = RightFilter->GetValue(AxisValue, DeltaSec);
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Black, FString::SanitizeFloat(FilterAxis));

		//Apply Force to Body - Right, Left
	    const FVector ForwardRightVector = SteerForward->GetRelativeRotation().Vector().YAxisVector;
		const FVector RearRightVector = SteerRear->GetRelativeRotation().Vector().YAxisVector;
	    const FVector ForceForward = FilterAxis * MoveRightForce * ForwardRightVector;
		const FVector ForceRear = FilterAxis * -MoveRightForce * RearRightVector;
	    Body->AddForceAtLocationLocal(ForceForward, SteerForward->GetRelativeLocation());
		Body->AddForceAtLocationLocal(ForceRear, SteerRear->GetRelativeLocation());
		
		//Only for AnimBP
		RudderAnim = FilterAxis * RudderAngle;
		
	}
}
void ABL_JetBase::RollControl_Implementation(float AxisValue)
{
	if(bisDead==false)
	{
		if(PilotsCamera->IsActive()==true && RightMousePressed==true){}
		else
		{
			//Smooth Axis Value
			const float FilterAxis = RollFilter->GetValue(AxisValue, DeltaSec);
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::SanitizeFloat(FilterAxis));
			
			//Eleron AnimBP
			EleronAnim_L = UKismetMathLibrary::MapRangeClamped(FilterAxis, -1.0f, 1.0f, -EleronAngle_L, EleronAngle_L);
			EleronAnim_R = EleronAnim_L * -1.0f;
				
			//Roll Force
			if(CurrentTorque > 1000.0f && CurrentHeight > 10.0f)
			{
				const FVector RightVectorL = Eleron_L->GetRelativeRotation().Vector().YAxisVector;
				const FVector RightVectorR = Eleron_R->GetRelativeRotation().Vector().YAxisVector;
				const FVector ForceLeft = FilterAxis * RollForce * FlyRollSpeed * RightVectorL;
				const FVector ForceRight = FilterAxis * RollForce * FlyRollSpeed * RightVectorR;
				Body->AddForceAtLocationLocal(ForceLeft, Eleron_L->GetRelativeLocation());
				Body->AddForceAtLocationLocal(ForceRight, Eleron_R->GetRelativeLocation());
			}
		};
	}
}
void ABL_JetBase::PitchControl_Implementation(float AxisValue)
{
	if(bisDead==false)
	{
		//Smooth Axis Value
		const float FilterAxis = PitchFilter->GetValue(AxisValue, DeltaSec);
			
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::SanitizeFloat(FilterAxis, 2));
		//Eleron AnimBP
		ElevatorsAnim = UKismetMathLibrary::MapRangeClamped(FilterAxis, -1.0f, 1.0f, ElevatorsAngle, -ElevatorsAngle);

		//Select Value
		const float SelectValue = UKismetMathLibrary::SelectFloat
		(
			UKismetMathLibrary::MapRangeClamped(Velocity_KMH, 50.0f, OptimalSpeed, 0.0f, PitchForce),
			UKismetMathLibrary::MapRangeClamped(Velocity_KMH, OptimalSpeed, 480.0f, PitchForce, 700000.0f),
			Velocity_KMH<=OptimalSpeed
		);
				
		//Roll Force
		const FVector UpVector = Elevator->GetRelativeRotation().Vector().ZAxisVector;
		const FVector LocalPitchForce = FilterAxis * (SelectValue * -1.0f) * FlyPitchSpeed * UpVector;
		Body->AddForceAtLocationLocal(LocalPitchForce, Elevator->GetRelativeLocation());
	}
}

//Tick Functions
void ABL_JetBase::GetHeight()
{
	FHitResult WidgetTraceHit;
	FVector Start = GetActorLocation();
	FVector End = FVector(Start.X, Start.Y, -10000000.0f);
	FCollisionQueryParams CollisionParam;
	CollisionParam.AddIgnoredActor(this);
	
	//Box Trace for Distance to ground
	bool GroundHit = UKismetSystemLibrary::BoxTraceSingle
	(this, Start, End,
		FVector(700.0f, 300.0f, 1.0f), 
		FRotator(0.0f, 0.0f, GetActorRotation().Yaw), 
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		TArray<AActor*>{this},
		EDrawDebugTrace::None,
		WidgetTraceHit,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		1.0f);

	//SetCurrentHeight
	CurrentHeight = WidgetTraceHit.Distance;
	//Set Linear Damping
	Body->SetLinearDamping(UKismetMathLibrary::MapRangeClamped(0.0f, 10000.0f, 0.1f, 1,1.5f));
}
void ABL_JetBase::ApplyAirForce()
{
	if (IsValid(Body))
	{
		Velocity_MS = UKismetMathLibrary::VSize(Body->GetComponentVelocity()/100.0f); // м\с
		Velocity_KMH = UKismetMathLibrary::VSize(Body->GetComponentVelocity() * 0.036f); // км\ч
		
		//Box Empty Vars for GetComponentBounds
		float SphereRadius;
		FVector Origin;
		FVector BoxExtent;
		// AirForce = (U*U) * const 0.5 * const 0.4 * const 1.2754 * S(area)
		// U = Velocity M/S
		// S(area) = Body Bounds
		UKismetSystemLibrary::GetComponentBounds(Body, Origin, BoxExtent, SphereRadius);
		const float Area = UKismetMathLibrary::VSize(BoxExtent/2.0f);
		const FVector Force = UKismetMathLibrary::Conv_FloatToVector(UKismetMathLibrary::Square(Velocity_MS) * 0.5f * 0.4f * 1.2754f * Area);
		Body->AddForce(Force);
	}
}
void ABL_JetBase::LiftForce()
{
	if(Velocity_MS>1 && IsValid(Body))
	{
		float ForceMapRange = UKismetMathLibrary::MapRangeClamped(
			CurrentTorque,
			0.0f,
			TurboTorque,
			MinLiftForce, MaxLiftForce) * Body->GetMass();
		//FVector Force(0.0f, 0.0f, 0.0f);
		
		const int16 MaxWings = 4;
		for (int16 i = 0; i < MaxWings; ++i)
		{
			//FVector SelectedUpVector(0.0f, 0.0f, 0.0f);
			//FVector SelectedLocation(0.0f, 0.0f, 0.0f);
			//Select Scene Component Up Vector and Relative Location
			/**switch (i)
			{
				case 0:
					//SelectedUpVector = WingBody->GetRelativeRotation().Vector().ZAxisVector * -1;
					//SelectedLocation = WingBody->GetRelativeLocation();
				case 1:
					SelectedUpVector = Wing_L->GetRelativeRotation().Vector().ZAxisVector * -1;
					SelectedLocation = Wing_L->GetRelativeLocation();
				case 2:
					SelectedUpVector = Wing_R->GetRelativeRotation().Vector().ZAxisVector * -1;
					SelectedLocation = Wing_R->GetRelativeLocation();
				case 3:
					//SelectedUpVector = Wing_RL->GetRelativeRotation().Vector().ZAxisVector * -1;
					//SelectedLocation = Wing_RL->GetRelativeLocation();
				case 4:
					//SelectedUpVector = Wing_RR->GetRelativeRotation().Vector().ZAxisVector * -1;
					//SelectedLocation = Wing_RR->GetRelativeLocation();
				
				default:;
			}**/
			FVector BodyUpVector = WingBody->GetRelativeRotation().Vector().ZAxisVector;
			const FVector BodyLocation = WingBody->GetRelativeLocation();

			FVector WingLUpVector = Wing_L->GetRelativeRotation().Vector().ZAxisVector;
			const FVector WingLLocation = Wing_L->GetRelativeLocation();

			FVector WingRUpVector = Wing_R->GetRelativeRotation().Vector().ZAxisVector;
			const FVector WingRLocation = Wing_R->GetRelativeLocation();
			
			FVector BodyForce = ForceMapRange * BodyUpVector;
			FVector WingLForce = ForceMapRange * WingLUpVector;
			FVector WingRForce = ForceMapRange * WingRUpVector;
			
			Body->AddForceAtLocationLocal(BodyForce, BodyLocation);
			Body->AddForceAtLocationLocal(WingLForce, WingLLocation);
			Body->AddForceAtLocationLocal(WingRForce, WingRLocation);
		}
	}
}

void ABL_JetBase::SetLinearDamping()
{
	Body->SetLinearDamping(UKismetMathLibrary::MapRangeClamped(
		CurrentTorque,
		0.0f,
		10000.0f,
		0.1f,
		1.0f ));
}
void ABL_JetBase::SetEngineSoundParameter()
{
	if (IsValid(EngineSound))
	{
		float PitchValue = UKismetMathLibrary::MapRangeClamped(
		UKismetMathLibrary::MapRangeClamped(
			CurrentTorque, 0.0f, TurboTorque, 0.0f, 1.0f),
		0.0f,
		1.0f, 
		0.5f,
		1.5f
		);
		float VolumeParam = UKismetMathLibrary::SelectFloat(
			UKismetMathLibrary::Sqrt(PitchValue) * 0.5f,
			UKismetMathLibrary::Sqrt(PitchValue) * 1.1f,
			PilotsCamera->IsActive()==true
			);
		float Throtlle = UKismetMathLibrary::MapRangeClamped(
			CurrentTorque,
			0.0f,
			TurboTorque,
			0.0f,
			1.0f);
		float MixValue = UKismetMathLibrary::Sin(GetWorld()->GetTimeSeconds() * 0.2f) + 1.0f * 0.5f;
			
		EngineSound->SetFloatParameter("PitchParam", PitchValue);
		EngineSound->SetFloatParameter("VolumeParam", VolumeParam);
		EngineSound->SetFloatParameter("Throttle", Throtlle);
		EngineSound->SetFloatParameter("MixValue", MixValue);
	}
}
void ABL_JetBase::SetFireHeatMatParameter()
{
	if (IsValid(FireCone_L) && IsValid(FireCone_R) && IsValid(HeatShape_L) && IsValid(HeatShape_R))
	{
		const float TurboValue = UKismetMathLibrary::Square(
			UKismetMathLibrary::MapRangeClamped(
				CurrentTorque,
				0.0f,
				TurboTorque,
				0.0f,
				1.0f)) * 35.0f;
		const float FireConeScale = UKismetMathLibrary::MapRangeClamped(
				CurrentTorque,
				0.0f,
				TurboTorque,
				0.0f,
				1.0f) * 3.0f;
		const float HeatScale = UKismetMathLibrary::MapRangeClamped(
				CurrentTorque,
				0.0f,
				TurboTorque,
				0.0f,
				1.0f);
		
		Body->SetScalarParameterValueOnMaterials("AfterburnerScale", TurboValue);
		FireCone_L->SetRelativeScale3D(FVector(FireConeScale, 1.7f, 1.7f));
		FireCone_R->SetRelativeScale3D(FVector(FireConeScale, 1.7f, 1.7f));
		FireCone_L->SetScalarParameterValueOnMaterials("EmissiveMultiplier", TurboValue);
		FireCone_R->SetScalarParameterValueOnMaterials("EmissiveMultiplier", TurboValue);
		HeatShape_L->SetScalarParameterValueOnMaterials("HeatScale", HeatScale);
		HeatShape_R->SetScalarParameterValueOnMaterials("HeatScale", HeatScale);

		if (HeatScale>0)
		{
			HeatShape_L->SetVisibility(true);
		}
		else
		{
			HeatShape_L->SetVisibility(false);
		}
	}
}

void ABL_JetBase::ChangeWeapon()
{
	if (WeaponIndex < MaxWeaponIndex)
	{
		WeaponIndex = UKismetMathLibrary::Clamp(++WeaponIndex, 0, MaxWeaponIndex);
	}
	else
	{
		WeaponIndex = 0;
	}

	switch (WeaponIndex)
	{
		case 0: WeaponState = EWeaponList::MachineGun;
			break;
		case 1: WeaponState = EWeaponList::AirAirMissle;
			break;
		case 2: WeaponState = EWeaponList::AirGroundMissle;
			break;
		case 3: WeaponState = EWeaponList::Nurs;
			break;
		default: WeaponState = EWeaponList::MachineGun;
	}
	bIsFiringWeapon = false;
}

void ABL_JetBase::CrosshairDetectTrace()
{
	EObjectTypeQuery DetectType = EObjectTypeQuery::ObjectTypeQuery15;
	TArray<TEnumAsByte<EObjectTypeQuery>> DetectArray;
		DetectArray.Init(DetectType, 1);
	
	switch (WeaponState)
	{
		case EWeaponList::AirAirMissle: DetectType = EObjectTypeQuery::ObjectTypeQuery15;
			break;
		case EWeaponList::AirGroundMissle: DetectType = EObjectTypeQuery::ObjectTypeQuery12;
			break;
		default: DetectType = EObjectTypeQuery::ObjectTypeQuery15;
	}
	if (WeaponState==EWeaponList::AirAirMissle || WeaponState==EWeaponList::AirGroundMissle)
	{
		//Sphere Trace For Object
		FHitResult CrosshairTraceHit;
		FVector Start = WB->GetComponentLocation();
		FVector End = Start + WB->GetForwardVector() * ShootDistance;
		FCollisionQueryParams CollisionParam;
		CollisionParam.AddIgnoredActor(this);
		
		bCrosshairTraceDetected = UKismetSystemLibrary::SphereTraceSingleForObjects
		(this, Start, End,
			1000.0f, 
			DetectArray,
			false,
			TArray<AActor*>{this},
			EDrawDebugTrace::ForOneFrame,
			CrosshairTraceHit,
			true,
			FLinearColor::Black,
			FLinearColor::Yellow,
			1.0f);

		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);

		CrosshairHitActor = CrosshairTraceHit.GetActor();

		if (bCrosshairTraceDetected)
		{
			bDoOnceMissleFire = true;
			StartMissleAimTimer();
		}
		else
		{
			bDoOnceMissleFire = false;
			StartMissleAimTimer();
		}
		
	}
}

/** Getter for Slip Value **/
FVector ABL_JetBase::GetSlipValue() const
{
	const FVector SlipValue = UKismetMathLibrary::GreaterGreater_VectorRotator(
			Body->GetPhysicsLinearVelocity(),
			UKismetMathLibrary::NegateRotator(Body->GetComponentRotation())) / 2000.0f;
			 
	return SlipValue;
}

//Camera Control
void ABL_JetBase::PilotCameraControl(float AxisYaw, float AxisPitch)
{
	if (IsValid(PilotsCamera))
	{
		if(PilotsCamera->IsActive()==true && RightMousePressed==true)
		{
			//Look Left / Right / Up / Down
			const float Pitch = FMath::Clamp(PilotsCamera->GetRelativeRotation().Pitch + (AxisPitch * -1.5f), PilotCameraPitchMin, PilotCameraPitchMax);
			const float Yaw = FMath::Clamp(PilotsCamera->GetRelativeRotation().Yaw + (AxisYaw * 1.5f), PilotCameraYawMin, PilotCameraYawMax);
			PilotsCamera->SetRelativeRotation(FRotator(Pitch, Yaw, 0.0f));
		}
		else
		{
			const FRotator CameraRot = UKismetMathLibrary::RInterpTo(PilotsCamera->GetRelativeRotation(), FRotator(0.0f, 0.0f, 0.0f), DeltaSec, 10.0f);
			PilotsCamera->SetRelativeRotation(CameraRot);
		}
	}
	
}
void ABL_JetBase::CameraZoom(float MouseWheelAxis)
{
    //Zoom
    if (bisDead==false)
    {
    	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength + MouseWheelAxis * -100, ZoomMin, ZoomMax);
    }
}

//Health and Damage
void ABL_JetBase::SetCurrentHealth(float healthValue)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
        OnHealthUpdate();
    }
}
float ABL_JetBase::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float damageApplied = CurrentHealth - DamageTaken;
    SetCurrentHealth(damageApplied);
    return damageApplied;
}

// Called when the game starts or when spawned
void ABL_JetBase::BeginPlay()
{
	Super::BeginPlay();
	EngineState = true;
}



void ABL_JetBase::OnHealthUpdate()
{
    //Client-specific functionality
    if (IsLocallyControlled())
    {
        FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

        if (CurrentHealth <= 0)
        {
            FString deathMessage = FString::Printf(TEXT("You have been killed."));
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, deathMessage);
            //Destroy(false, true);
            //K2_DestroyActor();
            Death();
        }
    }

    //Server-specific functionality
    if (GetLocalRole() == ROLE_Authority)
    {
        FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
    }

    //Functions that occur on all machines. 
    /*
        Any special functionality that should occur as a result of damage or death should be placed here.
    */
}

//Firing
void ABL_JetBase::StartMissleAimTimer()
{
	if (bDoOnceMissleFire)
	{
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(MissleAimigTimer, this, &ABL_JetBase::CanFireMissle, 1.5f, false);
		//bDoOnceMissleFire = false;
	}
}
void ABL_JetBase::CanFireMissle()
{
	
	if (bCrosshairTraceDetected)
	{
		bCanFireMissle = true;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "Target detected");
	}
	else
	{
		bCanFireMissle = false;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Target lost");
	}
}

void ABL_JetBase::StartFire()
{
    if (!bIsFiringWeapon)
    {
        bIsFiringWeapon = true;
        UWorld* World = GetWorld();
    	float FireRate(0.0f);
    	
    	switch (WeaponState)
    	{
    	case EWeaponList::MachineGun: FireRate = MachineGunFireRate;
    		break;
    	case EWeaponList::AirAirMissle: FireRate = AirAirFireRate;
    		break;
    	case EWeaponList::AirGroundMissle: FireRate = AirGroundFireRate;
    		break;
    	case EWeaponList::Nurs: FireRate = NursFireRate;
    		break;
    			
    		default: MachineGunFireRate = 0.1f;
    	}
    	
    	
        World->GetTimerManager().SetTimer(FiringTimer, this, &ABL_JetBase::StopFire, FireRate, false);
        HandleFire();
    }
}
void ABL_JetBase::StopFire()
{
    bIsFiringWeapon = false;
}
void ABL_JetBase::HandleFire_Implementation()
{
    /** FVector SpawnLocation = MachineGun_Muzzle->GetComponentLocation(); // +(GetControlRotation().Vector() * 1.0f) + (GetActorUpVector() * 1.0f);
    FRotator SpawnRotation = MachineGun_Muzzle->GetComponentRotation();
    //FVector SpawnScale;
    //const FTransform SpawnTransform(SpawnRotation, SpawnLocation, SpawnScale);
    
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.Instigator = GetInstigator();
    SpawnParameters.Owner = this;
    //spawnParameters.Template = BoatProjectile;
    UWorld* World = GetWorld();
    if(World)
    {
        World->SpawnActor<ABL_ProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParameters);
    }
    
    //ABL_ProjectileBase* spawnedProjectile = GetWorld()->SpawnActor<ABL_ProjectileBase>(spawnLocation, spawnRotation, spawnParameters); **/

	switch (WeaponState)
	{
		case EWeaponList::MachineGun: FireMachineGun_Implementation();
			break;
		case EWeaponList::AirAirMissle: if (bCanFireMissle && IsValid(MissleAimHitActor))
			{
			FireAirAirMissle_Implementation(CrosshairHitActor);
			}
			break;
		case EWeaponList::AirGroundMissle: if (bCanFireMissle && IsValid(MissleAimHitActor))
		{
			FireAirGroundMissle_Implementation(CrosshairHitActor);
		}
			break;
		case EWeaponList::Nurs: FireNurs_Implementation();
			break;
		
		default: FireMachineGun_Implementation();
	}
}

//Weapon
void ABL_JetBase::FireMachineGun_Implementation()
{
	if (bisDead==false)
	{
		FHitResult WidgetTraceHit;
		
		FVector Start = MachineGun_Muzzle->GetComponentLocation();
		FVector End = Start + MachineGun_Muzzle->GetForwardVector()*100000.0f;
		FCollisionQueryParams CollisionParam;
		CollisionParam.AddIgnoredActor(this);
		//Line Trace
		bool BHit = GetWorld()->LineTraceSingleByChannel(WidgetTraceHit, Start, End, ECollisionChannel::ECC_Visibility, CollisionParam);

		//Select hit location
		if (BHit == true) 
		{
			CameraTraceHitLoc = WidgetTraceHit.Location;
		}
		else
		{
			CameraTraceHitLoc = WidgetTraceHit.TraceEnd;
		}
	}
	//Draw Line Trace
	// DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
}
void ABL_JetBase::FireAirAirMissle_Implementation(AActor* TargetActor)
{
	FVector SpawnLocation = AirAirMuzzle->GetComponentLocation(); // +(GetControlRotation().Vector() * 1.0f) + (GetActorUpVector() * 1.0f);
	FRotator SpawnRotation = AirAirMuzzle->GetComponentRotation();
	//FVector SpawnScale(1.0f, 1.0f, 1.0f);
	//const FTransform SpawnTransform(SpawnRotation, SpawnLocation, SpawnScale);
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = GetInstigator();
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	UWorld* World = GetWorld();
	if(World)
	{
		ABL_ProjectileBase* SpawnedProjectile = World->SpawnActor<ABL_ProjectileBase>(AirAirProjectileClass, SpawnLocation, SpawnRotation, SpawnParameters);
		if (SpawnedProjectile->IsHoming)
		{
			SpawnedProjectile->MoveToTarget(TargetActor);
		}
		//World->SpawnActor<ABL_ProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParameters);
	}
}
void ABL_JetBase::FireAirGroundMissle_Implementation(AActor* TargetActor)
{
	FVector SpawnLocation = AirGroundMuzzle->GetComponentLocation(); // +(GetControlRotation().Vector() * 1.0f) + (GetActorUpVector() * 1.0f);
	FRotator SpawnRotation = AirGroundMuzzle->GetComponentRotation();
	//FVector SpawnScale(1.0f, 1.0f, 1.0f);
	//const FTransform SpawnTransform(SpawnRotation, SpawnLocation, SpawnScale);
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = GetInstigator();
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	UWorld* World = GetWorld();
	if(World)
	{
		ABL_ProjectileBase* SpawnedProjectile = World->SpawnActor<ABL_ProjectileBase>(AirGroundProjectileClass, SpawnLocation, SpawnRotation, SpawnParameters);
		if (SpawnedProjectile->IsHoming)
		{
			SpawnedProjectile->MoveToTarget(TargetActor);
		}
		//World->SpawnActor<ABL_ProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParameters);
	}
}
void ABL_JetBase::FireNurs_Implementation()
{
	
}


void ABL_JetBase::Death_Implementation()
{
	Destroy();
}

// Called every frame
void ABL_JetBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DeltaSec = DeltaTime;
	if (EngineState==true && bisDead==false)
	{
		if (GetInputAxisValue("Shoot")>0.0f)
		{
			StartFire();
		}
		//const float RollValue = RollFilter->Value = RollFilter->GetValue(GetInputAxisValue("LookYaw"), DeltaTime);
		//GetPitchValue(GetInputAxisValue("LookPitch"));
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Purple, "Torque: " + FString::SanitizeFloat(CurrentTorque, 2));
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Purple, "Height: " + FString::SanitizeFloat(CurrentHeight, 2));
		GetHeight();
		ApplyAirForce();
		LiftForce();
		SetCurrentTorque(GetInputAxisValue("MoveForward"));
		MoveForward("None", true);
		
		MoveRight(GetInputAxisValue("MoveRight"));
		RollControl(GetInputAxisValue("LookYaw"));
		PitchControl(GetInputAxisValue("LookPitch"));
		PilotCameraControl(GetInputAxisValue("LookYaw"), GetInputAxisValue("LookPitch"));
		SetEngineSoundParameter();
		CrosshairDetectTrace();
	}
}

// Called to bind functionality to input
void ABL_JetBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent->BindAction("F", IE_Pressed, this, &ABL_JetBase::ChangeWeapon);
	PlayerInputComponent->BindAxis("MoveForward"); //this, &ABL_JetBase::SetCurrentTorque
	PlayerInputComponent->BindAxis("MoveRight");
	PlayerInputComponent->BindAxis("LookYaw");
	PlayerInputComponent->BindAxis("LookPitch");
	PlayerInputComponent->BindAxis("Shoot");
}

//Event Possesed override
void ABL_JetBase::PossessedBy(AController* NewController)
{
	EngineState = true;
	EngineSound->Activate();
	EngineSound->SetVolumeMultiplier(1.0f);
	MachineGunSound->Activate();
}
// Event Unpossesed override
void ABL_JetBase::UnPossessed()
{
	EngineState = false;
}



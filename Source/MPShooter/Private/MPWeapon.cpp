
#include "MPWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "MPShooter.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AMPWeapon::AMPWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleFlashSocket";
	TracerTargetName = "Target";

	SetReplicates(true);
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;
}

// Called when the game starts or when spawned
void AMPWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void AMPWeapon::Fire()
{
	if (Role < ROLE_Authority) {
		ServerFire();
	}

	AActor* Owner = GetOwner();
	if (Owner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector TraceEnd = EyeLocation + EyeRotation.Vector() * 10000;
		FVector ShotDirection = EyeRotation.Vector();
		FVector TracerEndPoint = TraceEnd;

		FCollisionQueryParams QParams;
		QParams.AddIgnoredActor(Owner);
		QParams.AddIgnoredActor(this);
		QParams.bTraceComplex = true;
		QParams.bReturnPhysicalMaterial = true;

		FHitResult Hit;
		EPhysicalSurface SurfaceType = SurfaceType_Default;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QParams)) {
			//Was hit
			TracerEndPoint = Hit.ImpactPoint;

			AActor* HitActor = Hit.GetActor();

			SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			float ActualDamage = WeaponDamage;
			if (SurfaceType == SURFACE_FLESHVULNERABLE) {
				ActualDamage *= VulnerableMultiplier;
			}
			UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, Owner->GetInstigatorController(), this, DamageType);
			PlayImpactEffects(SurfaceType, Hit.ImpactPoint);


		}

		//DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.f, 0, 1.f);
		PlayFireEffects(TracerEndPoint);

		if (Role == ROLE_Authority)
		{
			HitScanTrace.TraceTo = TracerEndPoint;
			HitScanTrace.SurfaceType = SurfaceType;
			UE_LOG(LogTemp, Warning, TEXT("%d 2nd type"), SurfaceType)
		}

		LastFiredTime = GetWorld()->TimeSeconds;
	}
}

void AMPWeapon::OnRep_HitScanTrace()
{
	PlayFireEffects(HitScanTrace.TraceTo);
	PlayImpactEffects(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);

}

void AMPWeapon::ServerFire_Implementation()
{
	Fire();
}

bool AMPWeapon::ServerFire_Validate()
{
	return true;
}

void AMPWeapon::StartFire()
{
	float FirstDelay = FMath::Max(LastFiredTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.f);

	GetWorldTimerManager().SetTimer(TimerHandleTimeBetweenShots, this, &AMPWeapon::Fire, TimeBetweenShots, true, FirstDelay);

}

void AMPWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandleTimeBetweenShots);
}

// Called every frame
void AMPWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMPWeapon::PlayFireEffects(FVector TracerEndPoint) {
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}

	if (TracerEffect) {
		FVector MuzzleLocation = MeshComp->GetSocketLocation("MuzzleFlashSocket");
		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComp) {
			TracerComp->SetVectorParameter(TracerTargetName, TracerEndPoint);
		}
	}

	APawn* Owner = Cast<APawn>(GetOwner());
	if (Owner) {
		APlayerController* PC = Cast<APlayerController>(Owner->GetController());
		if (PC)
		{
			PC->ClientPlayCameraShake(FireCamShake);
		}
	}
}

void AMPWeapon::PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint)
{
	UParticleSystem* SelectedEffect = nullptr;
	switch (SurfaceType)
	{
	case SurfaceType_Default:
		SelectedEffect = DefaultImpactEffect;
		break;

	case SURFACE_FLESHDEFAULT:
	case SURFACE_FLESHVULNERABLE:
		SelectedEffect = FleshImpactEffect;

		break;

	default:
		break;
	}

	if (SelectedEffect) {
		FVector ShotDirectionNormal = MeshComp->GetSocketLocation(MuzzleSocketName) - ImpactPoint;
		ShotDirectionNormal.Normalize();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, ImpactPoint, ShotDirectionNormal.Rotation());
	}
}

void AMPWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//
	DOREPLIFETIME_CONDITION(AMPWeapon, HitScanTrace, COND_SkipOwner);
	DOREPLIFETIME(AMPWeapon, WeaponDamage);

}


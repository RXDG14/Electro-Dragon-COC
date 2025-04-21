#include "ElectroChain.h"

#include "Cannon.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AElectroChain::AElectroChain()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereCollider");
	SphereComponent->SetupAttachment(GetRootComponent());

	HitEffect = CreateDefaultSubobject<UNiagaraComponent>("HitEffect");
	HitEffect->SetupAttachment(SphereComponent);
	
	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComponent");
}

void AElectroChain::BeginPlay()
{
	Super::BeginPlay();
	CurrentDamage = DamageAmount;
	CurrentChainNumber = 1;
	AlreadyHitActors.Add(this);
	CheckForNextTarget();
}

void AElectroChain::CheckForNextTarget()
{
	// GetClosestTarget();
	// MoveTowardsTarget(); using projectileComponent
	CurrentTarget = GetClosestTarget(GetActorLocation());
	if (CurrentTarget)
	{
		MoveTowardsTarget();
	}
	else
	{
		GetWorldTimerManager().SetTimer(DestroyChainHandle, this, &AElectroChain::DestroyChain, 1.f, true);
		ProjectileComponent->StopMovementImmediately();
		//Destroy(); // End of chain
	}
}

AActor* AElectroChain::GetClosestTarget(const FVector& FromLocation) const
{
	TArray<AActor*> PotentialTargets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACannon::StaticClass(), PotentialTargets);

	AActor* ClosestTarget = nullptr;
	float ClosestDistance = ChainRange;

	for (AActor* Actor : PotentialTargets)
	{
		if (!Actor || AlreadyHitActors.Contains(Actor) || Actor == this)
			continue;

		float Distance = FVector::Dist(FromLocation, Actor->GetActorLocation());
		if (Distance < ClosestDistance)
		{
			ClosestTarget = Actor;
			ClosestDistance = Distance;
		}
	}

	return ClosestTarget;
}

void AElectroChain::MoveTowardsTarget()
{
	// move towards the target using projectileComponent
	// when reached-> apply damage
	if (!CurrentTarget) return;

	FVector Direction = (CurrentTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	ProjectileComponent->Velocity = Direction * ProjectileComponent->InitialSpeed;

	GetWorldTimerManager().SetTimer(TargetCheckHandle, this, &AElectroChain::OnHitTarget, 0.01f, true);
}

void AElectroChain::OnHitTarget()
{
	if (!CurrentTarget) return;

	float Distance = FVector::Dist(GetActorLocation(), CurrentTarget->GetActorLocation());
	if (Distance <= TargetReachedDistance)
	{
		GetWorldTimerManager().ClearTimer(TargetCheckHandle);
		ApplyDamage(CurrentTarget);
		HitEffect->Activate(true);
	}
}

void AElectroChain::ApplyDamage(AActor* Target)
{
	// apply damage to the target
	//  CheckForNextTarget();
	if (!Target) return;

	UGameplayStatics::ApplyDamage(Target, CurrentDamage, GetInstigatorController(), this, nullptr);
	AlreadyHitActors.Add(Target);
	CurrentDamage = FMath::Max(CurrentDamage - 50.f, 0.f);
	CurrentChainNumber++;
	if (CurrentChainNumber > MaxChains)
	{
		GetWorldTimerManager().SetTimer(DestroyChainHandle, this, &AElectroChain::DestroyChain, 1.f, true);
		ProjectileComponent->StopMovementImmediately();
	}
	else
	{
		CheckForNextTarget();
	}
}

void AElectroChain::DestroyChain()
{
	Destroy();
}


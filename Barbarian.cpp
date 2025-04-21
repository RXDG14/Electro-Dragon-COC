#include "Barbarian.h"
#include "AIController.h"
#include "AttributesComponent.h"
#include "Cannon.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"

ABarbarian::ABarbarian()
{
	PrimaryActorTick.bCanEverTick = true;
	
	DetectionRadius = CreateDefaultSubobject<USphereComponent>("Detection Radius");
	DetectionRadius->SetupAttachment(GetCapsuleComponent());
	DetectionRadius->OnComponentBeginOverlap.AddDynamic(this, &ABarbarian::OnTargetEnter);
	DetectionRadius->OnComponentEndOverlap.AddDynamic(this, &ABarbarian::OnTargetExit);
	
	WeaponCollider = CreateDefaultSubobject<USphereComponent>("Weapon Collider");
	WeaponCollider->SetupAttachment(GetMesh(), "WeaponSocket");

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon Mesh");
	WeaponMesh->SetupAttachment(GetMesh(), "RightHandSocket");
	
	Attributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes"));

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>("Health Bar Widget");
	HealthBarWidget->SetupAttachment(GetRootComponent());
	
	CurrentTarget = nullptr;
}

void ABarbarian::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABarbarian::BeginPlay()
{
	Super::BeginPlay();
	//WeaponMesh->SetupAttachment(GetMesh(), "RightHandSocket");
	WeaponCollider->OnComponentBeginOverlap.AddDynamic(this,&ABarbarian::OnWeaponOverlap);
}

void ABarbarian::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentTarget && IsValid(CurrentTarget))
	{
		const float DistanceToTarget = FVector::Dist(GetActorLocation(), CurrentTarget->GetActorLocation());
		//UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceToTarget);
		if (DistanceToTarget <= 320.f)
		{
			Attack();
			//UE_LOG(LogTemp, Warning, TEXT("attack"));
		}
		else
		{
			MoveToTarget();
			//UE_LOG(LogTemp, Warning, TEXT("moving to target"));
		}
	}
	else
	{
		SearchForTarget();
		//UE_LOG(LogTemp, Warning, TEXT("searching for target"));
	}
}

void ABarbarian::OnTargetEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ABarbarian::OnTargetExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ABarbarian::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto MyOwner = GetOwner();
	if (!MyOwner) return;

	auto MyOwnerInstigator =  MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();
	
	if (OtherActor && OtherActor != this && OtherActor!= MyOwner && OtherActor->ActorHasTag("Cannon"))
	{
		if (OtherComp && OtherComp->GetName().Contains("Cannon Top Mesh"))
		{
			if (WeaponOverlappedActors.Contains(OtherActor))
			{
				return;
			}

			WeaponOverlappedActors.AddUnique(OtherActor);
			//UE_LOG(LogTemp, Warning, TEXT("Weapon overlapped with CannonTopMesh"));
			UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, MyOwnerInstigator, this, DamageTypeClass);
		}
	}
}

void ABarbarian::IncreaseDetectionRadius()
{
	if (DetectionRadius)
	{
		DetectionRadius->SetSphereRadius(1500.0f, true); // 'true' updates the collision immediately
		//UE_LOG(LogTemp, Warning, TEXT("Detection radius increased to 1500"));
	}
}

void ABarbarian::SearchForTarget()
{
	TargetsArray.Empty();

	TArray<AActor*> FoundTargets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACannon::StaticClass(), FoundTargets);

	float ClosestDistanceSq = FMath::Square(DetectionRadius->GetScaledSphereRadius());
	AActor* ClosestTarget = nullptr;

	for (AActor* Target : FoundTargets)
	{
		if (!IsValid(Target)) continue;

		float DistanceSq = FVector::DistSquared(GetActorLocation(), Target->GetActorLocation());

		if (DistanceSq <= ClosestDistanceSq)
		{
			TargetsArray.Add(Target);

			if (!ClosestTarget || DistanceSq < FVector::DistSquared(GetActorLocation(), ClosestTarget->GetActorLocation()))
			{
				ClosestTarget = Target;
			}
		}
	}

	CurrentTarget = ClosestTarget;
}

void ABarbarian::MoveToTarget()
{
	if (CurrentTarget == nullptr) return;

	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController) return;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(CurrentTarget);
	MoveRequest.SetAcceptanceRadius(250.f);

	FNavPathSharedPtr NavPath;
	AIController->MoveTo(MoveRequest,&NavPath);
}

void ABarbarian::Attack()
{
	if (IsAttacking) return;

	IsAttacking = true;
	PlayAttackMontage();	
}

void ABarbarian::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection("Attack2",AttackMontage);
		/*int32 Selection = FMath::RandRange(0,1);
		if (Selection == 0)
		{
			AnimInstance->Montage_JumpToSection("Attack2",AttackMontage);	
		}
		else
		{
			AnimInstance->Montage_JumpToSection("Attack1",AttackMontage);
		}*/
	}
	//UE_LOG(LogTemp, Warning, TEXT("attack montage code end"));
}

void ABarbarian::AttackEnd()
{
	IsAttacking = false;
	WeaponOverlappedActors.Empty();
}

void ABarbarian::EnableWeaponCollision()
{
	WeaponCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABarbarian::DisableWeaponCollision()
{
	WeaponCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
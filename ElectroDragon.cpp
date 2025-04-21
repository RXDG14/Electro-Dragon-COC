#include "ElectroDragon.h"

#include "AIController.h"
#include "AttributesComponent.h"
#include "Cannon.h"
#include "ElectroChain.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"


AElectroDragon::AElectroDragon()
{
	PrimaryActorTick.bCanEverTick = true;
	
	DetectionRadius = CreateDefaultSubobject<USphereComponent>("Detection Radius");
	DetectionRadius->SetupAttachment(GetCapsuleComponent());

	ElectroChainSpawnLocation = CreateDefaultSubobject<USceneComponent>("Electro Chain Spawn Location");
	ElectroChainSpawnLocation->SetupAttachment(GetMesh(),"ElectroChainLocation");
	
	Attributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes"));

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>("Health Bar Widget");
	HealthBarWidget->SetupAttachment(GetRootComponent());
}

void AElectroDragon::BeginPlay()
{
	Super::BeginPlay();
}

void AElectroDragon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentTarget && IsValid(CurrentTarget))
    	{
    		//const float DistanceToTarget = FVector::Dist(GetActorLocation(), CurrentTarget->GetActorLocation());
			const float DistanceToTarget = FVector::Dist2D(GetActorLocation(), CurrentTarget->GetActorLocation());
			UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceToTarget);
    		if (DistanceToTarget <= 500.f)
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

void AElectroDragon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AElectroDragon::SearchForTarget()
{
	TargetsArray.Empty();

	TArray<AActor*> FoundTargets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACannon::StaticClass(), FoundTargets); // only for prototype purpose

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

void AElectroDragon::MoveToTarget()
{
	if (CurrentTarget == nullptr) return;

	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController) return;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(CurrentTarget);
	MoveRequest.SetAcceptanceRadius(350.f);

	FNavPathSharedPtr NavPath;
	AIController->MoveTo(MoveRequest,&NavPath);
}

void AElectroDragon::Attack()
{
	if (IsAttacking) return;

	IsAttacking = true;
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AElectroDragon::SpawnElectroChain, 3.f, false);
}

void AElectroDragon::SpawnElectroChain()
{
	FVector Location = ElectroChainSpawnLocation->GetComponentLocation();
	FRotator Rotation = ElectroChainSpawnLocation->GetComponentRotation();
	auto ElectroChain = GetWorld()->SpawnActor<AElectroChain>(ElectroChainClass, Location, Rotation);
	ElectroChain->SetOwner(this);
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AElectroDragon::ResetAttack, 1.0f, false);
}

void AElectroDragon::ResetAttack()
{
	IsAttacking = false;
}
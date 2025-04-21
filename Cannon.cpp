#include "Cannon.h"
#include "AttributesComponent.h"
#include "Barbarian.h"
#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	TurretPivot = CreateDefaultSubobject<USceneComponent>("TurretPivot");
	TurretPivot->SetupAttachment(Root);
	CannonTopMesh = CreateDefaultSubobject<UStaticMeshComponent>("Cannon Top Mesh");
	CannonTopMesh->SetupAttachment(TurretPivot);
	BulletSpawnPoint = CreateDefaultSubobject<USceneComponent>("BulletSpawnPoint");
	BulletSpawnPoint->SetupAttachment(CannonTopMesh);
	CannonKnockBackLocation = CreateDefaultSubobject<USceneComponent>("CannonKnockBackLocation");
	CannonKnockBackLocation->SetupAttachment(TurretPivot);
	ShootVFXLocation = CreateDefaultSubobject<USceneComponent>("ShootVFXLocation");
	ShootVFXLocation->SetupAttachment(TurretPivot);
	CannonMuzzleEffect = CreateDefaultSubobject<UNiagaraComponent>("CannonMuzzleEffect");
	CannonMuzzleEffect->SetupAttachment(ShootVFXLocation);
	
	
	CannonBottomMesh1 = CreateDefaultSubobject<UStaticMeshComponent>("Cannon Bottom Mesh 1");
	CannonBottomMesh1->SetupAttachment(Root);
	CannonBottomMesh2 = CreateDefaultSubobject<UStaticMeshComponent>("Cannon Bottom Mesh 2");
	CannonBottomMesh2->SetupAttachment(Root);
	CannonBottomMesh3 = CreateDefaultSubobject<UStaticMeshComponent>("Cannon Bottom Mesh 3");
	CannonBottomMesh3->SetupAttachment(Root);
	CannonBottomMesh4 = CreateDefaultSubobject<UStaticMeshComponent>("Cannon Bottom Mesh 4");
	CannonBottomMesh4->SetupAttachment(Root);
	CannonBottomMesh5 = CreateDefaultSubobject<UStaticMeshComponent>("Cannon Bottom Mesh 5");
	CannonBottomMesh5->SetupAttachment(Root);
	
	DetectionRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Detection Radius"));
	DetectionRadius->SetupAttachment(Root);
	DetectionRadius->OnComponentBeginOverlap.AddDynamic(this, &ACannon::OnEnemyEnter);
	DetectionRadius->OnComponentEndOverlap.AddDynamic(this, &ACannon::OnEnemyExit);
	
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>("Health Bar Widget");
	HealthBarWidget->SetupAttachment(GetRootComponent());

	Attributes = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes"));

}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
}

void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentTarget && IsValid(CurrentTarget)) // has a target
	{
		RotateTowardsTarget(DeltaTime);
		if (CanFire)
		{
			SpawnBullet();
			CannonKnockBackEffect();
		}
	}
	else // does not have a target
	{
		SearchForTarget();
	}
	
}

void ACannon::OnEnemyEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if enemy->add to EnemiesArray
	if (OtherActor && OtherActor != this && !EnemiesArray.Contains(OtherActor))
	{
		EnemiesArray.AddUnique(OtherActor);
	}
}

void ACannon::OnEnemyExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// if enemy->remove from EnemiesArray
	if (OtherActor && OtherActor != this && EnemiesArray.Contains(OtherActor))
	{
		EnemiesArray.Remove(OtherActor);

		if (OtherActor == CurrentTarget)
		{
			CurrentTarget = nullptr;
		}
	}
}

void ACannon::SearchForTarget()
{
	// get all the actors in the world currently of class Barbarian in the world and add them to EnemiesArray.
	// then see which actor is inside DetectionRadius sphere, make that the CurrentTarget
	EnemiesArray.Empty();

	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABarbarian::StaticClass(), FoundEnemies);

	float ClosestDistanceSq = DetectionRadius->GetScaledSphereRadius() * DetectionRadius->GetScaledSphereRadius();
	AActor* ClosestEnemy = nullptr;

	for (AActor* Enemy : FoundEnemies)
	{
		if (!IsValid(Enemy)) continue;

		float DistanceSq = FVector::DistSquared(GetActorLocation(), Enemy->GetActorLocation());

		if (DistanceSq <= ClosestDistanceSq)
		{
			EnemiesArray.Add(Enemy);

			if (!ClosestEnemy || DistanceSq < FVector::DistSquared(GetActorLocation(), ClosestEnemy->GetActorLocation()))
			{
				ClosestEnemy = Enemy;
			}
		}
	}

	CurrentTarget = ClosestEnemy;
}

void ACannon::RotateTowardsTarget(float DeltaTime)
{
	// get CurrentTarget and Rotate Towards it
	if (!CurrentTarget) return;

	FVector Direction = CurrentTarget->GetActorLocation() - TurretPivot->GetComponentLocation();
	FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	FRotator CurrentRotation = TurretPivot->GetComponentRotation();

	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.f);
	TurretPivot->SetWorldRotation(FRotator(0.f, NewRotation.Yaw, 0.f));
}

void ACannon::SpawnBullet()
{
	if (!CanFire || !BulletClass) return;

	FVector Location = BulletSpawnPoint->GetComponentLocation();
	FRotator Rotation = BulletSpawnPoint->GetComponentRotation();
	auto SpawnedBullet = GetWorld()->SpawnActor<ABullet>(BulletClass, Location, Rotation);
	SpawnedBullet->SetOwner(this);	
	CannonMuzzleEffect->Activate(true);
	CannonKnockBackEffect();
	
	CanFire = false;
	GetWorld()->GetTimerManager().SetTimer(FireBulletTimer, this, &ACannon::ResetBulletFireTimer, 0.8f, false);
}

void ACannon::CannonKnockBackEffect()
{
	OriginalLocation = CannonTopMesh->GetRelativeLocation();
	CannonTopMesh->SetRelativeLocation(CannonKnockBackLocation->GetRelativeLocation());
	
	GetWorld()->GetTimerManager().SetTimer(
		KnockBackTimerHandle,
		this,
		&ACannon::ResetKnockBackEffect,
		0.1f,
		false
	);
}

void ACannon::ResetKnockBackEffect()
{
	if (CannonTopMesh)
	{
		CannonTopMesh->SetRelativeLocation(OriginalLocation);
	}
}

void ACannon::ResetBulletFireTimer()
{
	CanFire = true;
}




#include "Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
	GetWorld()->GetTimerManager().SetTimer(BulletAutoDestroyTimer, this, &ABullet::DestroyBullet, 3.f, false);
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//DrawDebugSphere(GetWorld(),Mesh->GetComponentLocation(),20.f,12,FColor::Red);
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	auto MyOwner = GetOwner();
	if (!MyOwner) return;

	auto MyOwnerInstigator =  MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();
	
	if (OtherActor && OtherActor != this && OtherActor!= MyOwner && OtherActor->ActorHasTag("Barbarian"))
	{
		UGameplayStatics::ApplyDamage(OtherActor,DamageAmount,MyOwnerInstigator,this,DamageTypeClass);
		DestroyBullet();
	}
}

void ABullet::DestroyBullet()
{
	Destroy();	
}
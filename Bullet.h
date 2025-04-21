#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class COC_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(VisibleAnywhere)
	//USphereComponent* SphereComponent;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileComponent;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float DamageAmount = 50.f;

private:
	FTimerHandle BulletAutoDestroyTimer;
	void DestroyBullet();

	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, FVector NormalImpulse,const FHitResult& Hit);
};

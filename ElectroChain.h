#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElectroChain.generated.h"

class UNiagaraComponent;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class COC_API AElectroChain : public AActor
{
	GENERATED_BODY()
	
public:	
	AElectroChain();
	//virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;
	
	UPROPERTY(VisibleAnywhere, Category="Movement")
	UProjectileMovementComponent* ProjectileComponent;

	UPROPERTY(EditDefaultsOnly, Category="Chain")
	float ChainRange = 500.f;

	UPROPERTY(EditDefaultsOnly, Category="Chain")
	int32 MaxChains = 5;

	UPROPERTY()
	float CurrentDamage;
	
	UPROPERTY()
	float CurrentChainNumber;
	
	UPROPERTY(EditDefaultsOnly, Category="Chain")
	float DamageAmount = 50.f;

	UPROPERTY(EditDefaultsOnly)
	float TargetReachedDistance = 100.f;

	UPROPERTY(EditAnywhere, Category="Chain")
	UNiagaraComponent* HitEffect;

private:
	UPROPERTY()
	TArray<AActor*> AlreadyHitActors;

	UPROPERTY()
	AActor* CurrentTarget = nullptr;

	FTimerHandle TargetCheckHandle;
	FTimerHandle DestroyChainHandle;

	void CheckForNextTarget();
	AActor* GetClosestTarget(const FVector& FromLocation) const;
	void MoveTowardsTarget();
	void OnHitTarget();
	void ApplyDamage(AActor* Target);
	void DestroyChain();

	
};

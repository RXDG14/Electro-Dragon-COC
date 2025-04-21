#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Barbarian.generated.h"

class UWidgetComponent;
class UHealthBar;
class UAttributesComponent;
class USphereComponent;

UCLASS()
class COC_API ABarbarian : public ACharacter
{
	GENERATED_BODY()

public:	
	ABarbarian();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* HealthBarWidget;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float DamageAmount = 20.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Detection")
	USphereComponent* DetectionRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* WeaponMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USphereComponent* WeaponCollider;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Detection")
	TArray<AActor*> TargetsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AttackMontage;
	
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	
	UFUNCTION(BlueprintCallable)
	void EnableWeaponCollision();

	UFUNCTION(BlueprintCallable)
	void DisableWeaponCollision();

private:	
	FTimerHandle RadiusIncreaseTimerHandle;
	bool IsAttacking = false;
	bool HasHitCannonDuringThisAttack = false;
	void IncreaseDetectionRadius();
	void SearchForTarget();
	void MoveToTarget();
	void PlayAttackMontage();
	
	TArray<AActor*> WeaponOverlappedActors;

	UPROPERTY()
	AActor* CurrentTarget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAttributesComponent* Attributes;
	
	UFUNCTION()
	void Attack();
	
	UFUNCTION()
	void OnTargetEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTargetExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ElectroDragon.generated.h"

class AElectroChain;
class UWidgetComponent;
class UAttributesComponent;
class USphereComponent;

UCLASS()
class COC_API AElectroDragon : public ACharacter
{
	GENERATED_BODY()

public:
	AElectroDragon();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsAttacking = false;
	
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* HealthBarWidget;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Detection")
	USphereComponent* DetectionRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Detection")
	TArray<AActor*> TargetsArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USceneComponent* ElectroChainSpawnLocation;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<AElectroChain> ElectroChainClass;
	
	UPROPERTY(EditAnywhere)
	float DamageAmount = 20.f;

private:
	FTimerHandle AttackTimerHandle;
	void SearchForTarget();
	void MoveToTarget();
	void Attack();
	void SpawnElectroChain();
	void ResetAttack();

	UPROPERTY()
	AActor* CurrentTarget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAttributesComponent* Attributes;
	
};

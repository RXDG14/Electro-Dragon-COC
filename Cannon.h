#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "Cannon.generated.h"

class UWidgetComponent;
class UAttributesComponent;
class ABullet;
class ABarbarian;
class USphereComponent;

UCLASS()
class COC_API ACannon : public AActor
{
	GENERATED_BODY()

public:
	ACannon();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* HealthBarWidget;
	
	UPROPERTY(Editanywhere, Category="Cannon Mesh")
	USphereComponent* DetectionRadius;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cannon Mesh")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, Category="Cannon Mesh")
	USceneComponent* TurretPivot;
	
	UPROPERTY(VisibleAnywhere, Category="Cannon Mesh")
	UStaticMeshComponent* CannonTopMesh;
	
	UPROPERTY(VisibleAnywhere, Category="Cannon Mesh")
	UStaticMeshComponent* CannonBottomMesh1;

	UPROPERTY(VisibleAnywhere, Category="Cannon Mesh")
	UStaticMeshComponent* CannonBottomMesh2;

	UPROPERTY(VisibleAnywhere, Category="Cannon Mesh")
	UStaticMeshComponent* CannonBottomMesh3;

	UPROPERTY(VisibleAnywhere, Category="Cannon Mesh")
	UStaticMeshComponent* CannonBottomMesh4;

	UPROPERTY(VisibleAnywhere, Category="Cannon Mesh")
	UStaticMeshComponent* CannonBottomMesh5;

	UPROPERTY(EditDefaultsOnly, Category="Cannon")
	TSubclassOf<ABullet> BulletClass;

	UPROPERTY(EditAnywhere, Category="Cannon")
	UNiagaraComponent* CannonMuzzleEffect;
	
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* BulletSpawnPoint;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* CannonKnockBackLocation;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* ShootVFXLocation;
	
	UPROPERTY(VisibleAnywhere, Category="Cannon")
	TArray<AActor*> EnemiesArray;

	UPROPERTY(EditAnywhere)
	UAttributesComponent* Attributes;
	
private:
	UFUNCTION()
	void OnEnemyEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEnemyExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY()
	AActor* CurrentTarget = nullptr;
	
	void RotateTowardsTarget(float DeltaTime);
	void SpawnBullet();
	void ResetBulletFireTimer();
	void OnAimedAtTarget();
	void SearchForTarget();
	void CannonKnockBackEffect();
	void ResetKnockBackEffect();
	
	FTimerHandle FireBulletTimer;
	FTimerHandle KnockBackTimerHandle;
	FVector OriginalLocation;
	bool CanFire = true;
};

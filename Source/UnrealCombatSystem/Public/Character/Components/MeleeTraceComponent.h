#pragma once
#include "Kismet/KismetSystemLibrary.h"

UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class UMeleeTraceComponent: public UActorComponent
{
	GENERATED_BODY()
public:
	UMeleeTraceComponent();

	UPROPERTY(EditAnywhere, Category="Trace")
	FName StartSocket;
	UPROPERTY(EditAnywhere, Category="Trace")
	FName EndSocket;
	UPROPERTY(EditAnywhere, Category="Trace")
	float Radius = 20.f;
	UPROPERTY(EditAnywhere, Category="Trace")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	UPROPERTY(EditAnywhere, Category="Trace")
	EDrawDebugTrace::Type DebugTrace = EDrawDebugTrace::None;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTraceHit, AActor*, HitActor, const FHitResult&, HitResult);

	UPROPERTY(BlueprintAssignable, Category="Trace")
	FOnTraceHit OnTraceHit;

	void SetTraceMesh(USceneComponent* InMesh);
	void StartHitTrace();
	void StopHitTrace();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TWeakObjectPtr<USceneComponent> TraceMesh;
	TSet<TWeakObjectPtr<AActor>> HitActors;
	TArray<AActor*> IgnoredActors;
	FVector PrevStart;
	FVector PrevEnd;
};

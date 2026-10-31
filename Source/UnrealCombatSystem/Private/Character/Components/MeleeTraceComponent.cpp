#include "Character/Components/MeleeTraceComponent.h"

#include "Kismet/KismetSystemLibrary.h"

UMeleeTraceComponent::UMeleeTraceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UMeleeTraceComponent::SetTraceMesh(USceneComponent* InMesh)
{
	if (InMesh)
	{
		TraceMesh = InMesh;
		AddTickPrerequisiteComponent(InMesh);	
	}
}

bool UMeleeTraceComponent::StartHitTrace()
{
	USceneComponent* Mesh = TraceMesh.Get();
	if (!Mesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("MeleeTraceComponent on %s: TraceMesh is nullptr"), *GetNameSafe(GetOwner()));
		return false;
	}

	if (!Mesh->DoesSocketExist(StartSocket) || !Mesh->DoesSocketExist(EndSocket))
	{
		UE_LOG(LogTemp, Warning, TEXT("MeleeTraceComponent on %s: %s or %s not found on %s"),
			*GetNameSafe(GetOwner()),
			*StartSocket.ToString(),
			*EndSocket.ToString(),
			*GetNameSafe(Mesh));
		return false;
	}

	HitActors.Reset();
	PrevStart = Mesh->GetSocketLocation(StartSocket);
	PrevEnd = Mesh->GetSocketLocation(EndSocket);

	TArray<AActor*> Ignore {GetOwner()};
	GetOwner()->GetAttachedActors(Ignore, false);
	IgnoredActors = Ignore;
	
	SetComponentTickEnabled(true);
	return true;
}

void UMeleeTraceComponent::StopHitTrace()
{
	SetComponentTickEnabled(false);
}

void UMeleeTraceComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	USceneComponent* Mesh = TraceMesh.Get();
	if (!Mesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("MeleeTraceComponent on %s: TraceMesh is nullptr"), *GetNameSafe(GetOwner()));
		StopHitTrace();
		return;
	}

	const FVector CurStart = Mesh->GetSocketLocation(StartSocket);
	const FVector CurEnd = Mesh->GetSocketLocation(EndSocket);
	TArray<FHitResult> Hits;

	auto Sweep = [&](const FVector& From, const FVector& To)
	{
		TArray<FHitResult> Result;
		UKismetSystemLibrary::SphereTraceMultiForObjects(
			this, From, To, Radius, ObjectTypes, false,
			IgnoredActors, DebugTrace, Result, true);
		Hits.Append(Result);
	};

	Sweep(PrevEnd, CurEnd);
	Sweep(PrevStart, CurStart);
	Sweep(CurStart, CurEnd);

	for (const FHitResult& HitResult : Hits)
	{
		AActor* HitActor = HitResult.GetActor();
		if (!HitActor || HitActors.Contains(HitActor))
		{
			continue;
		}

		HitActors.Add(HitActor);
		OnTraceHit.Broadcast(HitActor, HitResult);
	}

	PrevStart = CurStart;
	PrevEnd = CurEnd;
}

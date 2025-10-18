#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AsyncComputeSubsystem.generated.h"

/**
 * WorldSubsystem to disable/enable AsyncCompute of Lumen and RDG.
 * See: https://forums.unrealengine.com/t/5-5-gpu-crash-device-hung-nodeandclustercull-recommended-action/2649934/2
 */
UCLASS()
class DISABLEASYNCCOMPUTEONMAPLOAD_API UAsyncComputeSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	static UAsyncComputeSubsystem* GetInstance(UWorld* World);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void DisableAsyncCompute();
	void EnableAsyncCompute();

	const TArray<FString>& GetConsoleCommands() const { return ConsoleCommands; }

private:
	void SetTimer(FTimerDelegate TimerDelegate, float DelayTime);
	void ClearTimer();
	void ExecConsoleCommand();

	void OnPreLoadMapWithContext(const FWorldContext& WorldContext, const FString& MapName);
	void OnPostLoadMapWithWorld(UWorld* World);

	TArray<FString> ConsoleCommands = { "r.Lumen.AsyncCompute", "r.RDG.AsyncCompute" };

	FTimerHandle TimerHandle;

	FDelegateHandle DelegateHandlePreLoadMapWithContext;
	FDelegateHandle DelegateHandlePostLoadMapWithWorld;
};
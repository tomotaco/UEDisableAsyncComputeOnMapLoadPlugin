#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AsyncComputeSubsystem.generated.h"

UCLASS()
class DISABLEASYNCCOMPUTEONMAPLOAD_API UAsyncComputeSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	static UAsyncComputeSubsystem* GetInstance(UWorld* World);

	void DisableAsyncCompute();
	void EnableAsyncCompute();

	const TArray<FString>& GetConsoleCommands() const { return ConsoleCommands; }

private:
	void TriggerDelayedEnableAsyncCompute();

	TArray<FString> ConsoleCommands = { "r.Lumen.AsyncCompute", "r.RDG.AsyncCompute" };
	FTimerHandle TimerHandle;
};
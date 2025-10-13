#include "AsyncComputeSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AsyncComputeLogGroup.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(AsyncComputeSubsystem)

static TAutoConsoleVariable<float> CVarDelayTimeEnableAsyncCompute(
	TEXT("DisableAsyncCompute.DelayTimeToEnable"),
	0.1f,
	TEXT("Delay time to enable AsyncCompute of Lumen and RDG"),
	ECVF_Scalability | ECVF_RenderThreadSafe);

/**
 * WorldSubsystem to disable/enable AsyncCompute of Lumen and RDG.
 * See: https://forums.unrealengine.com/t/5-5-gpu-crash-device-hung-nodeandclustercull-recommended-action/2649934/2
 */
UAsyncComputeSubsystem* UAsyncComputeSubsystem::GetInstance(UWorld* World)
{
	if (World == nullptr) return nullptr;
	auto* Subsystem = World->GetSubsystem<UAsyncComputeSubsystem>();
	return Subsystem;
}

void UAsyncComputeSubsystem::DisableAsyncCompute()
{
	bool IsEnabled = UKismetSystemLibrary::GetConsoleVariableBoolValue(ConsoleCommands[0]);
	UE_LOG(LogAsyncComp, Log, TEXT("UAsyncComputeSubsystem::DisableAsyncCompute(), IsEnabled=%s"), (IsEnabled ? TEXT("true") : TEXT("false")));
	if (!IsEnabled) {
		UE_LOG(LogAsyncComp, Log, TEXT("AsyncCompute not changed"));
		return;
	}
	UE_LOG(LogAsyncComp, Log, TEXT("AsyncCompute disabled immediately"));
	for (auto ConsoleCommand : ConsoleCommands) {
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), ConsoleCommand + " 0");
	}
}

void UAsyncComputeSubsystem::EnableAsyncCompute()
{
	bool IsEnabled = UKismetSystemLibrary::GetConsoleVariableBoolValue(ConsoleCommands[0]);
	UE_LOG(LogAsyncComp, Log, TEXT("UAsyncComputeSubsystem::EnableAsyncCompute(), IsEnabled==%s"), (IsEnabled ? TEXT("true") : TEXT("false")));
	if (IsEnabled) {
		UE_LOG(LogAsyncComp, Log, TEXT("AsyncCompute not changed"));
		return;
	}

	if (TimerHandle.IsValid()) {
		TimerHandle.Invalidate();
	}

	float DelayTime = CVarDelayTimeEnableAsyncCompute.GetValueOnGameThread();
	if (FMath::IsNearlyZero(DelayTime)) {
		UE_LOG(LogAsyncComp, Log, TEXT("AsyncCompute enabled immediately"));
		UWorld* World = GetWorld();
		for (auto ConsoleCommand : ConsoleCommands) {
			UKismetSystemLibrary::ExecuteConsoleCommand(World, ConsoleCommand + " 1");
		}
	}
	else {
		TriggerDelayedEnableAsyncCompute();
	}
}

void UAsyncComputeSubsystem::TriggerDelayedEnableAsyncCompute()
{
	UE_LOG(LogAsyncComp, Log, TEXT("Trigger enabling AsyncCompute"));
	float DelayTime = CVarDelayTimeEnableAsyncCompute.GetValueOnGameThread();
	UWorld* World = GetWorld();
	auto Commands = ConsoleCommands;
	auto TimerHandler = [DelayTime, World, Commands]() -> void
		{
			UE_LOG(LogAsyncComp, Log, TEXT("AsyncCompute enabled with delay time(%f)"), DelayTime);
			for (auto ConsoleCommand : Commands) {
				UKismetSystemLibrary::ExecuteConsoleCommand(World, ConsoleCommand + " 1");
			}
		};

#if WITH_EDITOR
	if (GEditor->IsEditor()) {
		GEditor->GetTimerManager()->SetTimer(TimerHandle, TimerHandler, DelayTime, false);
	}
	else
#endif
	{
		World->GetTimerManager().SetTimer(TimerHandle, TimerHandler, DelayTime, false);
	}
}

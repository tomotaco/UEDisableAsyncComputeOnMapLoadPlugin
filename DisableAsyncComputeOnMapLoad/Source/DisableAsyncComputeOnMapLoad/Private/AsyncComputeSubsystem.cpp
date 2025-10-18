#include "AsyncComputeSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AsyncComputeLogGroup.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(AsyncComputeSubsystem)

static TAutoConsoleVariable<float> CVarDelayTimeEnableAsyncCompute(
	TEXT("DisableAsyncCompute.DelayTimeToEnable"),
	5.0f,
	TEXT("Delay time to enable AsyncCompute of Lumen and RDG"),
	ECVF_Scalability | ECVF_RenderThreadSafe);

UAsyncComputeSubsystem* UAsyncComputeSubsystem::GetInstance(UWorld* World)
{
	if (World == nullptr) return nullptr;
	auto* Subsystem = World->GetSubsystem<UAsyncComputeSubsystem>();
	return Subsystem;
}


void UAsyncComputeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	DelegateHandlePreLoadMapWithContext = FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &UAsyncComputeSubsystem::OnPreLoadMapWithContext);
	DelegateHandlePostLoadMapWithWorld = FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UAsyncComputeSubsystem::OnPostLoadMapWithWorld);

}

void UAsyncComputeSubsystem::Deinitialize()
{
	if (TimerHandle.IsValid()) {
		ClearTimer();
	}

	FCoreUObjectDelegates::PreLoadMapWithContext.Remove(DelegateHandlePreLoadMapWithContext);
	FCoreUObjectDelegates::PreLoadMapWithContext.Remove(DelegateHandlePostLoadMapWithWorld);

	Super::Deinitialize();
}

void UAsyncComputeSubsystem::SetTimer(FTimerDelegate TimerDelegate, float DelayTime)
{
#if WITH_EDITOR
	if (GEditor->IsEditor()) {
		GEditor->GetTimerManager()->SetTimer(TimerHandle, TimerDelegate, DelayTime, false);
	}
	else
#endif
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, DelayTime, false);
	}
}

void UAsyncComputeSubsystem::ClearTimer()
{
#if WITH_EDITOR
	if (GEditor->IsEditor()) {
		GEditor->GetTimerManager()->ClearTimer(TimerHandle);
	}
	else
#endif
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
	TimerHandle.Invalidate();
}

void UAsyncComputeSubsystem::DisableAsyncCompute()
{
	bool IsEnabled = UKismetSystemLibrary::GetConsoleVariableBoolValue(ConsoleCommands[0]);
	UE_LOG(LogAsyncComp, Log, TEXT("UAsyncComputeSubsystem::DisableAsyncCompute(), IsEnabled=%s"), (IsEnabled ? TEXT("true") : TEXT("false")));
	if (!IsEnabled) {
		UE_LOG(LogAsyncComp, Log, TEXT("AsyncCompute not changed"));
		return;
	}

	if (TimerHandle.IsValid()) {
		ClearTimer();
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

	UE_LOG(LogAsyncComp, Log, TEXT("Trigger enabling AsyncCompute"));
	float DelayTime = CVarDelayTimeEnableAsyncCompute.GetValueOnGameThread();
	if (TimerHandle.IsValid()) {
		ClearTimer();
	}
	UWorld* World = GetWorld();
	auto Commands = ConsoleCommands;
	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &UAsyncComputeSubsystem::ExecConsoleCommand);
	SetTimer(Delegate, DelayTime);
}

void UAsyncComputeSubsystem::ExecConsoleCommand()
{
	float DelayTime = CVarDelayTimeEnableAsyncCompute.GetValueOnGameThread();
	UE_LOG(LogAsyncComp, Log, TEXT("AsyncCompute enabled with delay time(%f)"), DelayTime);
	for (auto ConsoleCommand : ConsoleCommands) {
		UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), ConsoleCommand + " 1");
	}
}

void UAsyncComputeSubsystem::OnPreLoadMapWithContext(const FWorldContext& WorldContext, const FString& MapName)
{
	UE_LOG(LogAsyncComp, Log, TEXT("UAsyncComputeSubsystem::OnPreLoadMapWithContext(%s)"), *MapName);
	DisableAsyncCompute();

}

void UAsyncComputeSubsystem::OnPostLoadMapWithWorld(UWorld* World)
{
	UE_LOG(LogAsyncComp, Log, TEXT("UAsyncComputeSubsystem::OnPostLoadMapWithWorld()"));
	EnableAsyncCompute();
}

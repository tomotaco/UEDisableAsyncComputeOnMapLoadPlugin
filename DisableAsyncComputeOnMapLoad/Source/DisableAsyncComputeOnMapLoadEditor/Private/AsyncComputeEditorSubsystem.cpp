#include "AsyncComputeEditorSubsystem.h"
#include "DisableAsyncComputeOnMapLoad/Public/AsyncComputeSubsystem.h"
#include "DisableAsyncComputeOnMapLoad/Public/AsyncComputeLogGroup.h"
#include "Subsystems/UnrealEditorSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"

UAsyncComputeEditorSubsystem::UAsyncComputeEditorSubsystem()
{
}

void UAsyncComputeEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FEditorDelegates::OnMapLoad.AddUObject(this, &UAsyncComputeEditorSubsystem::OnMapLoad);
	FEditorDelegates::OnMapOpened.AddUObject(this, &UAsyncComputeEditorSubsystem::OnMapOpened);
}


TObjectPtr<UWorld> UAsyncComputeEditorSubsystem::GetEditorWorld()
{
	auto EditorSubsystem = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>();
	return EditorSubsystem->GetEditorWorld();
}

void UAsyncComputeEditorSubsystem::OnMapLoad(const FString& Filename, FCanLoadMap& OutCanLoadMap)
{
	IsEnabledAsyncCompute = UKismetSystemLibrary::GetConsoleVariableBoolValue("r.RDG.AsyncCompute");
	UE_LOG(LogAsyncComp, Log, TEXT("UAsyncComputeEditorSubsystem::OnMapLoad(), IsEnabledAsyncComputeToKeep=%s"), (IsEnabledAsyncCompute ? TEXT("true") : TEXT("false")));
	auto AsyncComputeSubsystem = UAsyncComputeSubsystem::GetInstance(GetEditorWorld());
	if (AsyncComputeSubsystem) {
		AsyncComputeSubsystem->DisableAsyncCompute();
	}
}

void UAsyncComputeEditorSubsystem::OnMapOpened(const FString& Filename, bool bAsTemplate)
{
	UE_LOG(LogAsyncComp, Log, TEXT("UAsyncComputeEditorSubsystem::OnMapOpened()"));
	HandleOnEndFrame = FCoreDelegates::OnEndFrame.AddUObject(this, &UAsyncComputeEditorSubsystem::OnEndFrame);
}

void UAsyncComputeEditorSubsystem::OnEndFrame()
{
	UE_LOG(LogAsyncComp, Log, TEXT("UAsyncComputeEditorSubsystem::OnEndFrame()"));
	FCoreDelegates::OnEndFrame.Remove(HandleOnEndFrame);
	HandleOnEndFrameRT = FCoreDelegates::OnEndFrameRT.AddUObject(this, &UAsyncComputeEditorSubsystem::OnEndFrameRT);
}

void UAsyncComputeEditorSubsystem::OnEndFrameRT()
{
	AsyncTask(ENamedThreads::GameThread, [this]() {
		UE_LOG(LogAsyncComp, Log, TEXT("UAsyncComputeEditorSubsystem::OnEndFrameRT(), IsEnabledAsyncComputeToRestore=%s"), (IsEnabledAsyncCompute ? TEXT("true") : TEXT("false")));
		auto AsyncComputeSubsystem = UAsyncComputeSubsystem::GetInstance(GetEditorWorld());
		if (AsyncComputeSubsystem && IsEnabledAsyncCompute) {
			AsyncComputeSubsystem->EnableAsyncCompute();
		}
		FCoreDelegates::OnEndFrameRT.Remove(HandleOnEndFrameRT);
	});
}

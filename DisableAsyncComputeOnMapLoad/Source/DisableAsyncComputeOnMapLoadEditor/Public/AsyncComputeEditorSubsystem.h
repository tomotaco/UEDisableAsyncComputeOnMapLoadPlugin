#pragma once
#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "AsyncComputeEditorSubsystem.generated.h"


/**
 * EditorSubsystem to controll GPU AsyncCompute state.
 * Disable AsyncCompute on level load at editor, and enable it after completed.
 */
UCLASS()
class DISABLEASYNCCOMPUTEONMAPLOADEDITOR_API UAsyncComputeEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

public:
	UAsyncComputeEditorSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	TObjectPtr<UWorld> GetEditorWorld();
	bool IsEnabledAsyncCompute = false;
	void OnMapLoad(const FString& Filename, FCanLoadMap& OutCanLoadMap);
	void OnMapOpened(const FString& Filename, bool bAsTemplate);
	void OnEndFrame();
	void OnEndFrameRT();
	FDelegateHandle HandleOnEndFrame;
	FDelegateHandle HandleOnEndFrameRT;
};

#pragma once
#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "AsyncComputeEditorSubsystem.generated.h"


/**
 * GPU非同期計算処理の状態管理クラス(Editor専用)
 * エディタ上でレベルロード中にGPUのAyncComputeを停止し、ロード完了後に元に戻す処理を行う
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

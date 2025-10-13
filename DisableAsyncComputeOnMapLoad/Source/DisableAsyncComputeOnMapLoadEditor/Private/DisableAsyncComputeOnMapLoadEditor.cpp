#include "DisableAsyncComputeOnMapLoadEditor.h"

#define LOCTEXT_NAMESPACE "FDisableAsyncComputeOnMapLoadEditorModule"

void FDisableAsyncComputeOnMapLoadEditorModule::StartupModule()
{
}

void FDisableAsyncComputeOnMapLoadEditorModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDisableAsyncComputeOnMapLoadEditorModule, DisableAsyncComputeOnMapLoadEditor)
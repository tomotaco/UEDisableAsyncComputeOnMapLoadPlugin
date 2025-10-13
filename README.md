# UEDisableAsyncComputeOnMapLoadPlugin

UE Plugin to disable AsyncCompute while editor map load, intended to avoid GPU Crash([UE-230827](https://issues.unrealengine.com/issue/UE-230827)) for UE5.5

# How to reproduce GPU crash

- Make sure using GeForce 30/40 series GPU.

- Install [Hillside Sample project](https://www.fab.com/ja/listings/3277687b-a06f-4ef7-a285-63b981768c4a). (Select engine version 5.5)

- Disable raytracing in `Config/DefaultEngine.ini` by modifying the following line

  ```
  r.RayTracing=True
  ```

  to

  ```
  r.RayTracing=False
  ```

- Launch UE editor and open `HillsideSampleProject.uproject`.
- Create an empty level, and set it to EditorStartupMap in `Project settings` / `Maps & Modes`.
- Exit UE Editor, and launch again.
- Double-click at `Content/Hillside/Maps/LV_Exterior`.
- Comfirm UE Editor crashes. (If not, you don't need this plugin.)

# How to use thie plugin for UE editor

- Copy `DisableAsyncComputeOnMapLoad` folder below `Plugins` folder.
- Launch UE editor again, open `LV_Exterior`.
- Now UE editor won't crash.

# How to use thie plugin from runtime

Before start of map load:

```
#include "DisableAsyncComputeOnMapLoad/Public/AsyncComputeSubsystem.h"
  :
auto* AsyncComputeSubsystem = UAsyncComputeSubsystem::GetInstance(GetWorld());
if (AsyncComputeSubsystem) {
    AsyncComputeSubsystem->DisableAsyncCompute();
}
```

After completion of map load:

```
#include "DisableAsyncComputeOnMapLoad/Public/AsyncComputeSubsystem.h"
  :
auto* AsyncComputeSubsystem = UAsyncComputeSubsystem::GetInstance(GetWorld());
if (AsyncComputeSubsystem) {
    AsyncComputeSubsystem->EnableAsyncCompute();
}
```

Add dependency configuretion to your `*.Build.cs`

```
		PrivateIncludePathModuleNames.Add("DisableAsyncComputeOnMapLoad");
        PrivateDependencyModuleNames.Add("DisableAsyncComputeOnMapLoad");
```

# Console variables

- `DisableAsyncCompute.DelayTimeToEnable`

  Time(seconds) to enable async compute after `OnEndFrameRT`. Default is 5.0s.

If UE editor still crashes, set it to larger value.

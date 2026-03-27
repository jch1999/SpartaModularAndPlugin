// MySpartaLog.Build.cs

using UnrealBuildTool;

public class MySpartaLog : ModuleRules
{
	public MySpartaLog(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		// 현재 모듈의 루트 폴더를 포함 경로에 추가
		PublicIncludePaths.Add(ModuleDirectory);
		
		PublicDependencyModuleNames.AddRange(new string[]
			{
				// Initail Modules
				"Core", "CoreUObject", "Engine", "InputCore",
			}
		);

		PrivateDependencyModuleNames.AddRange(new string[]
			{
			}
		);
	}

}
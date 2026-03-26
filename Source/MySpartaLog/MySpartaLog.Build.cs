// MySpartaLog.Build.cs

using UnrealBuildTool;

public class MySpartaLog : ModuleRules
{
	public MySpartaLog(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

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
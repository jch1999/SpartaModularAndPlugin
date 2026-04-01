using UnrealBuildTool;

public class MySpartaLogEditor : ModuleRules
{
	public MySpartaLogEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		// 현재 모듈의 루트 폴더를 포함 경로에 추가
		PublicIncludePaths.Add(ModuleDirectory);
		
		PublicDependencyModuleNames.AddRange(new string[]
			{
				// Initail Modules
				"Core",
				"CoreUObject",
				"Engine",
				"UnrealEd",
				"Slate",
				"SlateCore",
				"EditorStyle",
				"ToolMenus",
				"MySpartaLog",
				"EditorFramework",
				"AppFramework",
				"ApplicationCore",
				"PropertyEditor",
				"InputCore",
			}
		);
	}
}
// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTagGame_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_TagGame;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_TagGame()
	{
		if (!Z_Registration_Info_UPackage__Script_TagGame.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/TagGame",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0xFB93CBAD,
				0xA34DFD5F,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_TagGame.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_TagGame.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_TagGame(Z_Construct_UPackage__Script_TagGame, TEXT("/Script/TagGame"), Z_Registration_Info_UPackage__Script_TagGame, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0xFB93CBAD, 0xA34DFD5F));
PRAGMA_ENABLE_DEPRECATION_WARNINGS

local GEN_PROJECT_FILE_PATH = "Temp/ProjectFiles/"
local BIN_PATH        = "Bin/%{cfg.buildcfg}/"
local BIN_DYNAMIC_TARGET_PATH = "Bin/%{cfg.buildcfg}/DynamicDlls"
local OBJECT_PATH     = "Temp/Obj/%{cfg.buildcfg}/"
local PCH_HEADER      = "PCH/PCH.h"
local PCH_HEADER_PATH = "Source/PCH/PCH.h"
local PCH_CPP_PATH    = "Source/PCH/PCH.cpp"

function DebugConfig(defined)
    symbols       "On"
    optimize      "Off"
    defines       {"_DEBUG"}
    cppdialect    "C++20"
    staticruntime "off"
    runtime       "Debug"
end

function ConfirmConfig(defined)
    optimize        "Full" 
    defines         {"_RELEASE", "NDEBUG"}
    cppdialect      "C++20"
    staticruntime   "off"
    runtime         "Release"
end

function ReleaseConfig(defined)
    optimize        "Full" 
    defines         {"_RELEASE", "NDEBUG"}
    cppdialect      "C++20"
    staticruntime   "off"
    runtime         "Release"
end

workspace "HEServer"
    architecture "x64"

    configurations
    { 
        "Debug",  -- Engine : Debug,  Game : Debug,  Profile On
		"Develop",
        "Release",    -- Engine : Release, Game : Debug, Profile On 
    }

    -- "StaticLib"  --
    -- "SharedLib"  --
    -- "ConsoleApp" --
    function SetCPPProjectConfig(kind_name, path, defined)
        location  (GEN_PROJECT_FILE_PATH)
        kind (kind_name)
        language "C++"
        debugdir  (BIN_PATH)
        targetdir (BIN_PATH)
        objdir(OBJECT_PATH)
        pchheader (PCH_HEADER)
        pchsource (PCH_CPP_PATH)
        if defined ~= nil then
            defines {defined}
        end
        -- file
        files {
            path .. "**.h",
            path .. "**.cpp",
            path .. "**.c",
            PCH_HEADER_PATH,
            PCH_CPP_PATH,
        }
    end

    function SetDynamicCPPProjectConfig(kind_name, path, defined, codeGenPath)
        location  (GEN_PROJECT_FILE_PATH)
        kind (kind_name)
        language "C++"
        debugdir  (BIN_PATH)
        targetdir (BIN_DYNAMIC_TARGET_PATH)
        objdir(OBJECT_PATH)
        pchheader (PCH_HEADER)
        pchsource (PCH_CPP_PATH)
        if defined ~= nil then
            defines {defined}
        end
       
        -- file
        files {
            path .. "**.h",
            path .. "**.cpp",
            path .. "**.c",
            codeGenPath .. "**.h",
            codeGenPath .. "**.cpp",
            PCH_HEADER_PATH,
            PCH_CPP_PATH,
        }
    end

    group "Server"
		group "Server/GameServer"
			project "HEServer"
				includedirs{ "ThirdParty/", "Source/", "Source/HEServer/", }
				links{ "Core", "BattleSystem"}
				SetCPPProjectConfig("ConsoleApp", "Source/HEServer/", {"_HESERVER", })
				filter "configurations:Debug"
					DebugConfig()
					defines{"_DEBUG"}
				filter "configurations:Develop"
					ConfirmConfig()
					defines{"_CONFIRM"}
				filter "configurations:Release"
					ReleaseConfig()
					defines{"_RELEASE"}


			project "BattleSystem"
				includedirs{ "ThirdParty/", "Source/", "Source/BattleSystem/", }
				links{ "Core" }
				SetCPPProjectConfig("StaticLib", "Source/BattleSystem/", {"_HESERVER"})
				filter "configurations:Debug"
					DebugConfig()
					defines{"_DEBUG"}
				filter "configurations:Develop"
					ConfirmConfig()
					defines{"_CONFIRM"}
				filter "configurations:Release"
					ReleaseConfig()
					defines{"_RELEASE"}

			project "Core"
				includedirs{ "ThirdParty/", "Source/", "Source/Core/", }
				links{}
				SetCPPProjectConfig("StaticLib", "Source/Core/", {"_HESERVER", })
				filter "configurations:Debug"
					DebugConfig()
					defines{"_DEBUG"}
				filter "configurations:Develop"
					ConfirmConfig()
					defines{"_CONFIRM"}
				filter "configurations:Release"
					ReleaseConfig()
					defines{"_RELEASE"}



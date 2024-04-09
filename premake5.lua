workspace "RTIOW"
	architecture "x64"
	startproject "RTIOW"

	configurations
	{
		"Debug",
		"Release"
	}

local tempPath = _MAIN_SCRIPT_DIR .. "/"

-- OUTPUT DIRECTORIES
OutDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"
BinDir = tempPath .. "Build/Bin/" .. OutDir
IntDir = tempPath .. "Build/Intermediates/" .. OutDir

-- EXTERNAL DEPENCENCIES DIR
ExtDep = tempPath .. "ED"

-- INCLUDE DIRECTORIES
IncludeDir = {}
IncludeDir["glm"] = "%{ExtDep}/glm/"

group "Apps"
	include "RTIOW/Source"

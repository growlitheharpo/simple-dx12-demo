
function set_location()
	location "%{wks.location}/proj/%{prj.name}/"
end

function get_root_location()
	return "%{wks.location}/../"
end

function get_output_location(prj_name)
	return get_root_location() .. "build/bin/" .. prj_name .. "/%{cfg.longname}/"
end

function include_mathfu(useSysInclude)
	if useSysInclude then
		sysincludedirs {
			"external/google-mathfu/include",
			"external/google-mathfu/dependencies/vectorial/include"
		}
	else
		includedirs {
			"external/google-mathfu/include",
			"external/google-mathfu/dependencies/vectorial/include"
		}
	end
	defines {
		"MATHFU_COMPILE_WITH_SIMD=1"
	}
end

workspace "dx12-playground"
	location "build/"
	language "C++"
	cppdialect "c++17"
	startproject "dx12-playground"

	architecture "x86_64"
	configurations { "Debug", "Release" }

	defines {
		"IN_USE=2",
		"NOT_IN_USE=4",
	}

	defines {
		"DEBUGGING=NOT_IN_USE",
		"DEVELOPMENT=NOT_IN_USE",
		"RELEASE=NOT_IN_USE",
	}

	filter { "configurations:Debug" }
		removedefines { "DEBUGGING*", "DEVELOPMENT*" }
		defines { 
			"DEBUG", 
			"_DEBUG",
			"DEBUGGING=IN_USE",
			"DEVELOPMENT=IN_USE",
		}
		symbols "On"

	filter { "configurations:Release" }
		removedefines { "RELEASE*" }
		defines { 
			"NDEBUG",
			"RELEASE=IN_USE"
		}
		optimize "On"

	filter {"system:windows", "action:vs*"}
		systemversion "latest"
		links {
			"d3d12.lib",
			"dxgi.lib",
			"dxguid.lib",
			"d3dcompiler.lib",
		}

	filter {}

	flags {
		"FatalWarnings"
	}

	defines {
		"_ITERATOR_DEBUG_LEVEL=0",
		"NOMINMAX",
		"WIN32_LEAN_AND_MEAN",
		"VC_EXTRALEAN"
	}

	targetdir ("build/bin/%{prj.name}/%{cfg.longname}")
	objdir ("build/obj/%{prj.name}/%{cfg.longname}")

project "dx12-playground"
	set_location()
	kind "WindowedApp"
	files {
		"src/**.cpp",
		"src/**.h",
		"src/**.hlsl",
	}

	debugdir("%{wks.location}")

	filter { "files:**.hlsl" }
		shaderobjectfileoutput("%{wks.location}/shaders/%{file.name}.cso")
		shadermodel "6.0"
	filter { "files:**_vs.hlsl" }
		shadertype "Vertex"
		shaderentry "main"
	filter { "files:**_ps.hlsl" }
		shadertype "Pixel"
		shaderentry "main"
	filter {}

	include_mathfu()
	includedirs {
		"external/microsoft",
		"src/",
	}


group "external"
project "google-mathfu"
	set_location()
	kind "None"
	include_mathfu(true)
	files {
		"external/google-mathfu/include/**",
	}

group "*"
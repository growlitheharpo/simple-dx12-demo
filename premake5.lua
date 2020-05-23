
function set_location()
	location "%{wks.location}/proj/%{prj.name}/"
end

function get_root_location()
	return "%{wks.location}/../"
end

function get_output_location(prj_name)
	return get_root_location() .. "build/bin/" .. prj_name .. "/%{cfg.longname}/"
end

workspace "dx12-tutorual"
	location "build/"
	language "C++"
	cppdialect "c++17"
	startproject "dx12-tutorial"

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

project "dx12-tutorial"
	set_location()
	kind "WindowedApp"
	files {
		"src/**",
	}

	includedirs {
		"external/microsoft",
		"src/"
	}

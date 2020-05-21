
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

	filter { "configurations:Debug" }
		defines { "DEBUG", "_DEBUG" }
		symbols "On"

	filter { "configurations:Release" }
		defines { "NDEBUG" }
		optimize "On"

	filter {"system:windows", "action:vs*"}
		systemversion "latest"
		links {
			"msvcrtd.lib",
			"vcruntimed.lib",
		}

	filter {}

	flags {
		"FatalWarnings"
	}

	defines {
		"_ITERATOR_DEBUG_LEVEL=0",
	}

	targetdir ("build/bin/%{prj.name}/%{cfg.longname}")
	objdir ("build/obj/%{prj.name}/%{cfg.longname}")

project "dx12-tutorial"
	set_location()
	kind "WindowedApp"
	files {
		"src/**",
	}

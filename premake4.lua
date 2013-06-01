function copy(src, dst, always)
  local action = "python"
  local script = "\"" ..  path.join(os.getcwd(), "copy-data.py")  .. "\""
  src = "\"" .. src .. "\""
  dst = "\"" .. dst .. "\""
  cwd = "\"" .. os.getcwd() .. "\""
  postbuildcommands { action  .. " " .. script .. " " .. cwd .. " " .. src .. " " .. dst .. " " .. tostring(always) }
end

function configurationPath(action, arch, conf)
  return path.join(action, path.join(arch, conf))
end

_config_path = nil
_action = nil
_architecture = nil
_configuration = nil
_build = nil

function setConfig(action, arch, conf)
  _action = action
  _architecture = arch
  _configuration = conf

  _build = {}
  if action ~= nil then table.insert(_build, action) end
  if arch ~= nil then table.insert(_build, arch) end
  if conf ~= nil then table.insert(_build, conf) end

  configuration(_build)

  if action ~= nil and arch ~= nil and conf ~= nil then
    _config_path = configurationPath(action, arch, conf)
  end
end

function forEachWinPlatform(func)
  for _,arch in pairs({"x32", "x64"}) do
    for _,conf in pairs({"debug", "release"}) do
      for _, action in pairs({"vs2008"}) do
        func(action, arch, conf)
      end
    end
  end
end

function resource(src, dst, always)
  if always == nil then
    always = false
  else
    always = true
  end

  copy(src, path.join("build", dst), always)
  forEachWinPlatform(function (action, arch, conf)
    setConfig(action, arch, conf)
    copy(src, path.join("bin", path.join(_config_path, dst)), always)
  end)
  configuration "*"
end

function windowsLibDir(name)
  forEachWinPlatform(function (action, arch, conf)
    setConfig(action, arch, conf)

    libdirs { path.join("lib", path.join(name, _config_path)) }
  end)
  configuration "*"
end

function windowsBinary(basePath, dllName)
  forEachWinPlatform(function (action, arch, conf)
    setConfig(action, arch, conf)

    local libBase = path.join("lib", path.join(basePath, _config_path))
    copy(path.join(libBase, dllName), path.join("bin", path.join(_config_path, dllName)))
  end)
  configuration "*"
end

newaction {
  trigger = 'clean',
  description = 'Cleans up the project.',
  shortname = "clean",

  execute = function()
    os.rmdir("bin")
    os.rmdir("build")
  end
}

solution "FOnlineOpenSource"
  configurations { "debug", "release" }
  platforms { "x32", "x64" }
  location "build"

  forEachWinPlatform(function (action, arch, conf)
    setConfig(action, arch, conf)
    targetdir(path.join("bin", _config_path))
  end)

  configuration "debug"
    defines { "DEBUG" }
    flags { "Symbols" }

  configuration "release"
    defines { "NDEBUG" }
    flags { "Optimize" }

  configuration "windows"
    includedirs "inc/win32"
    defines { "WIN32", "_WIN32" }
    defines { "_CRT_SECURE_NO_WARNINGS", "_CRT_NONSTDC_NO_DEPRECATE" }
    defines "HAVE_VSNPRINTF"

  project "FOnlineClient"
    kind "WindowedApp"
    language "C++"

    flags { "WinMain" }

    includedirs { "inc", "src" }

    links { "FOnlineCommon",
            "FOnlineFileManager",
            "IniFile",
            "SimpleLeakDetector",
            "LZSS",
            "ACMDecompressor",
            "DatArchive" }

    files {
      "src/FOnlineClient/**.hpp",
      "src/FOnlineClient/**.h",
      "src/FOnlineClient/**.cpp",
      "src/FOnlineClient/**.rc"
    }

    resource("data", "data")

    resincludedirs { "src/FOnlineClient" }

    -- DirectX
    includedirs { "dx8sdk/include" }
    configuration "x32"
      libdirs { "dx8sdk/lib" }
      links { "d3dx8", "d3d8", "dinput8", "dxguid", "dxerr8", "wsock32", "dsound" }
      linkoptions { "/nodefaultlib:libci.lib" }

    -- Ogg + Vorbis
    windowsLibDir("libogg")
    windowsLibDir("libvorbis")
    windowsBinary("libogg", "libogg.dll")
    windowsBinary("libvorbis", "libvorbis.dll")
    windowsBinary("libvorbis", "libvorbisfile.dll")
    links { "libogg", "libvorbis", "libvorbisfile" }

    -- ZLib
    includedirs { "inc/zlib" }
    windowsLibDir("zlib")
    windowsBinary("zlib", "zlib.dll")
    links "zlib"

    -- Winsock
    configuration "windows"
      links { "ws2_32" }

  project "FOnlineServer"
    kind "WindowedApp"
    language "C++"

    flags { "WinMain" }

    includedirs { "inc", "src" }

    links { "FOnlineCommon",
            "FOnlineFileManager",
            "IniFile",
            "SimpleLeakDetector" }

    files {
      "src/FOnlineServer/**.hpp",
      "src/FOnlineServer/**.h",
      "src/FOnlineServer/**.cpp",
      "src/FOnlineServer/**.rc"
    }

    resincludedirs { "src/FOnlineServer" }

    -- MySQL
    windowsLibDir("libmysql")
    windowsBinary("libmysql", "libmysql.dll")
    links { "libmysql" }

    -- Winsock
    configuration "windows"
      links { "ws2_32" }

    -- ZLib
    includedirs { "inc/zlib" }
    windowsLibDir("zlib")
    windowsBinary("zlib", "zlib.dll")
    links "zlib"

  project "FOnlineFileManager"
    kind "SharedLib"
    language "C++"

    defines "FONLINE_FILEMANAGER_DYNAMIC"
    defines "FONLINE_FILEMANAGER_EXPORTS"

    links "DatArchive"
    links "FOnlineCommon"

    includedirs { "src", "inc" }

    files { "src/FOnlineFileManager/**.cpp",
            "src/FOnlineFileManager/**.hpp" }

  project "FOnlineCommon"
    kind "SharedLib"
    language "C++"

    defines { "FONLINE_COMMON_DYNAMIC" }
    defines { "FONLINE_COMMON_EXPORTS" }

    includedirs { "inc", "src" }

    files {
      "src/FOnlineCommon/**.hpp",
      "src/FOnlineCommon/**.cpp",
      "src/FOnlineCommon/**.rc"
    }

    -- DirectX
    includedirs { "dx8sdk/include" }
    configuration { "x32", "windows" }
      libdirs { "dx8sdk/lib" }
      links { "d3dx8", "d3d8", "dinput8", "dxguid", "dxerr8", "wsock32", "dsound" }
      linkoptions { "/nodefaultlib:libci.lib" }

  project "IniFile"
    kind "SharedLib"
    language "C++"

    defines "INIFILE_DLL"

    includedirs { "src" }
    files { "src/IniFile/**.cpp",
            "src/IniFile/**.hpp" }

  project "SimpleLeakDetector"
    kind "SharedLib"
    language "C++"

    defines "SIMPLELEAKDETECTOR_DLL"

    includedirs { "src" }
    files { "src/SimpleLeakDetector/**.cpp",
            "src/SimpleLeakDetector/**.hpp" }

  project "LZSS"
    kind "SharedLib"
    language "C++"

    defines "LZSS_DLL"

    includedirs { "src" }
    files { "src/LZSS/**.cpp",
            "src/LZSS/**.hpp" }

  project "ACMDecompressor"
    kind "SharedLib"
    language "C++"

    defines "ACMDECOMPRESSOR_DLL"

    includedirs { "src" }
    files { "src/ACMDecompressor/**.cpp",
            "src/ACMDecompressor/**.hpp" }

  project "DatArchive"
    kind "SharedLib"
    language "C++"

    includedirs { "inc/zlib" }
    windowsLibDir("zlib")
    windowsBinary("zlib", "zlib.dll")
    links "zlib"

    links "LZSS"

    defines "DATARCHIVE_DLL"

    includedirs { "src" }
    files { "src/DatArchive/**.cpp",
            "src/DatArchive/**.hpp" }

  project "UnitTest++"
    kind "StaticLib"
    language "C++"

    files { "src/UnitTest++/*.cpp",
            "src/UnitTest++/*.h" }

    configuration "windows"
      files { "src/UnitTest++/Win32/**.cpp",
              "src/UnitTest++/Win32/**.h" }

    configuration "linux"
      files { "src/UnitTest++/Posix/**.cpp",
              "src/UnitTest++/Posix/**.h" }
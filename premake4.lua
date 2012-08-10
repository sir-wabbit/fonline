function copy(src, dst)
  local action = "\"" ..  path.join(os.getcwd(), "copy-data.py")  .. "\""
  src = "\"" .. src .. "\""
  dst = "\"" .. dst .. "\""
  cwd = "\"" .. os.getcwd() .. "\""
  postbuildcommands { action .. " " .. cwd .. " " .. src .. " " .. dst }
end

function resource(proj, src, dst)
  copy(src, path.join("build", dst))
  if proj == nil then
    copy(src, path.join("bin", dst))
  else
    copy(src, path.join(path.join("bin", proj), dst))
  end
end

function windows_libdir(basePath)
  for _,arch in pairs({"x32", "x64"}) do
    for _,conf in pairs({"debug", "release"}) do
      for _, plat in pairs({"vs2008"}) do
        local confpath = plat .. "/" .. arch .. "/" .. conf
        configuration { "windows", arch, conf, plat }
          libdirs { path.join(basePath, confpath) }
      end
    end
  end
end

function windows_binary(basePath, dllName)
  for _,arch in pairs({"x32", "x64"}) do
    for _,conf in pairs({"debug", "release"}) do
      for _, plat in pairs({"vs2008"}) do
        local confpath = plat .. "/" .. arch .. "/" .. conf
        configuration { "windows", arch, conf, plat }
          resource(path.join(path.join(basePath, confpath), dllName), dllName, true)
      end
    end
  end
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

solution "fonline-open-source"
  configurations { "debug", "release" }
  platforms { "x32" }
  location "build"
  
  for _,arch in pairs({"x32", "x64"}) do
    for _,conf in pairs({"debug", "release"}) do
      for _,plat in pairs({"vs2008"}) do
        configuration { arch, conf, plat }
          targetdir(path.join("bin", path.join(plat, path.join(arch, conf))))
      end
    end
  end

  configuration "debug"
    defines { "DEBUG" }
    flags { "Symbols" }
  
  configuration "release"
    defines { "NDEBUG" }
    flags { "Optimize" }
    
  configuration "vs2010 or vs2008"
    defines { "_CRT_SECURE_NO_WARNINGS",
              "_CRT_NONSTDC_NO_DEPRECATE" }
    
  configuration "windows"
    defines "HAVE_VSNPRINTF"
  
  project "fonline-client"
    kind "WindowedApp"
    language "C++"
    
    flags { "WinMain" }
        
    includedirs { "inc", "src" }
    
    links { "fo-base" }
    
    files { 
      "src/client/**.hpp", 
      "src/client/**.h", 
      "src/client/**.cpp",
      "src/client/**.rc"
    }
    
    resource(proj, "data", "data")
    
    resincludedirs { "src/client" }
    
    links { "zlib" }
    includedirs { "src/zlib" }
    
    -- DirectX
    includedirs { "dx8sdk/include" }
    configuration "x32"
      libdirs { "dx8sdk/lib" }
      links { "d3dx8", "d3d8", "dinput8", "dxguid", "dxerr8", "wsock32", "dsound" }
      linkoptions { "/nodefaultlib:libci.lib" }
      
    -- Ogg + Vorbis
    configuration { "vs2008", "x32", "debug" }
      libdirs { "lib/vs2008/x32/debug/libogg", "lib/vs2008/x32/debug/libvorbis" }
      resource(nil, "lib/vs2008/x32/debug/libogg/libogg.dll", "libogg.dll")
      resource(nil, "lib/vs2008/x32/debug/libvorbis/libvorbis.dll", "libvorbis.dll")
      resource(nil, "lib/vs2008/x32/debug/libvorbis/libvorbisfile.dll", "libvorbisfile.dll")
    configuration { "vs2008", "x32", "release" }
      libdirs { "lib/vs2008/x32/release/libogg", "lib/vs2008/x32/release/libvorbis" }
      resource(nil, "lib/vs2008/x32/release/libogg/libogg.dll", "libogg.dll")
      resource(nil, "lib/vs2008/x32/release/libvorbis/libvorbis.dll", "libvorbis.dll")
      resource(nil, "lib/vs2008/x32/release/libvorbis/libvorbisfile.dll", "libvorbisfile.dll")
    configuration "*"
      links { "libogg", "libvorbis", "libvorbisfile" }
      
    links { "ws2_32" }
  
  project "fo-base"
    kind "SharedLib"
    language "C++"
    
    defines { "FO_BASE_DLL" }
    
    includedirs { "inc", "src" }
    
    files {
      "src/base/**.hpp",
      "src/base/**.cpp",
      "src/base/**.rc"
    }
  
  project "zlib"
    kind "StaticLib"
    language "C"
    
    files { 
      "src/zlib/**.h", 
      "src/zlib/**.c"
    }
  
  project "fonline-server"
    kind "WindowedApp"
    language "C++"
    
    flags { "WinMain" }
    
    includedirs { "inc", "src" }
    
    links { "fo-base" }
    
    files { 
      "src/server/**.hpp", 
      "src/server/**.h", 
      "src/server/**.cpp",
      "src/server/**.rc"
    }
    
    links { "zlib" }
    includedirs { "src/zlib" }
    
    resincludedirs { "src/server" }
    
    -- MySQL
    configuration { "x32", "debug" }
      libdirs { "lib/vs2008/x32/debug/libmysql" }
      resource(nil, "lib/vs2008/x32/debug/libmysql/libmysql.dll", "libmysql.dll")
    configuration { "x32", "release" }
      libdirs { "lib/vs2008/x32/release/libmysql" }
      resource(nil, "lib/vs2008/x32/release/libmysql/libmysql.dll", "libmysql.dll")
    configuration "*"
      links { "libmysql" }
      
    links { "ws2_32" }
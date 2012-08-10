function copy(src, dst, always)
  local action
  if os.is("windows") then
    action = ""
  else
    action = "python "
  end
  
  local script = "\"" ..  path.join(os.getcwd(), "copy-data.py")  .. "\""
  src = "\"" .. src .. "\""
  dst = "\"" .. dst .. "\""
  cwd = "\"" .. os.getcwd() .. "\""
  postbuildcommands { action  .. script .. " " .. cwd .. " " .. src .. " " .. dst .. " " .. tostring(always) }
end

function resource(src, dst, always)
  if always == nil then
    always = false
  else
    always = true
  end

  copy(src, path.join("build", dst), always)
  copy(src, path.join("bin", dst), always)
end

function windows_libdir(name)
  for _,arch in pairs({"x32", "x64"}) do
    for _,conf in pairs({"debug", "release"}) do
      for _, plat in pairs({"vs2008"}) do
        local confpath = path.join(plat, path.join(arch, conf))
        
        configuration { "windows", arch, conf, plat }
          libdirs { path.join("lib", path.join(name, confpath)) }
      end
    end
  end
  configuration "*"
end

function windows_binary(basePath, dllName)
  for _,arch in pairs({"x32", "x64"}) do
    for _,conf in pairs({"debug", "release"}) do
      for _, plat in pairs({"vs2008"}) do
        local confpath = path.join(plat, path.join(arch, conf))
        local libBase = path.join("lib", path.join(basePath, confpath))
        
        configuration { "windows", arch, conf, plat }
          resource(path.join(libBase, dllName), dllName, true)
      end
    end
  end
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

solution "fonline-open-source"
  configurations { "debug", "release" }
  platforms { "x32" }
  location "build"
  
  targetdir "bin"
  --[[for _,arch in pairs({"x32", "x64"}) do
    for _,conf in pairs({"debug", "release"}) do
      for _,plat in pairs({"vs2008"}) do
        configuration { arch, conf, plat }
          targetdir(path.join("bin", path.join(plat, path.join(arch, conf))))
      end
    end
  end]]--

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
    
    resource("data", "data")
    
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
    windows_libdir("libogg")
    windows_libdir("libvorbis")
    windows_binary("libogg", "libogg.dll")
    windows_binary("libvorbis", "libvorbis.dll")
    windows_binary("libvorbis", "libvorbisfile.dll")
    links { "libogg", "libvorbis", "libvorbisfile" }
    
    -- Winsock
    configuration "windows"
      links { "ws2_32" }
  
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
    windows_libdir("libmysql")
    windows_binary("libmysql", "libmysql.dll")
    links { "libmysql" }
      
    -- Winsock
    configuration "windows"
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
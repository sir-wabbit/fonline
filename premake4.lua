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

  configuration "debug"
    defines { "DEBUG" }
    flags { "Symbols" }
  
  configuration "release"
    defines { "NDEBUG" }
    flags { "Optimize" }
  
  configuration {"debug", "x32"}
    targetdir "bin/debug"
  configuration {"release", "x32"}
    targetdir "bin/release"
    
  configuration "vs2010 or vs2008"
    defines { "_CRT_SECURE_NO_WARNINGS",
              "_CRT_NONSTDC_NO_DEPRECATE" }
    
  configuration "windows"
    defines "HAVE_VSNPRINTF"
  
  project "fonline-client"
    kind "WindowedApp"
    language "C++"
    
    flags { "WinMain" }
        
    includedirs { 
      "src/client",
      "inc"
    }
    
    files { 
      "src/client/**.hpp", 
      "src/client/**.h", 
      "src/client/**.cpp"
    }
    
    links { "zlib" }
    includedirs { "src/zlib" }
    
    -- DirectX
    includedirs { "dx8sdk/include" }
    configuration "x32"
      libdirs { "dx8sdk/lib" }
      links { "d3dx8", "d3d8", "dinput8", "dxguid", "dxerr8", "wsock32", "dsound" }
      linkoptions { "/nodefaultlib:libci.lib" }
      
    -- Ogg + Vorbis
    configuration { "x32", "debug" }
      libdirs { "lib/x32/debug/libogg", "lib/x32/debug/libvorbis" }
    configuration { "x32", "release" }
      libdirs { "lib/x32/release/libogg", "lib/x32/release/libvorbis" }
    configuration "*"
      links { "libogg", "libvorbis", "libvorbisfile" }
  
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
        
    includedirs { 
      "src/server",
      "inc"
    }
    
    files { 
      "src/server/**.hpp", 
      "src/server/**.h", 
      "src/server/**.cpp"
    }
    
    links { "zlib" }
    includedirs { "src/zlib" }
    
    -- MySQL
    configuration { "x32", "debug" }
      libdirs { "lib/x32/debug/libmysql" }
    configuration { "x32", "release" }
      libdirs { "lib/x32/release/libmysql" }
    configuration "*"
      links { "libmysql" }
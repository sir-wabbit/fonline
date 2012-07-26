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
    kind "ConsoleApp"
    language "C++"
        
    includedirs { 
      "src/client",
      "inc",
      "dx8sdk/include"
    }
    
    files { 
      "src/client/**.hpp", 
      "src/client/**.h", 
      "src/client/**.cpp"
    }
    
    links { "zlib" }
    includedirs { "src/zlib" }
    
    configuration "x32"
      libdirs { "libs", "dx8sdk/lib", "lib" }
      links { "d3dx8", "d3d8", "dinput8", "dxguid", "dxerr8", "wsock32", "dsound" }
      links { "ogg_static", "vorbisfile_static", "vorbisenc_static", "vorbis_static" }
  
  project "zlib"
    kind "StaticLib"
    language "C"
    
    files { 
      "src/zlib/**.h", 
      "src/zlib/**.c"
    }
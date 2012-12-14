//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("mapper2.res");
USEFORM("main.cpp", frmEnv);
USEUNIT("map.cpp");
USEUNIT("tileset.cpp");
USEUNIT("lists.cpp");
USEUNIT("frmset.cpp");
USEUNIT("utilites.cpp");
USEFORM("pbar.cpp", frmPBar);
USEFORM("mdi.cpp", frmMDI);
USEUNIT("frame.cpp");
USEUNIT("datfile.cpp");
USEUNIT("objset.cpp");
USEUNIT("proset.cpp");
USEUNIT("pro.cpp");
USELIB("lib\zlib10.lib");
USEUNIT("pal.cpp");
USELIB("C:\Program Files\Borland\CBuilder5\Lib\PSDK\msimg32.lib");
USEFORM("properts.cpp", frmProperties);
USEFORM("config.cpp", frmConfig);
USEUNIT("script.cpp");
USEUNIT("unlzss\unlzss.cpp");
USEUNIT("cfile\cfile.cpp");
USEUNIT("log.cpp");
USEUNIT("msg.cpp");
USELIB("C:\Program Files\Borland\CBuilder5\Lib\PSDK\ddraw.lib");
USELIB("C:\Program Files\Borland\CBuilder5\Lib\PSDK\dxguid.lib");
USEFORM("change.cpp", fmChange);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "Mapper";
                 Application->CreateForm(__classid(TfrmMDI), &frmMDI);
                 Application->CreateForm(__classid(TfrmProperties), &frmProperties);
                 Application->CreateForm(__classid(TfrmConfig), &frmConfig);
                 Application->CreateForm(__classid(TfmChange), &fmChange);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------





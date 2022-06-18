; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AppName=PatientCare
AppVersion=1.0.6.16
AppPublisher=Centaur Software
WizardStyle=modern
DefaultDirName=C:\Program Files (x86)\Common Files\Centaur Shared\PatientCare\
DefaultGroupName=PatientCareCloud Data Extractor
UninstallDisplayName=PatientCareCloud Data Extractor
UninstallDisplayIcon="{app}\PatCare.ico"
Compression=lzma2
SolidCompression=yes
OutputDir=userdocs:Inno Setup Examples Output
PrivilegesRequired=admin

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\sprokuda\pcc-binaries\PatientCareCloud.exe"; DestDir: "{app}"
Source: "C:\sprokuda\pcc-binaries\PatientCareCloud_config.exe"; DestDir: "{app}"
Source: "C:\sprokuda\pcc-binaries\PatCare.ico"; DestDir: "{app}"
Source: "C:\sprokuda\pcc-binaries\PatCareConfig.ico"; DestDir: "{app}"
Source: "C:\sprokuda\pcc-binaries\VC_redist.x86.exe"; DestDir: {app}; Flags: deleteafterinstall

[Code]
function VC2019RedistNeedsInstall(Arg: string): Boolean;
var 
  Version: String;
begin
  Version := '0';
  if RegQueryStringValue(
       HKEY_LOCAL_MACHINE,
       'SOFTWARE\WOW6432Node\Microsoft\VisualStudio\14.0\VC\Runtimes\X86',
       'Version', Version) then
    begin
      Log('VC Redist Version check : found ' + Version);
      Result := (CompareStr(Version, 'v14.30.30708.00')<0);
    end
  else
    begin
      Result := True;
    end
end;

[Icons]
Name: "{commondesktop}\PatientCareCloud_config"; Filename: "{app}\PatientCareCloud_config.exe";  IconFilename: "{app}\PatCareConfig.ico";Tasks: desktopicon
Name: "{group}\PatientCareCloud_config"; Filename: "{app}\PatientCareCloud_config.exe"; IconFilename: "{app}\PatCareConfig.ico";
Name: "{commondesktop}\PatientCareCloud"; Filename: "{app}\PatientCareCloud.exe";  IconFilename: "{app}\PatCare.ico";Tasks: desktopicon
Name: "{group}\PatientCareCloud"; Filename: "{app}\PatientCareCloud.exe"; IconFilename: "{app}\PatCare.ico";



[Run]
Filename: "{app}\VC_redist.x86.exe"; \
    Check: VC2019RedistNeedsInstall('Arg'); \
    Flags: waituntilterminated; \
    StatusMsg: "Installing VC++ x86 2019 redistributables..."





[UninstallDelete] 
Type: filesandordirs; Name: "{app}\PatientCareCloud.exe"
Type: filesandordirs; Name: "{app}\PatientCareCloud_config.exe" 
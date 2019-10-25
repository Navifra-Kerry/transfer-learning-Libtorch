Function Set-OperationsInfo()
{
    @(
        @{Name = "OpenCV-4.1.1-vc14-vc15"; ShortName = "CV-411"; Info = "Install OpenCV-4.1.1-vc14-vc15";
            Verification = @( @{Function = "VerifyDirectory"; Path = "$OpenCVBasePath"; } );
            Download = @( );
            Action = @( @{Function = "InstallExe"; Command = "$localCache\opencv-4.1.0-vc14_vc15.exe"; Param = "-y -oD:\Local2"; Message="Installing opencv ....";}); 
        }        
    )
    ,
    @{Name = "LibTorch-1.3"; ShortName = "TORCH-1.3"; Info = "Install Libtorch";
        Verification = @( @{Function = "VerifyDirectory"; Path = "$OpenCVBasePath"; } );
        Download = @( );
        Action = @( @{Function = "ExtractAllFromZip"; zipFileName = "$localCache\libtorch-win-shared-with-deps-1.3.0.zip"; destinationFolder = "D:\Local2"; } );
   }
}
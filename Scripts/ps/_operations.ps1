Function Set-OperationsInfo
{
    @(
        @{Name = "Scan System for installed programs"; ShortName = "SCANPROG"; Info = "Scan System for installed programs";
          Verification = @( @{Function = "VerifyScanPrograms" } );
          Download = @( ) ;
          Action = @() ;
         },
        @{Name = "OpenCV-4.1.1-vc14-vc15"; ShortName = "CV-411"; Info = "Install OpenCV-4.1.1-vc14-vc15";
          Verification = @( @{Function = "VerifyDirectory"; Path = "$OpenCVBasePath"; } );
          Download = @( );
          Action = @( @{Function = "InstallExe"; Command = "$localCache\opencv-4.1.0-vc14_vc15.exe"; Param = "-y -o$3rdPartyPath"; Message="Installing opencv ....";}); 
        },
        @{Name = "Libtorch 1.3 Install"; ShortName = "LIBTORCH-1.3"; Info = "Install LIBTORCH-1.3";
          Verification = @( @{Function = "VerifyDirectory"; Path = "$LibTorchPath"; } );
          Download = @( ) ;
          Action = @( @{Function = "ExtractAllFromZip"; zipFileName = "$localCache\libtorch-win-shared-with-deps-1.3.0.zip"; destinationFolder = "$3rdPartyPath"; } ) ;
        }    
    )
}

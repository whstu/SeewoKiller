@echo off

set pngfile="Origin.png"

for /d %%i in ("C:\Users\*") do (
    if exist "%%i\AppData\Roaming\Seewo\EasiNote5\Resources\Banner\" (
        ::取消文件只读，防止报错
        attrib "%%i\AppData\Roaming\Seewo\EasiNote5\Resources\Banner\Banner.png" -r

        copy %pngfile% "%%i\AppData\Roaming\Seewo\EasiNote5\Resources\Banner\Banner.png" > nul
        ::设置文件为只读
        attrib "%%i\AppData\Roaming\Seewo\EasiNote5\Resources\Banner\Banner.png" +r
        
        echo Copy to %%i\AppData\Roaming\Seewo\EasiNote5\Resources\Banner\Banner.png
    )
)

::for /d %%i in (%targetDir2%) do echo %%i
::for /r "C:\Program Files (x86)\Seewo\EasiNote5\EasiNote5_5.2.3.718\" %%i in (SplashScreen.png) do echo %%i


for /d %%j in ("C:\Program Files (x86)\Seewo\EasiNote5\EasiNote5*") do (
    if exist "%%j\Main\Assets\" (
        copy %pngfile% "%%j\Main\Assets\SplashScreen.png" > nul
        echo Copy to %%j\Main\Assets\SplashScreen.png
    )
)
::谨慎起见
for /d %%j in ("C:\Program Files\Seewo\EasiNote5\EasiNote5*") do (
    if exist "%%j\Main\Assets\" (
        copy %pngfile% "%%j\Main\Assets\SplashScreen.png" > nul
        echo Copy to %%j\Main\Assets\SplashScreen.png
    )
)
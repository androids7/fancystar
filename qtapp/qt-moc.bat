@echo off
if "%1"=="" (
	echo qt-moc [Ŀ¼��]
	goto :END
)
echo ����ִ��MOC����...
for /r .\%1 %%I in (*.h) do (
	echo making %%~dpnI_moc.cpp...
	E:\Qt\Qt5.2.1\5.2.1\msvc2010_opengl\bin\moc.exe -nn %%I -o %%~dpnI_moc.cpp
)
echo MOC�������
:END

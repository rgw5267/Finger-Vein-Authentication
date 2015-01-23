
MFCActiveXControltestps.dll: dlldata.obj MFCActiveXControltest_p.obj MFCActiveXControltest_i.obj
	link /dll /out:MFCActiveXControltestps.dll /def:MFCActiveXControltestps.def /entry:DllMain dlldata.obj MFCActiveXControltest_p.obj MFCActiveXControltest_i.obj \
		kernel32.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
.c.obj:
	cl /c /Ox /DREGISTER_PROXY_DLL \
		$<

clean:
	@del MFCActiveXControltestps.dll
	@del MFCActiveXControltestps.lib
	@del MFCActiveXControltestps.exp
	@del dlldata.obj
	@del MFCActiveXControltest_p.obj
	@del MFCActiveXControltest_i.obj

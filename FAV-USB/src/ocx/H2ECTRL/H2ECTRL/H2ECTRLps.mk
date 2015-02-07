
H2ECTRLps.dll: dlldata.obj H2ECTRL_p.obj H2ECTRL_i.obj
	link /dll /out:H2ECTRLps.dll /def:H2ECTRLps.def /entry:DllMain dlldata.obj H2ECTRL_p.obj H2ECTRL_i.obj \
		kernel32.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
.c.obj:
	cl /c /Ox /DREGISTER_PROXY_DLL \
		$<

clean:
	@del H2ECTRLps.dll
	@del H2ECTRLps.lib
	@del H2ECTRLps.exp
	@del dlldata.obj
	@del H2ECTRL_p.obj
	@del H2ECTRL_i.obj

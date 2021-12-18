# invoke SourceDir generated makefile for MultiFunctionBoard.pem4f
MultiFunctionBoard.pem4f: .libraries,MultiFunctionBoard.pem4f
.libraries,MultiFunctionBoard.pem4f: package/cfg/MultiFunctionBoard_pem4f.xdl
	$(MAKE) -f D:\ATI\CCS9\RTOS\RTOS\MultiFunctionBoard/src/makefile.libs

clean::
	$(MAKE) -f D:\ATI\CCS9\RTOS\RTOS\MultiFunctionBoard/src/makefile.libs clean


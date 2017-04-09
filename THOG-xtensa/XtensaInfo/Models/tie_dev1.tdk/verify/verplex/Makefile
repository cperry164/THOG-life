#
#Makefile for running formal equivalence check
#You must have VERPLEX installed and in your path before you run this
#
default: verplex



.SECONDARY: $(patsubst %.do,%.do.log,$(shell ls *.do 2>/dev/null))

verplex: log warnings semantics report

semantics: 

verplex_all: log warnings $(patsubst %.do,%.check,$(shell ls *.do 2>/dev/null)) report

PERL = C:/Logiciels/Xtensa/XtDevTools/install/tools/RF-2015.2-win32/XtensaTools/Tools/perl/bin/MSWin32-x86/perl.exe
XTENSA_TOOLS = C:/Logiciels/Xtensa/XtDevTools/install/tools/RF-2015.2-win32/XtensaTools/Tools
%.check: %.do ../src/verify_ref.v ../src/verify_sem.v
	@echo "Running verplex on $*" 
	XTENSA_TOOLS=$(XTENSA_TOOLS) $(PERL) -w tie_verplex -check $* | tee -a verplex.log

log:
	@rm -rf verplex.log
	@touch verplex.log

report:
	@echo "============= Report Summary ============="
	@cat verplex.log
	@rm -f *.vector.do *.fpt

clean:
	rm -rf *.log *.lck *.fpt


warnings:
	@echo "The following opcodes are not checked:RUR_reg_A, WUR_reg_A, RUR_cordic_x, WUR_cordic_x, RUR_cordic_y, WUR_cordic_y, RUR_cordic_z, WUR_cordic_z, RUR_cordic_k, WUR_cordic_k, RUR_mask_out, WUR_mask_out, RUR_ydiv2_out, WUR_ydiv2_out, RUR_xdiv2_out, WUR_xdiv2_out, tie_getbin, tie_add, tie_cordic_init, tie_cordic_iterate \n" | tee -a verplex.log


#
# 2015/04/17 created
# 2019/01/13 updated for markdown document finder to pdf
# 2019/03/16 updated for teensy3 makefile
#

DOC=teensy_makefile
DOCTYPE=markdown

PAN=pandoc

MDN=$(DOC).md
CSS=md.css
TEX=$(DOC).tex
PDF=$(DOC).pdf
HTM=$(DOC).html
OUT=$(DOC).log $(DOC).out $(DOC).aux

# test for pdf viewer...
# Mint is in the middle of changing the default viewer
XVIEW := $(shell  /bin/bash -c "command -v atril" )
ifndef XVIEW
   XVIEW := $(shell  /bin/bash -c "command -v xreader" )
endif

# .PHONY: pdf
# .PHONY: html 
.DEFAULT_GOAL:= pdf

# all: $(MDN) $(CSS)

html: $(MDN) $(CSS)
	$(PAN) -f markdown -t html -c md.css -o $(HTM) $(MDN)

pdf: $(MDN) $(CSS)
	$(PAN) -s -c md.css -f markdown -o $(PDF) $(MDN)

clean:
	rm -f $(PDF) $(OUT) $(HTM)

viewhtml:
	firefox $(HTM) 2>/dev/null &

viewpdf:
ifdef XVIEW
	${XVIEW} $(PDF) 2>/dev/null &
else
	@echo "You are SOL, no PDF viewer"
endif

# Release Notes
#
# 2019/03/16 downloaded version 2.7.1 so I could use wkhtmltopdf
# The builtin pdf version went wild on $ and # chars
# wkhtmlpdf complains but produces the pdf
#
# fix the problems with pandoc 1.12.2.1 by using code markes ~~~~
# But pdf conversions still complained about color...
# ....required install of latex-xcolor package

# 2019/01/13 pandoc now has PDF creation builtin
# pandoc -c md.css -f markdown -o df.pdf df.md 
#  can replace this:
#	 ifeq ($(DOCTYPE),markdown)
# 		pandoc -s  --latex-engine=pdflatex -o $(PDF) $(MDN) 
#	 else ifeq ($(DOCTYPE),latex)
# 		pdflatex $(TEX)
#	 else
# 		@echo "You need to define the source document type"
# 	endif

OBJS = catalog.yml \
       config.yml \
       config-starter.yml \
       ./images/02-tutorial/class.txt \
       ./contents/00-preface.re \
       ./contents/01-install.re \
       ./contents/02-tutorial.re \
       ./contents/03-syntax.re \
       ./contents/04-customize.re \
       ./contents/05-faq.re \
       ./contents/91-compare.re \
       ./contents/92-filelist.re \
       ./contents/93-background.re \
       ./contents/99-postface.re

book.pdf: front.pdf mybook.pdf back.pdf
	pdftk mybook.pdf cat 2-end output contents.pdf
	pdftk front.pdf contents.pdf back.pdf cat output book.pdf

mybook.pdf: $(OBJS)
	(cd images/02-tutorial; plantuml class.txt)
	echo $(PASSWD) | sudo -S docker run --rm -v ${PWD}:/work -w /work kauplan/review2.5 rake pdf

OBJS = config.yml \
       config-starter.yml \
       ./contents/00-preface.re \
       ./contents/01-install.re \
       ./contents/02-tutorial.re \
       ./contents/03-syntax.re \
       ./contents/04-customize.re \
       ./contents/05-faq.re \
       ./contents/06-bestpractice.re \
       ./contents/91-compare.re \
       ./contents/92-filelist.re \
       ./contents/93-background.re \
       ./contents/99-postface.re

mybook.pdf: $(OBJS)
	echo $(PASSWD) | sudo -S docker run --rm -v ${PWD}:/work -w /work kauplan/review2.5 rake pdf

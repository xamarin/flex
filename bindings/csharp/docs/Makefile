all:    html

update:
	mcs -t:library -out:lib.dll ../base.cs ../flex.cs
	mdoc update -o en lib.dll

html:
	rm -rf en.html
	monodocs2html -o en.html en

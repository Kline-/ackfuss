>fight_prog 40~
cackle
~
>greet_prog 50~
look $n
if isgood($n)
	smile $n
        say Welcome to Midgaard!
else
	mpechoaround $n $I eyes $n suspiciously.
	mpechoat $n $I eyes you suspiciously.
endif
~
>speech_prog p looking at~
growl $n
say A pesky little insect!
if rand(5)
	if isgood($n)
		mpechoaround $n $I eyes $n suspiciously.
		mpechoat $n $I eyes you suspiciously.
	else
		mpkill $n
	endif
endif
~
|

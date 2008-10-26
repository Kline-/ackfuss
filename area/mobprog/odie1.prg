> all_great_prog 100
if isnpc($n)
        look $n
        if isgood($n)
                say Hello $n
                smile $n
        else
                Say Evil Is Not Wanted in this City. 
                glare $n
        endif
endif
~
> rand_prog 50
  if rand(50)
        emote Smiles At all the people in the room. 
        else
                laugh
        else
                if rand(30)
                        c heal
                endif
 endif 
~
>act_prog p Grins
        say What are you Grining At?
~
>act_prog p Pokes you in
        look $n
        say Please Don't Poke Me $n
~
>act_prog p laughs
        chuc 
~ 
>fight_prog 70 
if rand(40)
        say You Will Never Win $n! 
        c fireball $n
        else
                if rand(50)
                        say What Is Your Problem!! 
                        c fireball $n
                endif 
endif
~
|


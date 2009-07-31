local ch = mud.char_info()
rsend(ch.name)

wait.make( function ()
 wait.pause(3)
 rsend("Three!")
 end
)

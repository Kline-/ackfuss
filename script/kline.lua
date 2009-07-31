rsend("See if it explodes!")
send("Testing")

require "tprint"
send("tprint loaded")

local ch = mud.char_info()

if ch.name == "Kline" then
 send("Hi Kline!")
else
 send("You're not Kline!")
end

if ch.npc == 0 then
 fsend("Your title is: %s",ch.pcdata.title)
end

frsend("%s",ch.name)

wait.make( function ()
 send("Testing wait...")
 wait.pause(3)
 send("Three seconds!")
 wait.pause(10)
 send("Ten seconds!")
 end
)

--tprint(ch)

send("Testing")

require "tprint"
send("tprint loaded")

local ch = mud.char_info()
tprint(ch)

if ch.name == "Kline" then
 send("Hi Kline!")
else
 send("You're not Kline!")
end

if ch.npc == 0 then
 fsend("Your title is: %s",ch.pcdata.title)
end

frsend("%s",ch.name)

send("Testing")
local ch = mud.char_info()
if ch.name == "Kline" then
 send("Hi Kline!")
else
 send("You're not Kline!")
end

package.path = "/home/matt/muds/active/ackfuss/script/?.lua;" .. package.path

-- send to player - with newline
function send (...)
  mud.send_to_char (table.concat {...} .. "\r\n")
end

-- formatted send to player - with newline
function fsend (s, ...)
  mud.send_to_char (string.format (s, ...) .. "\r\n")
end

-- send to player - without newline
function nsend (...)
  mud.send_to_char (table.concat {...})
end

-- send to room
function rsend (...)
  mud.recho (table.concat {...})
end

-- formatted send to room
function frsend (s, ...)
  mud.recho (string.format (s, ...))
end

require "tprint"

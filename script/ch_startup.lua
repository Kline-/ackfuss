package.path = "/home/matt/workspace/ackfuss/script/?.lua;" .. package.path
require "wait"

-- send to player - with newline
function send (...)
  mud.send_to_char (table.concat {...} .. "\r\n")
end

-- send to player - without newline
function nsend (...)
  mud.send_to_char (table.concat {...})
end

-- formatted send to player - with newline
function fsend (s, ...)
  mud.send_to_char (string.format (s, ...) .. "\r\n")
end

-- formatted send to player - without newline
function nfsend (s, ...)
  mud.send_to_char (string.format (s, ...))
end

-- send to room - with newline
function rsend (...)
  mud.recho (table.concat {...} .. "\r\n")
end

-- send to room - without newline
function nrsend (...)
  mud.recho (table.concat {...})
end

-- formatted send to room - with newline
function frsend (s, ...)
  mud.recho (string.format (s, ...) .. "\r\n")
end

-- formatted send to room - without newline
function frsend (s, ...)
  mud.recho (string.format (s, ...))
end

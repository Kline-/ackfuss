package.path = "/home/matt/workspace/ackfuss/script/?.lua;" .. package.path
require "wait"

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

-- send to room
function rsend (...)
  mud.recho (table.concat {...})
end

-- formatted send to room
function frsend (s, ...)
  mud.recho (string.format (s, ...))
end

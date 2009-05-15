-- send to player - with newline
function send (...)
  mud.send_to_char (table.concat {...} .. "\r\n")
end -- send

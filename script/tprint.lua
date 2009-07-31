--
--  tprint.lua

--[[

For debugging what tables have in them, prints recursively

See forum thread:  http://www.gammon.com.au/forum/?id=4903

eg.

require "tprint"

tprint (some_table)

--]]

module("tprint", package.seeall)

function print (t, indent, done)
  -- show strings differently to distinguish them from numbers
  local function show (val)
    if type (val) == "string" then
      return '"' .. val .. '"'
    else
      return tostring (val)
    end -- if
  end -- show
  if type (t) ~= "table" then
    send ("tprint got " .. type (t))
    return
  end -- not table
  -- entry point here
  done = done or {}
  indent = indent or 0
  for key, value in pairs (t) do
    nsend (string.rep (" ", indent)) -- indent it
    if type (value) == "table" and not done [value] then
      done [value] = true
      send (show (key), ":");
      tprint (value, indent + 2, done)
    else
      nsend (show (key), "=")
      send (show (value))
    end
  end
end

return tprint

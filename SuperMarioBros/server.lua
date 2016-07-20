 --[[
 @file write_joypad.lua
 @author Marcus Edel

 Definition of server/client routines.
 --]]

local socket = require("socket");

local S = {};

-- Server socket.
tcp_server = 0

-- Server socket error.
err = 0

-- Connected client socket.
client = 0

-- Function to create a socket using the given host and port.
-- @param host List on this host.
-- @param port Listen on this port.
-- @backlog The number of client connections that can be queued waiting for service.
function Server(host, port, backlog)
  tcp_server, err = socket.tcp();
  if tcp_server==nil then
    return nil, err;
  end

  tcp_server:setoption("reuseaddr", true);
  local res, err = tcp_server:bind(host, port);
  if res==nil then
    return nil, err;
  end

  res, err = tcp_server:listen(backlog);
  if res==nil then 
    return nil, err;
  end

  return tcp_server;
end

local function Accept()
  client = tcp_server:accept()

  return client;
end

-- Function to send data over the socket.
-- @param data The data to be send over the socket.
local function Send(data)
  if client ~= nil then
    client:send(data.."\r\n\r\n\r\n")
    io.flush()
  end
end

-- Function to receive data over the socket.
-- @param data The data received over the socket.
local function Receive()
  if client ~= nil then
    return client:receive()
  end

  return nil
end

S.Server = Server;
S.Accept = Accept;
S.Send = Send;
S.Receive = Receive;

return S
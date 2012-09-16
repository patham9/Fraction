<?php
/*****************************************************************************
 * PHP Multi-Client TCP Socket Fraction Server                               *
 *****************************************************************************
 * Will listen on a given socket for TCP connections, echoing whatever data  *
 *  is sent to that socket and send it to all clients                        *
 *                                                                           *
 * Original script by KnoB in a comment in the PHP documentation:            *
 *  http://www.php.net/manual/en/ref.sockets.php#43155                       *
 *                                                                           *
 * Heavily modified and commented by Andrew Gillard                          *
 * Modified by Patrick Hammer for Fraction                                   *
 *****************************************************************************/
$address = '91.203.212.130'; //127.0.0.1
$port = 10000;
$players=2;
$cnt_players=0;
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
 "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<title>GameOfGods Multi-Client Server</title>
	<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
	<style type="text/css">
	div.error{
		padding: 0px 10px 5px 10px;
		background-color: #f88;
		border: 1px solid #f00;
	}
	div.error span{
		font-family: monospace;
	}
	</style>
</head>
<body>
 
<?php
set_time_limit(0);
ob_implicit_flush();
ignore_user_abort(true);
function socketError($errorFunction, $die=false)
{
	$errMsg = socket_strerror(socket_last_error());
	echo <<<EOHTML
<div class="error">
<h1>$errorFunction() failed!</h1>
<p>
	<strong>Error Message:</strong>
	<span>$errMsg</span>
</p>
<p>You may have to wait a few seconds for the old server to release its listening port.</p>
</div>
EOHTML;
	if($die)
		die('</body></html>');
}
if(!($server = @socket_create(AF_INET, SOCK_STREAM, SOL_TCP)))
{
	socketError('socket_create', true);
}
socket_set_option($server, SOL_SOCKET, SO_REUSEADDR, 1);
if(!@socket_bind($server, $address, $port))
{
	socketError('socket_bind', true);
}
if(!@socket_listen($server))
{
	socketError('socket_listen', true);
}
$allSockets = array($server);
$LAST_STEP="";
while(true)
{
    echo ' ';
    if(connection_aborted())
    {
    	foreach($allSockets as $socket)
    	{
			if($socket!=NULL)
				socket_close($socket);
		}
		break;
	}
	$changedSockets = $allSockets;
	$STEP=date("s");
	if(strcmp($STEP,$LAST_STEP) && $cnt_players==$players) // !=  World step - Patrick Hammer
	{
		echo "step<br/>";
		foreach($allSockets as $socket)
		{
			try
			{
				/*if($socket!=NULL && $socket!=$server)
				{
					$stepbuf="1111111111111111111111111";
					socket_write($socket,$stepbuf,20);
					flush();
					ob_flush();
				}*/
			}
			catch(Exception $e){}
		}
	}
	$LAST_STEP=$STEP;
	socket_select($changedSockets, $write = NULL, $except = NULL, 5);
	foreach($changedSockets as $socket)
	{
	    if($socket == $server)
	    {
	        if(!($client = @socket_accept($server)))
	        	socketError('socket_accept', false);
	        else
	        if($cnt_players<$players)
	        {
	        	$allSockets[] = $client;
				$cnt_players++;
	        }
	    }
	    else
	    {
	        $data = socket_read($socket, 20);
	        if($data === false || $data === '')
	        {
            	//socket_read() returned FALSE, meaning that the client has closed the connection.
	            unset($allSockets[array_search($socket, $allSockets)]);
	            socket_close($socket);
	        }
	        else
	        {
				//We got useful data from socket_read(), so let's echo it.
				$stru_req=" req: "+(string)intval($data[0]+$data[1]+$data[2]+$data[3]);
				$stru_cli=" cli: "+(string)intval($data[4]+$data[5]+$data[6]+$data[7]);
				$stru_mk=" mk: "+(string)intval($data[8]+$data[9]+$data[10]+$data[11]);
				$stru_mx=" mx: "+(string)intval($data[12]+$data[13]+$data[14]+$data[15]);
				$stru_my=" my: "+(string)intval($data[16]+$data[17]+$data[18]+$data[19]);
				echo "\r\n<p><strong>&middot;</strong> $socket wrote: $stru_req $stru_cli $stru_mk $stru_mx $stru_my </p>"; //$data
				//but also write it to all clients - Patrick Hammer
				foreach($allSockets as $socket)
				{
					try
					{
						if($socket!=NULL && $socket!=$server)
						{
							socket_write($socket,$data,20);
							flush();
							ob_flush();
						}
					}
					catch(Exception $e){}
				}
	        }
	    }
	}
}
?>
</body>
</html>

var webSocket   = null;
var ws_protocol = "ws";
var ws_hostname = "localhost";
var ws_port     = "";
var ws_endpoint = "";

var data = String();


/**
 * Update cursor in textarea
 * id = incomingMsgOutput
 */
function update_cursor(element)
{
    console.log('ELEMENT ON CHANGE - TEXTAREA: ', element);

    element.focus();
    element.setSelectionRange(element.value.length, element.value.length, "forward");
}


/**
 * Event handler for clicking on button "Connect"
 * id = btnConnect
 */
function onConnectClick()
{
    openWSConnection(ws_protocol, ws_hostname, ws_port, ws_endpoint);
}


/**
 * Event handler for clicking on button "Disconnect"
 * id = btnDisconnect
 */
function onDisconnectClick()
{
    webSocket.close();
}


/**
 * Open a new WebSocket connection using the given parameters
 */
function openWSConnection(protocol, hostname, port, endpoint)
{

    var webSocketURL = null;
    webSocketURL = protocol + "://" + hostname + ":" + port + endpoint;
    console.log("openWSConnection::Connecting to: " + webSocketURL);

    try
    {
        webSocket = new WebSocket(webSocketURL);

        webSocket.onopen = function(openEvent)
        {
            console.log("WebSocket OPEN: " + JSON.stringify(openEvent, null, 4));
            document.getElementById("btnPlot").disabled       = false;
            document.getElementById("btnConnect").disabled    = true;
            document.getElementById("btnDisconnect").disabled = false;
        };


        webSocket.onclose = function (closeEvent)
        {
            console.log("WebSocket CLOSE: " + JSON.stringify(closeEvent, null, 4));
            document.getElementById("btnPlot").disabled       = true;
            document.getElementById("btnConnect").disabled    = false;
            document.getElementById("btnDisconnect").disabled = true;
        };


        webSocket.onerror = function (errorEvent)
        {
            console.log("WebSocket ERROR: " + JSON.stringify(errorEvent, null, 4));
        };


        webSocket.onmessage = function (messageEvent)
        {

            var wsMsg = messageEvent.data;
            console.log("WebSocket MESSAGE: " + wsMsg);

            if (wsMsg.indexOf("error") > 0)
            {
                document.getElementById("incomingMsgOutput").value += "\nERROR: " + wsMsg.error + "\r\n";
            }
            else
            {
                document.getElementById("incomingMsgOutput").value += "\nINFO:\n\n" + wsMsg + "\r\n";

                data = wsMsg;
                window.onload = render_graph();
            }

        };
    }
    catch (exception)
    {
        console.error(exception);
    }
}


/**
 * Render graph by dagreD3
 */
function render_graph()
{
    console.log(data);

    var g = graphlibDot.parse(data);

    // Render the graphlib object using d3.
    var renderer = new dagreD3.Renderer();

    renderer.run(g, d3.select("svg g"));

    var svg = document.querySelector('#graphContainer');

    var bbox = svg.getBBox();
    svg.style.width = bbox.width + 50 + "px";
    svg.style.height = bbox.height + 50 + "px";
}


/**
 * Render options table by handling options button click event
 */
function onOptionsclick()
{
    var panel = document.getElementsByClassName("options_panel")[0];

    console.log(panel);

    if (panel.style.display === "block")
    {
        panel.style.display = "none";
    }
	
	panel.style.display = "block";
}

/**
 * Render options table by handling options button click event
 */
function onApplyPortClick()
{
    var port = document.getElementById("port").value;
    ws_port = port;
}

/**
 * Send a message to the WebSocket server
 */
function onPlotClick(element)
{
    if(webSocket.readyState !== WebSocket.OPEN)
    {
        console.error("webSocket is not open: " + webSocket.readyState);
    }
    else
    {

        var matrix_dim = document.getElementById("matrix_dim").value;
        var characters_length = document.getElementById("nodes_dim").value;
        var metrics = document.getElementById("metrics_dim").value;

        console.log('MATRIX DIM: ', matrix_dim);
        console.log('CHARACTERS LEN: ', characters_length);
        console.log('METRICS: ', metrics);

        var payload = '{"matrix_dim": ' + matrix_dim +
                      ', "characters_length": ' + characters_length +
                      ', "metrics": ' + metrics + '}';

        console.log('STRING PAYLOAD IN SEND EVENT: ', payload);

        webSocket.send(payload);

        update_cursor(element);

    }
}
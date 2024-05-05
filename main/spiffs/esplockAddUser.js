var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
var rfidStatus = false;

window.addEventListener('load', onLoad);

function onLoad(event) {
    initWebSocket();
    initButtons();
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection...')
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage;
}

function initButtons(){
    document.getElementById('RFID').addEventListener('click', getRFID)
}

function onOpen(event) {
    console.log('Connection opened');
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}
function onMessage(event) {
    console.log(event.data)
    console.log(rfidStatus)
    checkData = event.data.split("=")
    if (event.data == 'unavailable'){
        document.getElementById('tag').value = ""
        alert('TAG já cadastrada')
    }
    else if (event.data == 'success#add'){
        document.getElementById('user').value = ""
        document.getElementById('tag').value = ""
        alert('Usuario adicionado')
    }
    else if (checkData[0] == 'newUserId' && rfidStatus == true){
        if (checkData[1] != '0'){
            document.getElementById('tag').value = checkData[1]
            document.getElementById('RFID').innerHTML = 'Ler TAG RFID'
            rfidStatus = false
        }
        else{
            websocket.send('readRFID')
        }
    }
    else if (event.data == 'cancel'){
        document.getElementById('RFID').innerHTML = 'Ler TAG RFID'  
        document.getElementById('tag').value = ""
        rfidStatus = false
    }
}

function getRFID(){
    rfidStatus = !rfidStatus    
    if(rfidStatus == true){
        document.getElementById('RFID').innerHTML = 'Aguardando TAG...'
        websocket.send('loopRFID')
        websocket.send('readRFID')
    }
    if(rfidStatus == false){
        websocket.send('cancelRFID')
    }
}

function sendInfo(event){
    console.log(event)
    var message = "addUser:User=" + event.srcElement[0].value + ";ID=" + event.srcElement[1].value
    console.log("Form submitted: User:" + event.srcElement[0].value + "; ID:" + event.srcElement[1].value)
    websocket.send(message)
}
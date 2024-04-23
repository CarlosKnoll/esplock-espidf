var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
var usersArray;
var numPage;
var oldestID;
var maxPage;

window.addEventListener('load', onLoad);

function onLoad(event) {
    numPage=1;
    initWebSocket();
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage;
}
function onOpen(event) {
    websocket.send('populateAccess;numPage=' + numPage);
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

function onMessage(event) {
    console.log(event.data)
    let data = event.data;
    checkData = data.split("#")
    console.log(checkData)
    if (checkData[1] == 'empty'){
        location.reload();
    }
    else{
        if (checkData[0] == 'access'){
            temp = checkData[1].split("=");
            accessArray = temp[2].split(";");
            temp = temp[1].split(";")
            oldestID = temp[0];
            populateTable();
            dataControl();
        }
        if (checkData[0] == 'updateAccess'){
            alert('Histórico de acessos modificado!');
            numPage = 1;
            websocket.send('populateAccess;numPage=' + numPage);
        }
        if (checkData[0] == 'csv'){
            window.location.href = 'access.csv';
        }
    }
}

function populateTable(){
    var table = document.getElementById('accessTable').tBodies[0];;
    table.innerHTML = "";
    if(accessArray != ''){
        accessArray.forEach(element => {
            const currentUser = element.split(",");
            var tr = document.createElement('tr');
            tr.id = currentUser[0];
            if (currentUser[0] == oldestID){
                maxPage = true;
            }
            else{
                maxPage = false;
            }
            tr.innerHTML = '<td>' + currentUser[1] + '</td>' +
            '<td>' + currentUser[2] + '</td>' +
            '<td>' + currentUser[3] + '</td>' + 
            '<td>' + currentUser[4] + '</td>' 
            table.appendChild(tr);
        })
    }
}

function dataControl(){
    var tr = document.getElementById('pageControl');
    var html = "";
    tr.innerHTML = "";
    console.log(numPage);
    if (numPage == 1){
        html = '<td class="no-border"><button class="button data-button-deactivate">&#60</button></td>'
    }
    else{
        html = '<td class="no-border"><button onclick="pageMinus()" class="button data-button">&#60</button></td>'
    }
    html += '<td class="border" id="numPage">' + numPage + '</td>'
    if (maxPage == false){
        html += '<td class="no-border"><button onclick="pagePlus()" class="button data-button">&#62</button></td>'
    }
    else{
        html += '<td class="no-border"><button class="button data-button-deactivate">&#62</button></td>'
    }
    tr.innerHTML = html
}

function pageMinus(){
    if (numPage != 1){
        numPage = numPage - 1;
        updateData();
    }
}

function pagePlus(){
    numPage = numPage + 1;
    updateData();
}

function updateData(){
    var td = document.getElementById('numPage');
    td.innerHTML = numPage;
    websocket.send('populateAccess;numPage=' + numPage);
}

function clearDB(){
    if(confirm('Confirma que deseja limpar os dados do banco de dados?')){
        websocket.send('clear')
    }
}

function getDB(){
    websocket.send('get')
}

function clearCSV(){
    websocket.send('clearCSV')
}
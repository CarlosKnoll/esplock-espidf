var ip
var gateway
var newLabel
var newData
var count = 0
var data=[]
var oldData=0
var y=0

function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage;
}
function onOpen(event) {
    console.log('Success')
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

function onMessage(event) {
    const obj = JSON.parse(event.data)
    const value = (JSON.stringify(obj.values).replace('[','')).replace(']','');
    //const timeMS = JSON.stringify(obj.timestamp)
    //const time = new Date()
    //time.setUTCMilliseconds(parseInt(timeMS)/10000000000)
    //newLabel = time.getDate() + '/' + time.getMonth() + '-' + time.getHours() + ':' + time.getMinutes()
    newData = parseInt(value)

    myPlotter.plotLine(y, oldData, y+10, newData, "red");
    console.log(oldData, y, newData, y+10)

    oldData = newData
    y = y+10

    // count = count + 1
    // if (count > 14){
    //     //console.log("count>14")
    //     //data = data.slice(data.length - 10, data.length);
    //     count = 0
    // }
    // else{
    //     //console.log("count<14")
    // }
    //console.log(newLabel, newData)


}  

let myPlotter = new XYPlotter("myCanvas");
myPlotter.transformXY();

// Plotter Object
function XYPlotter(id) {

this.canvas = document.getElementById(id);
this.ctx = this.canvas.getContext("2d");
this.xMin = 0;
this.yMin = 0;
this.xMax = this.canvas.width;
this.yMax = this.canvas.height;

// Plot a Line Function
this.plotLine = function(x0, y0, x, y, color) {
  this.ctx.moveTo(x0, y0);
  this.ctx.lineTo(x, y);
  this.ctx.strokeStyle = color;
  this.ctx.stroke();
  }

this.transformXY = function() {
  this.ctx.transform(1, 0, 0, -1, 0, this.canvas.height)
  }

}

function sendInfo(event){
    ip = event.srcElement[0].value
    gateway = `ws://` + ip + `:8080/sensor/connect?type=android.sensor.light`;
    initWebSocket();
}
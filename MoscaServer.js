//Mosca
const mosca = require('mosca')
const settings = {
port: 1883
};
//here we start mosca
const moscaServer = new mosca.Server(settings);
moscaServer.on('ready', setup);
// fired when the mqtt server is ready
function setup() {
console.log('Mosca server is up and running in port 1883!')
}
// fired whena  client is connected
moscaServer.on('clientConnected', function(client) {
console.log('client connected', client.id);
});
// fired when a message is received
moscaServer.on('published', function(packet, client) {
    if (packet.topic == '/example') {
      console.log('\n Message arrived to the mosca broker server: ');
      console.log(packet.payload.toString('utf-8'));
    }
});
// fired when a client subscribes to a topic
moscaServer.on('subscribed', function(topic, client) {
console.log('subscribed : ', topic);
});
// fired when a client subscribes to a topic
moscaServer.on('unsubscribed', function(topic, client) {
console.log('unsubscribed : ', topic);
});
// fired when a client is disconnecting
moscaServer.on('clientDisconnecting', function(client) {
console.log('clientDisconnecting : ', client.id);
});
// fired when a client is disconnected
moscaServer.on('clientDisconnected', function(client) {
console.log('clientDisconnected : ', client.id);
});
//Mosca
const chalk = require('chalk');
const mosca = require('mosca');

const settings = {
port: 1883
};
//here we start mosca
const moscaServer = new mosca.Server(settings);
moscaServer.on('ready', setup);
// fired when the mqtt server is ready
function setup() {
console.log(chalk.green('Mosca server is up and running in port 1883!'));
}
// fired whena  client is connected
moscaServer.on('clientConnected', function(client) {
console.log(chalk.yellow('client connected', client.id));
});
// fired when a message is received
moscaServer.on('published', function(packet, client) {
    if (packet.topic == '/data/temp' || packet.topic == '/data/motion') {
      console.log(chalk.green('\n Message arrived to the mosca broker server: '));
      console.log(chalk.blue(packet.payload.toString('utf-8')));
    }
});
// fired when a client subscribes to a topic
moscaServer.on('subscribed', function(topic, client) {
console.log(chalk.yellow('subscribed : ', topic));
});
// fired when a client subscribes to a topic
moscaServer.on('unsubscribed', function(topic, client) {
console.log(chalk.red('unsubscribed : ', topic));
});
// fired when a client is disconnecting
moscaServer.on('clientDisconnecting', function(client) {
console.log(chalk.red('clientDisconnecting : ', client.id));
});
// fired when a client is disconnected
moscaServer.on('clientDisconnected', function(client) {
console.log(chalk.red('clientDisconnected : ', client.id));
});
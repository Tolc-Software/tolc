const loadmyLib = require('./build/tolc/myLib');

loadmyLib().then(myLib => {
	console.log("Expecting a number:");
	console.log(myLib.myLib.getNumber());
});

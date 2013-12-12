// This function is called by common.js when the NaCl module is loaded.
function moduleDidLoad() {
  // Once we load, hide the plugin. In this example, we don't display anything
  // in the plugin, so it is fine to hide it.
  common.hideModule();

  // After the NaCl module has loaded, common.naclModule is a reference to the
  // NaCl module's <embed> element.
  //
  // postMessage sends a message to it.
  common.naclModule.postMessage('Please calculate the homography.');
}

// This function is called by common.js when a message is received from the
// NaCl module.
function handleMessage(message) {

  // Dump stuff to special PNaCl output area.
  var logEl = document.getElementById('log');
  logEl.textContent += message.data;

  // Or to JS Console.
  console.log(message.data);
}

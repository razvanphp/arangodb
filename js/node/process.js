/*jshint esnext: true, strict: false, unused: false, -W051: true */
/*global require, module, exports:true */
var EventEmitter = require('events').EventEmitter;
exports = module.exports = new EventEmitter();
exports.env = require("internal").env;
exports.argv = [];
exports.stdout = {
  isTTY: false,
  write: function (text) {
    require('console').infoLines(text);
  }
};
exports.nextTick = function (fn) {
  fn();
};
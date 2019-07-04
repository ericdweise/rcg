//filename: routes.js

var express = require('express');
var router = express.Router();

const ctrl = require('../controllers/index.js');

router.get('/', function(req, res){
  req.query.database = 'sensors';
  ctrl.listTables(req, res);
});

router.get('/postReading', function(req, res){
  ctrl.logData(req, res);
});

router.get('/postLog', function(req, res){
  ctrl.logMessage(req, res);
});

router.get('/tables', function(req, res){
  ctrl.listTables(req, res);
});

router.get('/retrieveTable', function(req, res){
  ctrl.retrieveTable(req, res);
});

router.get('/retrieveTableDates', function(req, res){
  ctrl.retrieveTableDates(req, res);
});

module.exports = router;

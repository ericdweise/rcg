//filename: queryController.js
let bodyparser = require('body-parser');
const dbUtilities = require('../db/db.js');

//function to create and do arithmetic on date objects
datePlus = function(rawDate, days){
  var dateParts = rawDate.split("-");
  var day = parseInt(dateParts[2]);
  var dur = parseInt(days);
  var newDay = day + dur;
  dateParts[2] = newDay;
  var dateObjectEnd = (dateParts[0] + "-" + dateParts[1] + "-" + dateParts[2]);
  return dateObjectEnd;
}

//function to extract the complete set of dates from a set of data
skimDates = function(array){
  let dates = [];
  for(var i = array.length - 1; i >= 0; i--){
    const dateRaw = JSON.stringify(array[i].datetime);
    let date = dateRaw.substring(1, 11);
    if(dates.indexOf(date) == -1){
      dates.push(date);
    }
  }
  return dates;
}

//defnining the actuall controller and its methods
let controller = {
  retrieveTables: async function(req, res){
    const database = req.params.database;
    const query = [
      'SHOW TABLES FROM ' +database +';',
    ];
    const results = await connectDb(query, database);
    const tables = results[0];
    var sensors = [];
    for(var i = 0; i < tables.length; i++){
      sensors.push(tables[i].Tables_in_sensors);
    }
      res.send({
        sensors: sensors
      });
  },

  retrieveTable: async function(req, res){
    const database = req.params.database;
    const table = req.params.table;
    const query = [
      'SELECT * FROM ' +table +';'
    ];
    const view = 'dashboard'; 
    const results = await connectDb(query, database);
    let resultsTrimmed = [];
    if(results[0].length > 500){
      for(var i = results[0].length; i > 0; i--){
        if(i % 10 == 0){
          resultsTrimmed.push(results[0][i]);
        }
      }
    } else{
        resultsTrimmed = results[0].reverse();
    }
      res.send({
        data: resultsTrimmed,
      });
  },

  retrieveTableDates: async function(req, res) {
    const database = req.params.database;
    const table = req.params.table;
    const start = req.query.start;
    const days = req.query.days;
    const end = datePlus(start, days);
    const query = [
      "SELECT * FROM " +table +" WHERE datetime BETWEEN '" +start + "' AND '" +end +"';"
    ];
    
    const results = await connectDb(query, database);
    var resultsTrimmed = [];
    if(results[0].length > 500){
      for(var i = results[0].length - 1; i >= 0; i--){
        if(i % 10 == 0){
          resultsTrimmed.push(results[0][i]);
        }
      }
    } else{
        resultsTrimmed = results[0].reverse();
    }
      res.send(resultsTrimmed);
  },

  retrieveTableIds: async function(req, res) {
    const database = req.params.database;
    const table = req.params.table;
    const start = req.query.start;
    const end = req.query.end;
    const query = [
      "SELECT * FROM " +table +" WHERE id BETWEEN " +start +" AND " +end +";"
    ];
    const view = 'dashboard'
    
    const results = await connectDb(query, database);
    const tables = results[0];
    var resultsTrimmed = [];
    if(results[0].length > 500){
      for(var i = results[0].length - 1; i >= 0; i--){
        if(i % 10 == 0){
          resultsTrimmed.push(results[0][i]);
        }
      }
    } else{
        resultsTrimmed = results[0].reverse();
    }
      res.send(resultsTrimmed);
  }
}

module.exports = controller;
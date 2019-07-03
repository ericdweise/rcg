var mysql = require('mysql');

//function to retrieve the names of all the tables in the database
retrieveTables = function(req, res, database) {

var connection = mysql.createConnection({
  host : 'localhost',
  user : 'rcg',
  password: '8693ViaMallorca',
});

//opening the connection to the database
  connection.connect(function(err) {
    if(err) {
      console.error('error connecting: ' + err.stack);
      return;
    }
    console.log('connected as id ' + connection.threadID);
  });

// where the query string begins
  connection.query('SHOW TABLES FROM ' + database + ';', function(error, results, fields) {
    if(error) {
      console.error('error retrieving the table names');
      return;
    }
    console.log('successfully retrieved tables' + results);
    res.send(results);
  });
//end of the query strings

//closing the connection to the database
  connection.end(function(err) {
    if(err) {
      console.error('error terminating connection');
      return;
    }
    console.log('connection successfully terminated');
  });
}

//function to retrieve a specific tables and its rows
retrieveTable = function(req, res, database, table) {

var connection = mysql.createConnection({
  host : 'localhost',
  user : 'rcg',
  password: '8693ViaMallorca',
  database: req.query.database
});

//opening the connection to the database
  connection.connect(function(err) {
    if(err) {
      console.error('error connecting: ' + err.stack);
      return;
    }
    console.log('connected as id ' + connection.threadID);
  });

// where the query string begins
  connection.query('SELECT * FROM ' + table + ';', function(error, results, fields) {
    if(error) {
      console.error('error retrieving the table names');
      return;
    }
    console.log('successfully retrieved tables' + results);
    res.send(results);
  });
//end of the query strings

//closing the connection to the database
  connection.end(function(err) {
    if(err) {
      console.error('error terminating connection');
      return;
    }
    console.log('connection successfully terminated');
  });
}
module.exports = retrieveTables, retrieveTable;

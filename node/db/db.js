var mysql = require('mysql');

//generalized utility db functions
//function to open a db connection

//opening the connection to the database
connectDb = function(query, database){  
resultData = [];
  var connectionPromise = dbConnect(database);
    connectionPromise.then(function(connection){
      for(var i = 0; i < query.length; i++){
        var queryPromise = buildQuery(connection, query[i]);
        queryPromise.then(function(result){
          console.log('QUERY' + i + ': ' + query[i]);
          console.log(result);
          resultData.push(result);
          if(resultData.length == query.length){
            dbDisconnect(connection);
            return(resultData);
          }
        })
      }
    });

//opening the db connection
function dbConnect(database){
  return new Promise(function(resolve, reject){
    var connection = mysql.createConnection({
      host : 'localhost',
      user : 'rcg',
      password: '8693ViaMallorca',
      database: database
    });

    connection.connect(function(err) {
      if(err) {
        console.error('error connecting: ' + err.stack);
        reject(error);
      }
      else{
        console.log('connected as id ' + connection.threadID);
        resolve(connection);
      }
    });
  });
}
    
//building the query promise function
function buildQuery(connection, query){
  return new Promise(function(resolve, reject){
    connection.query(query, function(error, results, fields){
      if(error){
        console.error('error retrieving the query');
        console.log(query);
        reject(error);
      }
      else{
        resolve(results);
      };
    });
  });
}

//closing the connection to the database
function dbDisconnect(connection){
  connection.end(function(err) {
    if(err) {
      console.error('error terminating connection');
      return(error);
    }
    else{
      return(console.log('connection successfully terminated'));
    }
  });
}

}
module.exports = connectDb;

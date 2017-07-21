/* GET 'reporting' pages */
module.exports.reporting = function(req, res) {
  res.render('reporting', { title: 'Aquaponics Data' });
};

module.exports.fish = function(req, res) {
  res.render('fish', { title: 'Fish Tank Reporting' });
};

module.exports.biofilter = function(req, res) {
  res.render('biofilter', { title: 'Biofilter Reporting' });
};

module.exports.reservoir = function(req, res) {
  res.render('reservoir', { title: 'Plant Reservoir Reporting' });
};

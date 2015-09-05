var max = 293;
var min = 35;
var tmp = [];
var res = [];

for (var i = 0; i < 30; i++) {
  var num = parseInt((Math.random()*(max-min)) + min);
  tmp.push(num);
  res = tmp.sort(function(a, b) {
    return b - a;
  });
}

for (var i = 0; i < res.length; i++) {
  console.log(res[i] + ',');
}



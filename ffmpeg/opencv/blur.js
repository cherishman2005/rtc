/*
// Include gm library
var gm = require('gm');
  
// Import the image
gm('gfg.png')
  
// Invoke Blur function with blur radius as 12
.blur(10)
  
// Process and Write the image
.write("blur1.png", function (err) {
  if (!err) console.log('done');
});
*/


// Include gm library
var gm = require('gm');
  
// Import the image
gm('gfg.png')
  
// Invoke shear function with 
// xDegrees as 45 and yDegrees as 90
.rotate("#545651", 78)
  
// Invoke blur function with radius
// as 5 and sigma as 5 
.blur(5, 5)
  
// Process and Write the image
.write("blur2.png", function (err) {
    if (!err) console.log('done');
});

//https://gist.githubusercontent.com/asimmittal/dab727f67b1dc791604b35bc2933ca97/raw/7251316b2a98f19b35d23f9494762c312c5a8c11/sequentialLoad.js
(function(){
  
  //three JS files that need to be loaded one after the other
  var libs = [
    'https://code.jquery.com/jquery-3.1.1.min.js',
    'https://cdnjs.cloudflare.com/ajax/libs/jquery-easing/1.4.1/jquery.easing.min.js',
    'https://cdnjs.cloudflare.com/ajax/libs/underscore.string/3.3.4/underscore.string.js'
  ];

  var injectLibFromStack = function(){
      if(libs.length > 0){

        //grab the next item on the stack
        var nextLib = libs.shift();
        var headTag = document.getElementsByTagName('head')[0];

        //create a script tag with this library
        var scriptTag = document.createElement('script');
        scriptTag.src = nextLib;

        //when successful, inject the next script
        scriptTag.onload = function(e){
          console.log("---> loaded: " + e.target.src);
          injectLibFromStack();
        };

        //append the script tag to the <head></head>
        headTag.appendChild(scriptTag);
        console.log("injecting: " + nextLib);
      }
      else return;
  }

  //start script injection
  injectLibFromStack();
})();

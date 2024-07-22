// A $( document ).ready() block.
$( document ).ready(renderMathInElement()) {
    console.log( "ready!" );
          document.body,
          {
              delimiters: [
                  {left: "$$", right: "$$", display: true},
                  {left: "\\[", right: "\\]", display: true},
                  {left: "$", right: "$", display: false},
                  {left: "\\(", right: "\\)", display: false}
              ]
          }
});

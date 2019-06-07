
let canvas = document.getElementById("canvas");

const W = 320;
const H = 240;


// Resize function
let resize = function(w, h) {

    let c = this.canvas;

    // Find the best multiplier for
    // square pixels
    let hratio = (w / W) | 0;
    let vratio = (h / H) | 0;
    let mul = Math.min(hratio, vratio);
    
    // Compute properties
    let width, height, x, y;
    width = W * mul;
    height = H * mul;
    x = w/2 - width/2;
    y = h/2 - height/2;

    // Set style properties
    let top = String(y | 0) + "px";
    let left = String(x | 0) + "px";
    c.style.height = String(height | 0) + "px";
    c.style.width = String(width | 0) + "px";
    c.style.top = top;
    c.style.left = left;
}
resize(window.innerWidth, window.innerHeight);

// Resize event
window.addEventListener("resize", () => {
    
    resize(window.innerWidth, window.innerHeight);
});

// Mouse events
window.addEventListener("mousemove", (e) => {

    // Autofocus
    window.focus();
});
 // Mouse down
 window.addEventListener("mousedown", (e) => {
     // Autofocus
    window.focus();
    e.preventDefault();
});
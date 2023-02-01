
function move(x,y) {
	var tx = scripted1.x;
	var ty = scripted1.y;
	scripted1.x = tx + x;
	scripted1.y = ty + y;
    print("MOVED SCRIPTED OBJECT")
    print(scripted1.x);
}

if(scripted1.x >= 800) {
    move(-1000, 0);
} else {
    move(1,0);
}
    

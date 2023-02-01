//Handle fly event (move script platform down)
function move(y) {
	var ty = scripted1.y;
	scripted1.y = ty + y;
    print("MOVED SCRIPTED OBJECT FLY")
    print(scripted1.y);
}


move(1);
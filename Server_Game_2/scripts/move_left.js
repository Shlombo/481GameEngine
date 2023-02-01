//Handle right event (move paddle left)
function move(x) {
	var tx = paddle.x;
	paddle.x = tx + x;
    print("MOVED SCRIPTED OBJECT Left")
    print(paddle.x);
}


move(-1);
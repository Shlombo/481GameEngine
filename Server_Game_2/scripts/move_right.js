//Handle right event (move paddle right)
function move(x) {
	var tx = paddle.x;
	paddle.x = tx + x;
    print("MOVED SCRIPTED OBJECT Right")
    print(paddle.x);
}


move(1);
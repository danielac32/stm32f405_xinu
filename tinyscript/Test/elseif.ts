func test_a(x){
	if x<1{
		print "a less"
	}else{
		if(x=1){
			print "a one"
		}else{
			if(x=2){
				print "a two"
			}else{
				if(x=3){
					print "a three"
				}else{
					print "a more"
				}
			}
		}
	}
}

func test_b(x){
	if(x<1){
		print "b less"
	}elseif(x=1){
		print "b one"
	}elseif(x=2) {
		print "b two"
	}elseif(x=3){
		print "b three"
	}else{
		print "b more"
	}
}

var i=0
while i<5{
	test_a(i)
	test_b(i)
	i=i+1
}

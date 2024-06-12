# define an array without initializing it
# the number in parenthesis is the length of the array
array x(3)

# define a function to print the contents of x
func print_x(){
	print "x: ", x(0), ", ", x(1), ", ", x(2)
}

# the data is initialized to zeros when defined without explicit initialization
print_x()

# set an array value
x(0) = 1
print_x()

# set multiple values starting at a specific index
x(1) = 2, 3
print_x()

# set multiple values starting at index zero
# the number of values can be <= array size
x = 4, 5, 6
print_x()

# define an array and set the values at the same time
# the number of values can be <= array size
array y(3) = 0x0C, 0x22, 0x38
print "y: ", y(0), ", ", y(1), ", ", y(2)

# access the array data
# note that index (-1) is the length of the array
var sum=0
var ix=0
while (ix < y(-1)) {
	sum = sum + y(ix)
	ix = ix + 1
}
print "sum of y = ", sum


# array name with no parens returns the address (pointer)
# this is exactly what we need to pass to C functions

# print "address of x = ", x
# don't actually print the address in this test becase it is always different


# tinyscript cannot pass variables to script functions by reference, only value
# so, passing an array to a script function just passes the address

func array_info(a){
	# convert the variable to an array (raises an error if not a valid array)
	array a
	print "array len=", a(-1), ", first element=", a(0)
}
array_info(y)


# example C function to consume a pointer to a tinyscript array
# element zero is the length, which must not be changed
#
#	Val ts_sumarray(Val* ary)
#	{
#		if ((Val)ary < (Val)memarena || (Val)(ary + ary[0]) >= (Val)(memarena + ARENA_SIZE)) {
#			// if the address is not in the arena, then it cannot be an array
#			return -1;
#		}
#		Val length = ary[0];
#		ary++;
#		Val sum = 0;
#		for (int ix = 0; ix < length; ix++) {
#			sum += ary[ix];
#		}
#		return sum;
#	}
#	err = TinyScript_Define("sumarray",  CFUNC(1), (Val)ts_sumarray);


# example of defining a tinyscript array from C
# remember that the first array element contains the length
#
#	Val aa[4] = {3, 0, 1, 2};
#	err = TinyScript_Define("aa", ARRAY, (Val)aa);


print ""
print "the array length can be read but not changed"
print "x(-1) is ", x(-1)
x(-1) = 20

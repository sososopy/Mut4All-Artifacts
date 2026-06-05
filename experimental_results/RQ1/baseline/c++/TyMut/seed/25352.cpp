
int main(){
_infinite_retry:
	[=]{
	_infinite_retry:
		goto _infinite_retry; // go to the second _infinite_retry.
	}();
	goto _infinite_retry; // go to the first _infinite_retry.
}

struct A {
 A *a[2];
 A() { a[0] = 0; a[1] = 0; }
 ~A(){ if(a[0]) delete a[0];  if(a[1]) delete a[1];
}

void main(){
A *a = new A();
a->a[0] = new A();
a->a[1] = new A();

delete a; //<----<< crash, because adress of a == address of a.a[0] :(
}

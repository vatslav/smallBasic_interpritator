var
	a:real;
	b:real;
begin
  writeln('Input variable a');
  readln( a );
  b:= (a*a);
  writeln(a,' * ',a,' = ',b);
  
  if b 
  >
  a  then
  writeln('b>a') 
  else 
  writeln('a>b')
end.


const
     SIZE = 10;

var
   b:array[1..2*SIZE, 1..SIZE] of integer;
   i,j:integer;
begin

   for i:=1 to SIZE do
   	for j:=1 to SIZE do
   		b[i,j] := i*j;


  
   for i:=1 to SIZE do
   	begin
   		for j:=1 to SIZE do
   			write( b[i,j], ' ' );
   		writeln;
   	end;
end.
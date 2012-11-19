var i,a:integer;

begin
	readln(a);
	if  a > 0  then
		writeln('OK!')
	else
		for i := 1 downto a do
			writeln(i);
	writeln('FINISH!');	
end.

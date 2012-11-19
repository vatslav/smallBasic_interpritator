var
 op:integer;
 a,b:real;


begin
 repeat 
  writeln ('1:=+');{Всем привет}
  writeln ('2:=-');
  writeln ('3:=/');
  writeln ('4:=*');
  writeln ('0:=exit');;

  readln (op);
  if (op>0) and (op<=4) then 
   begin
    writeln ('Input two numbers');
    readln (a,b);
    if op=1 then
     writeln ('The сумма is ',a+b)
    else
    if op=2 then
     writeln ('raz ',a-b)
    else
    if op=3 then
     if b<>0 then
      writeln ('chastn ',a/b)
     else
      writeln ('division by 0')
    else
    if op=4 then
     writeln ('mult ',a*b);

    readln;
   end
  else 
   if op<>0 then 
    begin
     writeln ('Error!');
     readln;
    end;
   until op=0; 
end.

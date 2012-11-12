




const
     SIZE = 5;

var
   b:array[1..2*SIZE, 1..SIZE] of integer;
   i,j:integer;
begin


   
   i:=1;
   repeat
         j:=1;
         repeat
               b[i,j] := i*j;
               j:=j+1;
         until j > SIZE;
         i:=i+1;
   until i > SIZE;

   i:=1;
  repeat
         j:=1;
         repeat

               write( b[i,j], ' ' );
               j:=j+1;
         until j > SIZE;
         writeln;
         i:=i+1;
   until i > SIZE;


   
end.
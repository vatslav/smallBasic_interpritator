var
 op:integer;
 a,b:real;

{
Должны работать перечисленные ниже входные комбинации( обозначим !enter - нажатие на клавишу Enter,
> ваш ввод, : вывода интерпретатора ):


STARTING OF PROGRAMM
: Test readln
: run "readln(a,b)"
> 4 5 !enter
: end "readln(a,b)"
: run "readln;"
> !enter
: end "readln;"
: Press Enter to quit'
> !enter
FINISHING OF THE PROGRAMM
________
STARTING OF PROGRAMM
: Test readln
: run "readln(a,b)"
> 4 !enter
> 5 !enter
: end "readln(a,b)"
: run "readln;"
> !enter
: end "readln;"
: Press Enter to quit
> !enter
FINISHING OF THE PROGRAMM
_____________________
STARTING OF PROGRAMM
: Test readln
: run "readln(a,b)"
> 4 5 vnjdfks v fnvjk nv kljsn vkdnvld !enter
: end "readln(a,b)"
: run "readln;"
> vndsjkfvn n jvknk nvknkvsl nkj !enter
: end "readln;"
: Press Enter to quit
> vnjdkvnjdksf nvklsdn klvnskl vnkldsn kldvsn !enter
FINISHING OF THE PROGRAMM
____________________



Вообще логика такая:
readln(a,b) - читае 2 первых слова из входного потока ( не важно, корректны ли они, это могут быть даже последовательности букв ) и пытается на основе этих данных записать числа в a и b.
Все остальные символы вплоть до перехода на следующую строку отбрасываются,
чтобы не всплыли при следующем вызове readln

readln; - читает ввод пользователя до тех пор, пока он не нажмёт enter.
}


begin
 writeln('Test readln');
 
 writeln('run "readln(a,b)"');
 readln(a, b);
 writeln('end "readln(a,b)"');
 writeln('run "readln;"');
 readln;
 writeln('end "readln;"');
 
 writeln('Press Enter to quit');
 readln;
end.

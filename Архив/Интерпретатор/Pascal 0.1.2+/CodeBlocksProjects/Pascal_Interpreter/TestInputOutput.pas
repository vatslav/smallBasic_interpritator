var
 op:integer;
 a,b:real;

{
������ �������� ������������� ���� ������� ����������( ��������� !enter - ������� �� ������� Enter,
> ��� ����, : ������ �������������� ):


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



������ ������ �����:
readln(a,b) - ����� 2 ������ ����� �� �������� ������ ( �� �����, ��������� �� ���, ��� ����� ���� ���� ������������������ ���� ) � �������� �� ������ ���� ������ �������� ����� � a � b.
��� ��������� ������� ������ �� �������� �� ��������� ������ �������������,
����� �� ������� ��� ��������� ������ readln

readln; - ������ ���� ������������ �� ��� ���, ���� �� �� ����� enter.
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

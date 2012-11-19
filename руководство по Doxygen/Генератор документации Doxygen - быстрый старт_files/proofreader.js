/* com_proofreader module Javascript library :: version 1.0 RC4 */

var gecko;

function keyPressInit()
{
	if (document.addEventListener)
	{
		document.addEventListener("keypress",
			function(e){keyAction(e)},true);
		if (navigator.appName == "Microsoft Internet Explorer") return;
		document.addEventListener("keydown", function(e){keyAction(e)}, true);
		gecko = true;
	} else if (document.attachEvent)
		document.attachEvent("onkeydown", keyAction);
}


function keyAction(e)
{
	if (!( ((e.shiftKey == true) && (pr_shift == true)) || ((e.altKey == true) && (pr_alt == true)) || ((e.ctrlKey == true) && (pr_ctrl == true)) )) return;
	var Key = e.keyCode;
	if (Key == 0) Key = e.charCode;
	if (Key == 13) {   
		if (!gecko && Key>64 && Key<91) Key += 32;
		var text;
		GetSelectedText();
		MistOpenPop("MistWindow");
	}
}


function GetMeHtmlCodePlease(){
	pr_setup();
	return xajax.call("GetMeHtmlCodePlease", arguments, 1);
}


function GetSelectedText()
{
	var text = "";
	var selection = null;
	if (document.getSelection) {
		text = document.getSelection();
	} else {
		selection = document.selection;
	}
	if (selection) {
		var r = selection.createRange();
		if (!r) return null;
		text = r.text;
	}
	if (text){
		GetMeHtmlCodePlease(text);
	}
}


function getrandom() { 
	var min_random = 0;
	var max_random = 99999;

	max_random++;

	var range = max_random - min_random;
	var n=Math.floor(Math.random()*range) + min_random;
	return n;
}


function MistClosePop() {
	var Obj = document.getElementById(arguments[0]);
	Obj.style.display="none";
}


function GetText(text)
{
	text = ""+text;
	return text.replace("\r", "").replace("\n", "").replace(new RegExp("^\\s+|\\s+$", "g"), "");
}


function MistOpenPop(m_form)
{
	var Obj = document.getElementById(arguments[0]);
	if (arguments[0]=="MistWindow") {
		var topSum=50;
		var topSum1=50;
		var leftSum=50;
		scrollTop = (parseInt(document.documentElement.scrollTop) > parseInt (document.body.scrollTop)) ? parseInt(document.documentElement.scrollTop) : parseInt (document.body.scrollTop);
		topSum = scrollTop + 50;
		leftSum = parseInt ((document.body.clientWidth/2)-(Obj.style.width.substr (-0,3)/2));
		if (topSum<50) {
			topSum=50;
		}
		if (leftSum<50) {
			leftSum=50;
		}
		Obj.style.top=topSum+'px';
		Obj.style.left=leftSum+'px';
		Obj.style.display="block";
	}
}


function SendPrepared()
{
	pr_setup();
	xajax.call("SendMistakeFunc", arguments, 1);
}
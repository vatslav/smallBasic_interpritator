var jc_isDOM=document.getElementById;
var jc_isO=window.opera&&jc_isDOM;
var jc_isO8=jc_isO&&document.createProcessingInstruction&&(new XMLHttpRequest()).getAllResponseHeaders;
var jc_isO9=jc_isO&&window.getSelection;
var jc_isIE=document.all&&document.all.item&&!jc_isO;
var jc_isMZ=jc_isDOM&&(navigator.appName=="Netscape");
var jc_cache=new Array();
var jc_enable_translit=0;

function jc_addComment(){jc_setup();return xajax.call('addComment',arguments,1);}
function jc_applyComment(){jc_setup();return xajax.call('saveComment',arguments,1);}
function jc_deleteComment(){jc_setup();return xajax.call('deleteComment',arguments,1);}
function jc_editComment(i){jc_setup();var a=new Array();a[0]=i;if((jc_cache[i]=='')||(!jc_def(jc_cache[i]))) return xajax.call('editComment',a,1);}
function jc_cancelComment(){jc_setup();return xajax.call('cancelComment',arguments,1);}
function jc_publishComment(){jc_setup();return xajax.call('publishComment',arguments,1);}
function jc_quoteComment(){jc_setup();return xajax.call('quoteComment',arguments,1);}
function jc_showPage(){jc_setup(); return xajax.call('showPage',arguments,1);}
function jc_showComment(){jc_setup();return xajax.call('showComment',arguments,1);}
function jc_jump2email(){jc_setup();return xajax.call('jump2email',arguments,1);}
function jc_sendComment(){jc_addComment(xajax.getFormValues('comments-form'));return false;}
function jc_saveComment(i){jc_applyComment(xajax.getFormValues('comment-edit-form-'+i));return false;}
function jc_showForm(){jc_setup();return xajax.call('showForm',arguments,1);}

function jc_cancelEditComment(i){if(jc_cache[i]!=''){xajax.$('comment-body-'+i).innerHTML=jc_cache[i];jc_cache[i]='';jc_cancelComment(i);}}
function jc_geteditor(){return xajax.$('comments-form-comment');}
function jc_scrollTo(n){var e=xajax.$(n),t=e.offsetTop;for(var p=e.offsetParent;p;p=p.offsetParent){t+=p.offsetTop;}scrollTo(0,t);}
function jc_storeCaret(){var ta=jc_geteditor();if(typeof(ta.createTextRange)!="undefined")ta.caretPos=document.selection.createRange().duplicate();}
function jc_def(v){return (typeof(v)!="undefined");}
function jc_bbcode(e){return jc_surroundText('['+e+']','[/'+e+']');}
function jc_bbcode_list(e){return jc_surroundText('[list][*]','[/list]');}
function jc_smile(t){return jc_insertText(' '+t+' ');}
function jc_goto(l){window.open(l);return;}

function jc_updateafterdelete(id) {
	var l=xajax.$('comments-list'),i=0,f=false,c='';
	for(var i=0;i<l.childNodes.length;i++){
		var n=l.childNodes[i];
	        if((n.tagName!=undefined)&&(n.tagName!='')){
	        	if(n.getAttribute('id')==('comment-'+id)){f=true;c=n.getAttribute('class');}
	        	else if(f){var s=n.getAttribute('class');n.setAttribute('class',c);c=s;}
		}
	}
}

function jc_insertText(text) {
        var ta=jc_geteditor();
	if(jc_def(ta.caretPos)&&ta.createTextRange){ta.focus();var sel=document.selection.createRange();sel.text=sel.text+text;ta.focus();}
	else if(jc_def(ta.selectionStart)){var ss=ta.value.substr(0, ta.selectionStart);
		var se=ta.value.substr(ta.selectionEnd),sp=ta.scrollTop;
		ta.value=ss+text+se;
		if(ta.setSelectionRange){ta.focus();ta.setSelectionRange(ss.length+text.length,ss.length+text.length);}
		ta.scrollTop=sp;
	} else {
		ta.value+=text;
		ta.focus(ta.value.length - 1);
	}
}

function jc_surroundText(t1,t2){
        var ta=jc_geteditor();
	if (jc_def(ta.caretPos) && ta.createTextRange){
		var cp=ta.caretPos,tl=cp.text.length;
		cp.text=cp.text.charAt(cp.text.length-1)==' '?t1+cp.text+t2+' ':t1+cp.text+t2;
		if(tl==0){cp.moveStart("character",-t2.length);cp.moveEnd("character",-t2.length);cp.select();}
		else{ta.focus(cp)};
	}else if(jc_def(ta.selectionStart)){
		var ss=ta.value.substr(0,ta.selectionStart),se=ta.value.substr(ta.selectionEnd);
		var sl=ta.value.substr(ta.selectionStart,ta.selectionEnd-ta.selectionStart);
		var nc=ta.selectionStart,sp=ta.scrollTop;
		ta.value=ss+t1+sl+t2+se;
		if(ta.setSelectionRange){
			if(sl.length==0){ta.setSelectionRange(nc+t1.length,nc+t1.length);}
			else{ta.setSelectionRange(nc,nc+t1.length+sl.length+t2.length);}
			ta.focus();
		}
		ta.scrollTop=sp;
	}else{ta.value+=t1+t2;ta.focus(ta.value.length-1);}
	return true;
}

function jc_checkKey(k){
if(k==4181||k==4169||k==2097||k==2098||k==2099||k==2100||k==4172||k==2124||k==2126||k==2127
||k==2114||k==2131||k==2133||k==2120||k==2121||k==2122||k==2132||k==2109||k==2156||k==2158
||k==2159||k==2146||k==2163||k==2165||k==2153||k==2152||k==2154){return true;}else{return false;}}

function jc_keydown(e) {
	var s,sl,r=false,code=e.keyCode,ta=jc_geteditor();
	if(ta==null){return}
	if(!e) var e=window.event;
	if(code==0) code=e.charCode;
	if(e.altKey&&!e.ctrlKey) code=code+4096;
	if(e.ctrlKey) code=code+2048;
	if(jc_isMZ&&e.type=="keypress"&&jc_checkKey(code)){e.preventDefault();e.stopPropagation();return false;}
	if(jc_isMZ&&e.type=="keyup"&&(code==13||code==2061)) return false;
	if(jc_isIE&&code==2048+187) code=2048+61;
	if(jc_isIE&&code==2048+189&&e.shiftKey) code=2048+95;

	s=jc_isIE?document.selection.createRange().text:ta.value.substr(ta.selectionStart,ta.selectionEnd-ta.selectionStart);
	sl=(s.length>0);

	switch (code) {
		case 43:
		case 2091: 
		case 2109:
	                if(e.ctrlKey&&e.shiftKey){ta.style.height=ta.clientHeight+12+'px';r=true;}break;
		case 45:
		case 2093: 
		case 2143:
	                if(e.ctrlKey&&e.shiftKey){if(ta.clientHeight>50) ta.style.height=ta.clientHeight-12+'px';r=true;}break;
		case 2114:
			if(sl)r=jc_bbcode('b');break;
		case 2131:
			if(sl)r=jc_bbcode('s');break;
		case 2133:
			if(sl)r=jc_bbcode('u');break;
		case 2121:
			if(sl)r=jc_bbcode('i');break;
		case 2129:
		case 2146:
			var txt=null;if(document.getSelection){txt=document.getSelection();}
			else{var ds=document.selection;if(ds){var rn=ds.createRange();if(rn){txt=rn.text;}}}
			if(txt!=null){jc_insertText('[quote]'+txt+'[/quote]')}r=true;
			break;
		case 13:
		case 2061:
		case 4109:
			if(e.ctrlKey&&(ta.value.length>0)){jc_sendComment();r=true;}break;
	}
	if(r){ta.focus();if(jc_isMZ||jc_isO8){e.cancelBubble=true;e.preventDefault();e.stopPropagation();}e.returnValue=false;return false;} 
}

function jc_dblclick(e){
	var o=jc_isMZ?e.target:((jc_isIE||jc_isO)?e.srcElement:null);
	while (o!=null&&o.tagName!="BODY") {
		var id=(o.getAttribute("id")!=null)?o.getAttribute("id"):"";
		if(id.substring(0,13)=='comment-body-'){jc_editComment(id.substring(13,id.length));break;}
		o=jc_isMZ?o.parentNode:((jc_isIE||jc_isO)?o.parentElement:null);
	}
}

if(jc_isIE||jc_isO8){
	document.attachEvent("ondblclick",jc_dblclick);
	document.attachEvent((jc_isIE?"onkeydown":"onkeypress"),jc_keydown);
}else if(jc_isMZ){
	document.addEventListener("dblclick",jc_dblclick,true);
	document.addEventListener("keypress",jc_keydown,true);
	document.addEventListener("keyup",jc_keydown,true);
}

function jc_keypress(event){
	if(jc_enable_translit==1){return jc_translate(event);}
	return true;
}		
function jc_changed(ml){
	if (ml != 0) {
		var o=jc_geteditor();if(o.value.length>ml){o.className='error';}else if(o.className!=''){o.className='';}
		xajax.$("comments-form-comment-length").innerHTML=ml-o.value.length;
	}
	return false;
}
function js_lib_addItem(o_sel, s_text, s_value)
	{
	o_sel.options[o_sel.options.length] = new Option(s_text, s_value, true, true);	
	}
function js_lib_openWindow(s_url,s_title,i_width,i_height) {
	var width  = i_width;
	var height = i_height;
	var left   = (screen.width  - width)/2;
	var top    = (screen.height - height)/2;
	var s_params = '';

	s_params += 'width='+width;
	s_params += ', height='+height;
	s_params += ', top='+top;
	s_params += ', left='+left;
	s_params += ', directories=no';
	s_params += ', location=no';
	s_params += ', menubar=no';
	s_params += ', resizable=no';
	s_params += ', scrollbars=no';
	s_params += ', status=no';
	s_params += ', toolbar=no';
	newwin = window.open(s_url,s_title,s_params,false);
	if (window.focus) {
		newwin.focus()
	}
	
	return false;
}
function js_lib_collapseMe(s_container,s_imageElement) {
	s_showImage = "../img/show.png";
	s_hideImage = "../img/hide.png";
	if (document.getElementById(s_container)) {
		if (document.getElementById(s_container).style.display == "block" || document.getElementById(s_container).style.display == "table-row-group") {
			document.getElementById(s_container).style.display = "none";
			document.getElementById(s_imageElement).src = s_showImage;
			document.getElementById(s_imageElement).title = "show detail";
		} else {
			var isWebKit = navigator.userAgent.indexOf("AppleWebKit") > -1;
			if (isWebKit) {
				document.getElementById(s_container).style.display = "table-row-group";
			} else {
				document.getElementById(s_container).style.display = "block";
			}
			document.getElementById(s_imageElement).src = s_hideImage;
			document.getElementById(s_imageElement).title = "hide detail";
		}
	}
}
function js_lib_collapseMe0(s_container) {
	if (document.getElementById(s_container)) {
		if (document.getElementById(s_container).style.display == "block" || document.getElementById(s_container).style.display == "table-row-group") {
			document.getElementById(s_container).style.display = "none";
		} else {
			var isWebKit = navigator.userAgent.indexOf("AppleWebKit") > -1;
			if (isWebKit) {
				document.getElementById(s_container).style.display = "table-row-group";
			} else {
				document.getElementById(s_container).style.display = "block";
			}
		}
	}
}
function js_lib_getAllTags(groupID,s_element_mask,s_imageElement) {
	document.getElementById(s_imageElement).style.cursor = 'wait';
	//document.getElementById(s_imageElement).style.display = "block";
	var arr = new Array();
    arr = document.getElementsByTagName("*");
    for(var i=0;i<arr.length;i++) {
        var tagName = document.getElementsByTagName("*").item(i).id;
        //var tagObj = document.getElementsByTagName("*").item(i);
        if (tagName.split("_",1) == s_element_mask+groupID) {
            //if (x == y) {
            //	document.getElementsByTagName(tagName).style.display = "none";
            //}else{
            //	document.getElementsByTagName(tagName).style.display = "block";
            //}
        	js_lib_collapseMe(tagName,s_imageElement);
        }
		//if (tagName.split("_",1) > s_element_mask+groupID) {break;}
    }
	document.getElementById(s_imageElement).style.cursor = 'pointer';
}

function js_lib_copyText(text) {
  if (window.clipboardData) {
    window.clipboardData.setData("Text",text);
  }
}

function js_lib_isNumeric(sText) {
	var ValidChars = "0123456789.";
	var b_retval=true;
	var Char;
	for (i = 0; i < sText.length && b_retval; i++) { 
		Char = sText.charAt(i); 
		if (ValidChars.indexOf(Char) == -1) {
			b_retval = false;
		}
	}
	return b_retval;
}
function js_lib_requireDate(domID,sText) {
	if (sText.length > 0 && !js_lib_isDate(sText)) {
		//alert("Value must be a date in the format of: mm/dd/yyyy.");
		document.getElementById(domID).focus();
	}
}

function js_lib_requireNumber(domID,sText) {
	if (!js_lib_isNumeric(sText)) {
		alert("Value must be a number.");
		document.getElementById(domID).focus();
	}
}

/**
 * DHTML date validation script. Courtesy of SmartWebby.com (http://www.smartwebby.com/dhtml/)
**/
// Declaring valid date character, minimum year and maximum year
var dtCh= "/";
var minYear=1900;
var maxYear=2100;

function isInteger(s){
	var i;
    for (i = 0; i < s.length; i++){   
        // Check that current character is number.
        var c = s.charAt(i);
        if (((c < "0") || (c > "9"))) return false;
    }
    // All characters are numbers.
    return true;
}

function stripCharsInBag(s, bag){
	var i;
    var returnString = "";
    // Search through string's characters one by one.
    // If character is not in bag, append to returnString.
    for (i = 0; i < s.length; i++){   
        var c = s.charAt(i);
        if (bag.indexOf(c) == -1) returnString += c;
    }
    return returnString;
}

function daysInFebruary (year){
	// February has 29 days in any year evenly divisible by four,
    // EXCEPT for centurial years which are not also divisible by 400.
    return (((year % 4 == 0) && ( (!(year % 100 == 0)) || (year % 400 == 0))) ? 29 : 28 );
}
function DaysArray(n) {
	for (var i = 1; i <= n; i++) {
		this[i] = 31
		if (i==4 || i==6 || i==9 || i==11) {this[i] = 30}
		if (i==2) {this[i] = 29}
   } 
   return this
}

function js_lib_isDate(dtStr){
	var daysInMonth = DaysArray(12)
	var pos1=dtStr.indexOf(dtCh)
	var pos2=dtStr.indexOf(dtCh,pos1+1)
	var strMonth=dtStr.substring(0,pos1)
	var strDay=dtStr.substring(pos1+1,pos2)
	var strYear=dtStr.substring(pos2+1)
	strYr=strYear
	if (strDay.charAt(0)=="0" && strDay.length>1) strDay=strDay.substring(1)
	if (strMonth.charAt(0)=="0" && strMonth.length>1) strMonth=strMonth.substring(1)
	for (var i = 1; i <= 3; i++) {
		if (strYr.charAt(0)=="0" && strYr.length>1) strYr=strYr.substring(1)
	}
	month=parseInt(strMonth)
	day=parseInt(strDay)
	year=parseInt(strYr)
	if (pos1==-1 || pos2==-1){
		alert("The date format should be : mm/dd/yyyy")
		return false
	}
	if (strMonth.length<1 || month<1 || month>12){
		alert("Please enter a valid month")
		return false
	}
	if (strDay.length<1 || day<1 || day>31 || (month==2 && day>daysInFebruary(year)) || day > daysInMonth[month]){
		alert("Please enter a valid day")
		return false
	}
	if (strYear.length != 4 || year==0 || year<minYear || year>maxYear){
		alert("Please enter a valid 4 digit year between "+minYear+" and "+maxYear)
		return false
	}
	if (dtStr.indexOf(dtCh,pos2+1)!=-1 || isInteger(stripCharsInBag(dtStr, dtCh))==false){
		alert("Please enter a valid date")
		return false
	}
return true
}
/**
 * DHTML date validation script. Courtesy of SmartWebby.com (http://www.smartwebby.com/dhtml/)
**/

/*
//for debugging  
window.onerror=function(msg, url, linenumber){
	var logerror='Error message: ' + msg + '\n. Url: ' + url + '\nLine Number: ' + linenumber
	alert(logerror)
	return true
}
*/
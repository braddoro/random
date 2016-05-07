<html>
<head>
<style>
.scroll {text-align:center;width:80%;font-family:Sans-serif;font-size:.8em;background-color:#F5F5F5;}
.scroll2 {text-align:center;width:100%;font-family:Sans-serif;font-size:.8em;}
</style>
</head>
<body>
<div id="div_title" class="scroll2">Tasks</div>
<div align="center"><img src="..\img\hide.png" border="0" onclick="js_dateChange(-1);" title="previous" style="display:inline;">&nbsp;<span id="div_input" name="div_input" class="scroll"></span>&nbsp;<img src="..\img\show.png" border="0" onclick="js_dateChange(1);" title="next" style="display:inline;"></div>
<hr/>
<div id="div_output" class="scroll2"></div>
</body>
<script type="text/javascript" language="javascript">
var i_days = 3;
var g_dateStart = new Date();
g_dateStart.setDate(g_dateStart.getDate());
function js_parseDate(day) {
	var tempDate = new Date(day);
	var i_year	= tempDate.getFullYear();
	var i_day 	= tempDate.getDate();
	var i_month	= tempDate.getMonth();
	i_month++;
	var s_return = i_month+"/"+i_day+"/"+i_year;
	return s_return;
}
function js_dateChange(dir) {
	if (dir == -1) {g_dateStart.setDate(g_dateStart.getDate()-1);}
	if (dir == 1) {g_dateStart.setDate(g_dateStart.getDate()+1);}

	var tempDate = new Date();
	tempDate.setDate(g_dateStart.getDate());
	tempDate.setMonth(g_dateStart.getMonth());
	tempDate.setFullYear(g_dateStart.getFullYear());
	var s_output = "";
	for (var x=0;x<i_days;x++) {
		s_js = 'js_showDay("'+tempDate+'");';
		s_post = "";
		if (g_dateStart.getDate() == tempDate.getDate()) {
			s_post = "font-weight:bold;";
		}
		s_output += "<span id='span_day_"+x+"' name='span_day_"+x+"' onclick='"+s_js+"' style='"+s_post+"'>"+js_parseDate(tempDate)+"</span>&nbsp;"
		
		tempDate.setDate(tempDate.getDate()+1);
	}
	document.getElementById("div_input").innerHTML = s_output;
}
function js_showDay(day) {
	var s_ajax = "task=scroll&date="+js_parseDate(day);
	var s_return = http_post_request("http://cha-SSQL03.huskyclt.com/calendar/salescalendar_ajax.php",s_ajax);
	document.getElementById("div_output").innerHTML = s_return;
	var tempDate = new Date(day);
	var i_year	= tempDate.getFullYear();
	var i_day 	= tempDate.getDate();
	var i_month	= tempDate.getMonth();
	g_dateStart.setMonth(i_month);
	g_dateStart.setDate(i_day);
	g_dateStart.setFullYear(i_year);
	js_dateChange(0);
}
function http_post_request(url,s_param) {
	var s_results = "";
	var ajaxTime = new Date();
	s_param += "&ajaxTime=" + ajaxTime.getTime();

	var o_req = false;
	if(window.XMLHttpRequest) {
		o_req = new XMLHttpRequest();
	}else{
		try{o_req = new ActiveXObject("MSXML2.XMLHTTP.6.0");}catch(e){alert(e.description);}
		try{if(!o_req) o_req = new ActiveXObject("MSXML2.XMLHTTP");}catch(e){alert(e.description);}
	}
	if (!o_req) {
		s_results = "Cannot create XML/HTTP instance";
	} else {
		o_req.open("POST",url,false);
		o_req.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
		o_req.setRequestHeader("Cache-Control","no-cache");
		o_req.setRequestHeader("Content-length",s_param.length);
		o_req.setRequestHeader("Connection","close");
		o_req.send(s_param);
		s_results = o_req.responseText;
	}
	return s_results;
}
js_dateChange(-1);
</script>
</html>

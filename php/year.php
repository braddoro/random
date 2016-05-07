<html>
<head>
<META content="text/html; charset=windows-1252" http-equiv=Content-Type></HEAD>
<style type="text/css">
.s1 {font-size: .66em;}
.s2 {font-size: .75em;}
.label2 {font-size: .75em;font-family: Arial, Helvetica, sans-serif;}
.label {font-size: 1em;font-family: Arial, Helvetica, sans-serif;font-weight:bold;}
.tabledata {padding: 5px;font-size: .75em;font-family: Arial, Helvetica, sans-serif;width:100%;height:92%;z-Index:1;}
.detail0 {width:10px;text-align:right;height:20px;vertical-align:middle;font-weight:bold;border-left:1px solid #999999;border-top:1px solid #999999;border-bottom:none;border-right:none;}
.detail {text-align:right;height:20px;vertical-align:top;border: 1px solid #999999;border-left:1px solid #999999;border-top:1px solid #999999;border-bottom:none;border-right:none;}
.detail_bott {border-bottom:1px solid #999999;}
.detail_right {border-right:1px solid #999999;}
.popdiv {position:absolute;top:75px;left:1px;top:1px;width:200px;height:100px;display:none;overflow:auto;z-Index:5;border:5px solid navy;background-color:#F5F5F5;}
</style>
<script language="javascript" type="text/javascript">
function http_post_request(url,sPostString) {
	var results = "";
	var ajax_http_request = false;
	var AjaxTime = new Date();
	sPostString += "&ajaxTime=" + AjaxTime.getTime();
	if (window.XMLHttpRequest) {
		ajax_http_request = new XMLHttpRequest();
		if (ajax_http_request.overrideMimeType) {
			ajax_http_request.overrideMimeType("text/html");
		}
	} else if (window.ActiveXObject) {
		try {
			ajax_http_request = new ActiveXObject("Msxml2.XMLHTTP");
		} catch (e0) {
			try {
				ajax_http_request = new ActiveXObject("Microsoft.XMLHTTP");
			} catch (e1) {
				var strErr = "Object Error";
				strErr += "\nNumber: " + e1.number;
				strErr += "\nDescription: " + e1.description;
				results = strErr;
			}
		}
	}
	if (!ajax_http_request) {
		results = "Cannot create XML/HTTP instance";
	} else {
		ajax_http_request.open("POST", url, false);
		ajax_http_request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
		ajax_http_request.setRequestHeader("Content-length", sPostString.length);
		ajax_http_request.setRequestHeader("Connection", "close");
		ajax_http_request.send(sPostString);
		results = ajax_http_request.responseText;
	}
	return results;
}
function js_expand(i_joo,s_greg,s_dow) {
	//alert(document.body.scrollTop);
	s_id = "event_"+i_joo;
	s_event = document.getElementById(s_id).innerHTML;
	s_button = "<div class='label2'>&nbsp;Date:&nbsp;"+s_dow+"&nbsp;"+s_greg+"<input type='hidden' id='julian_curr' value='"+i_joo+"'></div><textarea id='event' name='event' style='margin-left:2px;margin-top:2px;'>"+s_event+"</textarea><div align='center'><button id='saveme' name='saveme' onclick='js_saveMe();' value='Save'>Save</button>&nbsp;<button id='closeme' name='closeme' onclick='js_hideMe();' value='Close'>Close</button></div>";
	document.getElementById("div_expand").innerHTML = s_button;
	document.getElementById("div_expand").style.height = 250;
	document.getElementById("div_expand").style.width = 415;
	document.getElementById("div_expand").style.top = 50;
	document.getElementById("div_expand").style.left = (screen.width/4)/2;
	document.getElementById("event").style.height = 250-60;
	document.getElementById("event").style.width = 415-15;
	document.getElementById("div_expand").style.display = "block";
}
function js_hideMe() {
	document.getElementById("div_expand").style.display = "none";
}
function js_saveMe() {
	var s_ajax = "task=save";
	s_ajax += "&julian="+document.getElementById("julian_curr").value;
	s_ajax += "&overlayID="+document.getElementById("overlayID").value;
	s_ajax += "&input="+document.getElementById("event").value+"";
	if (document.getElementById("overlayID").value > 0) {
		http_post_request("year_ajax.php",s_ajax);
		s_id = "event_"+document.getElementById("julian_curr").value;
		document.getElementById(s_id).innerHTML = document.getElementById("event").value;
	}
	document.getElementById("div_expand").style.display = "none";
}
</script>
</head>
<body>
<?php
if (isset($_REQUEST["o"])) {
	$i_overlayID = $_REQUEST["o"];
} else {
	$i_overlayID = -1;
}
if (isset($_REQUEST["y"])) {
	$i_year = intval($_REQUEST["y"]);
} else {
	$i_year = date("Y");
}
$i_month=1;
$i_Jtoday = date("Yz");
$i_julianDay = 1;
$i_julianYear = $i_year.str_pad($i_julianDay,3,"0",STR_PAD_LEFT);
$s_sql = "select dateID, overlayID, julianDate, event from dbo.julian_dates where overlayID in ($i_overlayID) order by julianDate";
$server		= "CHA-SSQL03,1433";
$username	= "data_writer";
$password	= "hWCiKNs1U4";
$dbname		= "calendar";
$conn=mssql_connect($server, $username, $password);
$sqldb=mssql_select_db($dbname,$conn);
$o_result = mssql_query($s_sql,$conn);
echo "<input type='hidden' name='overlayID' id='overlayID' value='$i_overlayID'>";
echo "<span class='label'>$i_year</span>";
echo "<table class='tabledata' border='0' cellspacing='0' cellpadding='0'>";
for ($i_month=1;$i_month<13;$i_month++) {
	$s_class_bott = "";
	// Months for non-leap years, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31.
	switch ($i_month) {
	case 1:
		$s_month = "Jan";
		$i_days = 31;
		break;
	case 2:
		$s_month = "Feb";
		$i_days = 28;
		//((year MOD 4 = 0 AND year MOD 100 <> 0) OR year MOD 400 = 0)
		if ((($i_year % 4) == 0 && ($i_year % 100) != 0) || ($i_year % 400) == 0){
			$i_days = 29;
		}
		break;
	case 3:
		$s_month = "Mar";
		$i_days = 31;
		break;
	case 4:
		$s_month = "Apr";
		$i_days = 30;
		break;
	case 5:
		$s_month = "May";
		$i_days = 31;
		break;
	case 6:
		$s_month = "Jun";
		$i_days = 30;
		break;
	case 7:
		$s_month = "Jul";
		$i_days = 31;
		break;
	case 8:
		$s_month = "Aug";
		$i_days = 31;
		break;
	case 9:
		$s_month = "Sep";
		$i_days = 30;
		break;
	case 10:
		$s_month = "Oct";
		$i_days = 31;
		break;
	case 11:
		$s_month = "Nov";
		$i_days = 30;
		break;
	case 12:
		$s_month = "Dec";
		$i_days = 31;
		$s_class_bott = "detail_bott";
		break;
	}
	echo "<tr>";
	$s_class_right = "";
	echo "<td class='detail0 $s_class_right $s_class_bott'>$s_month</td>";
	for ($i_day=1;$i_day<32;$i_day++) {
		if ($i_day == 31) {
			$s_class_right = "detail_right";
		}
		$i_time = mktime(0,0,0,$i_month,$i_day,$i_year);
		$i_dow = idate("w",$i_time);
		switch($i_dow) {
		case 0:
			// sunday
			$s_style = "background-color: #EFEFEF;";
			$s_abbr = "S";
			break;
		case 1:
			$s_style = "";
			$s_abbr = "M";
			break;
		case 2:
			$s_style = "";
			$s_abbr = "T";
			break;
		case 3:
			$s_style = "";
			$s_abbr = "W";
			break;
		case 4:
			$s_style = "";
			$s_abbr = "T";
			break;
		case 5:
			$s_style = "";
			$s_abbr = "F";
			break;
		case 6:
			$s_style = "background-color: #EFEFEF;";
			$s_abbr = "S";
			break;
		}
		if ($i_Jtoday == $i_julianYear) {
			$s_bold = "font-weight:bold;text-decoration:underline;";
			$s_bold = "";
		} else {
			$s_bold = "";
		}
		$i_julianYear = $i_year.str_pad($i_julianDay,3,"0",STR_PAD_LEFT);
		$s_greg = date("m/d/Y",$i_time);
		$s_event = "";
		$event = "";
		$s_dow = strftime("%A", strtotime($s_greg));
		if (mssql_num_rows($o_result) > 0) {
			mssql_data_seek($o_result,0);
		}
		while($row = mssql_fetch_object($o_result)) {
			$dateID		= $row->dateID;
			$overlayID	= $row->overlayID;
			$julianDate	= $row->julianDate;
			$event		= $row->event;
			if ($julianDate == $i_julianYear) {
				$s_event = $event;
				break;
			}
		}
		echo "<td id='day_$i_julianYear' class='detail $s_class_right $s_class_bott' style='$s_style$s_bold' onclick='js_expand($i_julianYear,\"$s_greg\",\"$s_dow\");'>";
		if ($i_day <= $i_days) {
			if ($i_day < 10 ) {echo "&nbsp;";}
			echo $i_day."<span class='s1'>".$s_abbr."&nbsp;</span>";
			$i_julianDay++;
		} else {
			echo "&nbsp;";
		}
		echo "<div id='event_$i_julianYear' class='s2'>$s_event</div>";
		echo "</td>";
	}
	echo "</tr>";
}
echo "</table>";
?>
<div id='div_expand' name='div_expand' class='popdiv'></div>
</body>
</html>

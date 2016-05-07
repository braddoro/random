<?php
$g_break = chr(10).chr(13);
define("CONSTANT_LINE_BREAK","\n");

function lib_divide2($i_divisor,$i_dividend)
	{
	// 2 = 5/10
	$i_quotient = 0;
	if ($i_divisor != 0 && $i_dividend != 0)
		{
		$i_quotient = ($i_dividend/$i_divisor);
		}
	return $i_quotient;
	}
function lib_divide($i_divisor,$i_dividend)
	{
	// 2 = 5/10
	$i_quotient = 0;
	if ($i_divisor != 0 && $i_dividend != 0)
		{
		$i_quotient = ($i_dividend/$i_divisor)*100;
		}
	return $i_quotient;
	}
function lib_format_phone($phone_in)
	{
	$retval = "";
	$phone_in = str_replace("No Value","",$phone_in);
	$phone_in = str_replace(" ","",$phone_in);
	$phone_in = str_replace("-","",$phone_in);
	$phone_in = str_replace("(","",$phone_in);
	$phone_in = str_replace(")","",$phone_in);
	if (strlen($phone_in) == 10)
		{
		$npx = substr($phone_in,0,3);
		$nxx = substr($phone_in,3,3);
		$line = substr($phone_in,6,4);
		$phone_in = $npx."-".$nxx."-".$line; 
		}
	$retval = $phone_in;
	
	return $retval;	
	}

function lib_formatTime($secs) {
   $times = array(3600, 60, 1);
   $time = '';
   $tmp = '';
   for($i = 0; $i < 3; $i++) {
      $tmp = floor($secs / $times[$i]);
      if($tmp < 1) {
         $tmp = '00';
      }
      elseif($tmp < 10) {
         $tmp = '0' . $tmp;
      }
      $time .= $tmp;
      if($i < 2) {
         $time .= ':';
      }
      $secs = $secs % $times[$i];
   }
   return $time;
}

function lib_calc_mph($i_miles,$i_seconds,$i_precision=0) {
	if ($i_miles > 0 && $i_seconds > 0) {
		$i_mph = round($i_miles/($i_seconds/60/60),$i_precision);
	}else{
		$i_mph = 0;
	}
	return $i_mph;
}

function lib_close_db() {
	global $sqlconnect;
	mssql_close($sqlconnect);
}

function lib_convert_date($in) {
	$td = strtotime($in);
	//06-25-2008 00:52:57
	$Datespec = date('m\-d\-Y H:i:s',$td);

	return $Datespec;
}

function lib_clean_string($instr) {
	$outstr = str_replace('"','',$instr);
	$outstr = trim($outstr);
	$outstr = rtrim($outstr);
	$outstr = ltrim($outstr);
	
	$outstr = preg_replace('[\']', '"', $outstr);
	$outstr = preg_replace('[\t]', '[t]', $outstr);
	$outstr = preg_replace('[\n]', '[n]', $outstr);
	$outstr = preg_replace('[\r]', '[r]', $outstr);
	
	return $outstr;
}

// Accepts 2 times from SQL Server and compares them.
//                                               1111111111
//                                     0123456789012345678
// it assumes that the time looks like 2009-01-08 17:26:08
// convert(char(19),field,120)
function lib_date_sql2php($s_datein1,$i_format=0){
	$s_return = $s_datein1;
	
	// convert(char(19),field,120)
	//                                               1111111111
	//                                     0123456789012345678
	// it assumes that the time looks like 2009-01-08 17:26:08
	if ($i_format == 120 && strlen($s_datein1) == 19) {
		$y1 = substr($s_datein1,0,4);
		$t1 = substr($s_datein1,5,2);
		$d1 = substr($s_datein1,8,2);
		$h1 = substr($s_datein1,11,2);
		$m1 = substr($s_datein1,14,2);
		$s1 = substr($s_datein1,17,2);
		$s_return = mktime($h1,$m1,$s1,$t1,$d1,$y1);
	}

	// straight data format no conversion
	//                                               1111111111222
	//                                     01234567890123456789012
	// it assumes that the time looks like 2010-01-16 12:28:19.000
	if ($i_format == 0 && strlen($s_datein1) == 22) {
		$y1 = substr($s_datein1,0,4);
		$t1 = substr($s_datein1,5,2);
		$d1 = substr($s_datein1,8,2);
		$h1 = substr($s_datein1,11,2);
		$m1 = substr($s_datein1,14,2);
		$s1 = substr($s_datein1,17,2);
		$s_return = mktime($h1,$m1,$s1,$t1,$d1,$y1);
	}

	return $s_return;
}

// Accepts 2 times from SQL Server and compares them.
//                                               1111111111
//                                     0123456789012345678
// it assumes that the time looks like 2009-01-08 17:26:08
// convert(char(19),field,120)
function lib_dateTimeDiff($s_datein1,$s_datein2){
	$s_diff = "";
		
	$y1 = substr($s_datein1,0,4);
	$t1 = substr($s_datein1,5,2);
	$d1 = substr($s_datein1,8,2);
	$h1 = substr($s_datein1,11,2);
	$m1 = substr($s_datein1,14,2);
	$s1 = substr($s_datein1,17,2);

	$y2 = substr($s_datein2,0,4);
	$t2 = substr($s_datein2,5,2);
	$d2 = substr($s_datein2,8,2);
	$h2 = substr($s_datein2,11,2);
	$m2 = substr($s_datein2,14,2);
	$s2 = substr($s_datein2,17,2);

	$s_diff = mktime($h2,$m2,$s2,$t2,$d2,$y2)-mktime($h1,$m1,$s1,$t1,$d1,$y1);
	
	return $s_diff;
}

function lib_distance($lat1, $lng1, $lat2, $lng2, $miles = true){
	$pi80 = M_PI / 180;
	$lat1 *= $pi80;
	$lng1 *= $pi80;
	$lat2 *= $pi80;
	$lng2 *= $pi80;
	//$r = 6372.797; // mean radius of Earth in km
	$r = 6371; // mean radius of Earth in km
	$dlat = $lat2 - $lat1;
	$dlng = $lng2 - $lng1;
	$a = sin($dlat / 2) * sin($dlat / 2) + cos($lat1) * cos($lat2) * sin($dlng / 2) * sin($dlng / 2);
	$c = 2 * atan2(sqrt($a), sqrt(1 - $a));
	$km = $r * $c;
	return ($miles ? ($km * 0.621371192) : $km);
}

function lib_dealer_class($ytd) {
	switch ($ytd) {
	case ($ytd > 300000):
		$s_return = "A";		
		break;
	case ($ytd > 100000):
		$s_return = "B";
		break;
	case ($ytd > 50000):
		$s_return = "C";
		break;		
	case ($ytd > 20000):
		$s_return = "D";
		break;		
	case ($ytd > 10000):
		$s_return = "E";
		break;
	default:
		$s_return = "F";
		break;
	}
	return $s_return;
}

function lib_die_well ($s_error="",$s_detail="",$s_file="",$i_line=0) {
	echo "Page: ".$s_file." Line:".$i_line."\nError:".$s_error."\nDetail:".$s_detail;
	$fp=fopen("error.txt","a");
	$output = date('Y-m-d H:i:s')." | ".$s_file." | ".$i_line." | ".$_SERVER["REMOTE_ADDR"]." | ".$s_error." | ".$s_detail."\r\n";
	fputs($fp,$output);
	fclose($fp);
	//exit("An unexpected error has occured, please call for support.\r\n Page: ".$page."\r\n Line:".$line);
}

function lib_getClosestGeocode($i_latin, $i_lonin,$a_geocode) {
	$s_return = "";
	
	for ($i=0;$i<sizeof($a_geocode);$i++) {
		$a_row = $a_geocode[$i];
		$i_distance = lib_distance($i_latin, $i_lonin, $a_row['lat'], $a_row['lon']);
		if ($i_distance < .5) {
			$s_return = "".$a_row['name'];
			break;
		}
	}

	return $s_return;
}

function lib_getClosestGeocode_fuzzy($i_latin, $i_lonin,$a_geocode) {
	$s_return = "";
	$s_hit = "";
	$s_near = "";
	
	for ($i=0;$i<sizeof($a_geocode);$i++) {
		$a_row = $a_geocode[$i];
		$i_distance = lib_distance($i_latin, $i_lonin, $a_row['lat'], $a_row['lon']);
		if ($i_distance <= 1 && $s_hit == "") {
			$s_hit = "Stopped At: ".$a_row['name'];
		}
		if ($i_distance > 1 && $i_distance < 20 && $s_near == "") {
			$s_near = 'Stopped Near: '.$a_row['name'].' ('.round($i_distance,0).' miles) '.$a_row['street'];
		}
		if ($s_hit != "" && $s_near != "") {
			break;
		}
	}
	
	if ($s_hit != "") {
		$s_return .= $s_hit;
	}
	if ($s_hit != "" && $s_near != "") {
		$s_return = "<br />";
	}	
	if ($s_near != "") {
		$s_return .= $s_near;
	}
	if (trim($s_return) == "") {
		$s_return = "&nbsp;";
	}

	return $s_return;
}

// returns brightness value from 0 to 255
function lib_get_brightness($hex) { 

	// strip off any leading # 
	$hex = str_replace('#', '', $hex); 
	
	$c_r = hexdec(substr($hex, 0, 2)); 
	$c_g = hexdec(substr($hex, 2, 2)); 
	$c_b = hexdec(substr($hex, 4, 2)); 
	
	return (($c_r * 299) + ($c_g * 587) + ($c_b * 114)) / 1000; 
} 

function lib_open_db($s_db="gps",$server="CHA-SSQL03,1433",$username="sql_remote",$password="0F5jG1tM2z") {
	$sqlconnect=mssql_connect($server, $username, $password);
	if (!$sqlconnect) {lib_die_well(mssql_error(),"",__FILE__,__LINE__);}
	$sqldb=mssql_select_db($s_db,$sqlconnect);
	if (!$sqldb) {lib_die_well(mssql_error(),"",__FILE__,__LINE__);}

	return $sqldb;
}

function lib_pad_html($s_in) {
	$s_out = $s_in;
	if ($s_in == "") {
		$s_out = "&nbsp;";
	}
	return $s_out;
}

function lib_split_pair($s_instring,$s_delimiter,$s_parmWanted) {
	$s_return = "";
	$i_length = strlen($s_instring);
	$i_spot = stripos($s_instring,$s_delimiter);
	$s_param = substr($s_instring,0,$i_spot);
	$s_value = substr($s_instring,($i_spot),($i_length-$i_spot));
	if (substr($s_value,0,1) == $s_delimiter) {
		$s_value = substr($s_instring,($i_spot+1),($i_length-$i_spot));
	}
	if ($s_parmWanted > "" && strtolower($s_param) == strtolower($s_parmWanted)) {
		$s_return = $s_value;
	}
	
	return $s_return;
}

function lib_set_var($testName,$s_default) {
	$varName=$s_default;
	
	if (isset($_REQUEST[$testName])) {
		$varName = $_REQUEST[$testName];
	} else { 
		if (isset($_POST[$testName])) {
			$varName = $_POST[$testName];
		}
	}

	return $varName;
}

function lib_sql_safe($inString) {
	$outString = str_replace("'", "''",$inString);
	$outString = utf8_encode($outString);
	
	return $outString;
}

function lib_sqlpp($InString,$_title="") {
	$s_pretty = '<div style="background-color: #DCDCDC;border:1px solid black;font-family: courier;">';
	if ($_title != "") {$s_pretty .= '<div style="font-weight: bold;">'.$_title.'</div>';}
	$s_pretty .= str_replace("\n","<br />",$InString);
	$s_pretty .= '</div>';
	return $s_pretty;
}
function lib_sqlpp2($InString) 	{return str_replace("\n","<br />",$InString);}

function lib_random_color(){
    mt_srand((double)microtime()*1000000);
    $c = '';
    while(strlen($c)<6){
        $c .= sprintf("%02X", mt_rand(0, 255));
    }
    return $c;
}

function lib_secToTime($time,$b_showSeconds=false){
	$s_return = "";
  if(is_numeric($time)){
    $value = array("years" => 0, "days" => 0, "hours" => 0, "minutes" => 0, "seconds" => 0,);
    if($time >= 31556926){
      $i_years = floor($time/31556926);
      $time = ($time%31556926);
	  $s_return .= $i_years."y ";
    }
    if($time >= 86400){
      $i_days = floor($time/86400);
      $time = ($time%86400);
	  $s_return .= $i_days."d ";
    }
    if($time >= 3600){
      $i_hours = floor($time/3600);
      $time = ($time%3600);
	  $s_return .= $i_hours."h ";
    }
    if($time >= 60){
      $i_minutes = floor($time/60);
	  if ($b_showSeconds) {
		$time = ($time%60);
	  }
	  $s_return .= $i_minutes."m ";
    }
	if($time < 60){
		if ($b_showSeconds) {
			$i_seconds = floor($time);
			$s_return .= $i_seconds."s ";
		} else {
			$s_return .= "0m";
		}
	}
  }
  
  return trim($s_return);
}

function lib_time2sec($time='00:00:00'){
  list($hr,$m,$s) = explode(':', $time);
  return ( (int)$hr*3600 ) + ( (int)$m*60 ) + (int)$s;
}

function lib_write_file($s_fileName,$s_text) {
	$fp=fopen($s_fileName,"a");
	$output = date('Y-m-d H:i:s')." | ".$s_text."\r\n";
	fputs($fp,$output);
	fclose($fp);
}
function lib_write_line($s_fileName,$s_text) 
	{
	$fp=fopen($s_fileName,"a");
	fputs($fp,$s_text);
	fclose($fp);
	}
function lib_get_month($i_month)
	{
	$s_retval = "";
	switch($i_month)
		{
		case 1:
			$s_retval = "January";
			break;
		case 2:
			$s_retval = "February";
			break;
		case 3:
			$s_retval = "March";
			break;
		case 4:
			$s_retval = "April";
			break;
		case 5:
			$s_retval = "May";
			break;
		case 6:
			$s_retval = "June";
			break;
		case 7:
			$s_retval = "July";
			break;
		case 8:
			$s_retval = "August";
			break;
		case 9:
			$s_retval = "September";
			break;
		case 10:
			$s_retval = "October";
			break;
		case 11:
			$s_retval = "November";
			break;
		case 12:
			$s_retval = "December";
			break;
		}
	return $s_retval;
	}
/*if ($argc > 1) {
	for ($i=1;$i<$argc;$i++) {
		echo $argv[$i];
		$codes[] = strtoupper($argv[$i]);
	}
}
*/
?>
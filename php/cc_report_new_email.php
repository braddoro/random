<?php
$s_sendTo = "";
$s_sendCC = "";
$s_sendBCC = "";
$s_body_link = "<a href='http://cha-ssql03/agent/cc_report_filter.php'>link to interactive reporting</a>";

$s_sql = "
select 
	L.fullName, 
	L.address, 
	A.sendType 
from nightly_update.dbo.email_list L 
inner join nightly_update.dbo.email_app A 
	on L.emailID = A.emailID 
	and A.applicationID = 8 
	and L.active = 1
	and A.active = 1
order by 
	L.fullName, 
	L.address, 
	A.sendType
";
$o_conn = mssql_connect("CHA-SSQL03,1433","data_writer","hWCiKNs1U4");
mssql_select_db("nightly_update",$o_conn);
$q_data = mssql_query($s_sql,$o_conn);
while($row = mssql_fetch_object($q_data))
	{
	$fullName = $row->fullName;
	$address = $row->address;
	$sendType = strtoupper($row->sendType);
	$s_email = trim("$address,");
	switch($sendType)
		{
		case "BCC":
			$s_sendBCC .= $s_email;
			break;
		case "CC":
			$s_sendCC .= $s_email;
			break;
		default:
			$s_sendTo .= $s_email;
			break;
		}
	}
mssql_free_result($q_data);
mssql_close($o_conn);

$s_sendTo = chop($s_sendTo,",");
$s_sendCC = chop($s_sendCC,",");
$s_sendBCC = chop($s_sendBCC,",");

exec("cc_report_new_cli.bat");
echo "$s_sendTo \n $s_sendCC \n $s_sendBCC \n";

$body = file_get_contents("cc_report_new.htm");
$body .= $s_body_link;
$body .= "";
//$ = "brad.hughes@husqvarna.com";
if ($body > "" && $s_sendTo > "")
	{
	$s_sendFrom = "reports@huskysales.com";
	$subject = "Call Center Reporting";
	$headers  = 'MIME-Version: 1.0'."\r\n";
	$headers .= 'To: '.$s_sendTo."\r\n";
	$headers .= 'From: reports@huskysales.com' . "\r\n";
	//$headers .= "Reply-To: ".$s_sendFrom."\r\n";
	//$headers .= "Return-Path: ".$s_sendFrom."\r\n";
	if (trim($s_sendCC) > "")
		{
		$headers .= "CC: ".$s_sendCC."\r\n";
		}
	if (trim($s_sendBCC) > "")
		{
		$headers .= "BCC: ".$s_sendBCC."\r\n";
		}
	//$s_body = file_get_contents("sd_daily_dashboard.php.htm");
	$headers .= 'Content-Type: text/html; charset="iso-8859-1"' . "\r\n";
	$headers .= 'Content-Type: text/html; name="report.htm"' . "\r\n";
	$headers .= 'Content-Transfer-Encoding: 7bit'."\r\n";
	$headers .= 'Content-Disposition: attachment; file="report.htm"' . "\r\n";
	echo mail($s_sendTo,$subject,$body,$headers);
	}
?>
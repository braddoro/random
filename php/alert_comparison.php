<?php
ini_set("memory_limit","2048M");
set_time_limit(0);
$i_sleepTime = 1;
$i_timeGroup = 0;
$s_CC = "";
$i_send_email = 1;
$i_email_list = 7;
if (isset($argv[1])) {$i_timeGroup = intval($argv[1]);}
if (isset($argv[2])) {if($argv[2] != "ALL") {$s_CC = "and CompanyCode = '$argv[2]'";}}
if (isset($argv[3])) {$i_send_email = intval($argv[3]);}
if (isset($argv[4])) {$i_email_list = intval($argv[4]);}
$a_control_data = array();
$b_force = false;
lib_logit("clearit_");
$s_output = "";
function lib_logit($message)
	{
	global $s_output;
	$s_logfile = "log\\".basename(__FILE__)."_log.txt";
	if ($message == "clearit_")
		{
		if (file_exists($s_logfile)) {unlink($s_logfile);}
		}else{
		$s_message = str_replace("\0","",$message);
		$fp=fopen($s_logfile,"a");
		if (!$fp)
			{
			echo("Cant write to log file: $s_logfile\n");
			}else{
			fputs($fp,$s_message."\n");
			fclose($fp);
			$s_output .= $message."\n";
			}
		}
	}
function f_control_array($o_recT)
	{
	$a_temp = array();
	while ($rowT = mssql_fetch_array($o_recT))
		{
		$companyCode = $rowT["companyCode"];
		$compareType = $rowT["compareType"];
		$validCodes = $rowT["validCodes"];
		$org_table = $rowT["org_table"];
		$sales_details = $rowT["sales_details"];
		$csv_export_3 = $rowT["csv_export_3"];
		$crm_odbc_server = $rowT["crm_odbc_server"];
		$crm_host_name = $rowT["crm_host_name"];
		$entry = array();
		$entry[] = trim($compareType);
		$entry[] = trim($validCodes);
		$entry[] = trim($crm_odbc_server);
		$entry[] = trim($crm_host_name);
		$a_temp[$companyCode] = $entry;
		}
	return $a_temp;
	}
function f_gettimeGroup($i_timeGroup)
	{
	switch($i_timeGroup)
		{
		case 1:
			$s_timeGroup = " and timeGroup = '1-Europe' ";
			break;
		case 2:
			$s_timeGroup = " and timeGroup = '2-Americas' ";
			break;
		case 3:
			$s_timeGroup = " and timeGroup = '3-Pacific' ";
			break;
		case 4:
			$s_timeGroup = " and timeGroup = '4-Split' ";
			break;
		default:
			$s_timeGroup = "";
			break;
		}
	return $s_timeGroup;
	}
function f_getRegion($companyCode,$table)
	{
	global $a_control_data;
	$s_where = "";
	$compareType = "";
	$validCodes = "";
	if (array_key_exists($companyCode,$a_control_data))
		{
		$compareType = $a_control_data[$companyCode][0];
		$validCodes = $a_control_data[$companyCode][1];
		}
	if (trim($validCodes) > "" && trim($validCodes) > "")
		{
		if ($compareType == "USF_Region" && trim($validCodes) > "")
			{
			switch($table)
				{
				case "dw":
					$s_where = "and RegionNumber in ($validCodes)";
					break;
				case "sd":
					$s_where = "and region_number in ($validCodes)";
					break;
				default:
					$s_where = "and c\$husq_cbdm_territory_num in ($validCodes)";
					break;
				}
			}
		if ($compareType == "Channel")
			{
			switch($table)
				{
				case "dw":
					$s_where = "and SalesChannel in ($validCodes)";
					break;
				case "sd":
					$s_where = "and territory_desc in ($validCodes)";
					break;
				default:
					$s_where = "and c\$src_sales_channel in ($validCodes)";
					break;
				}
			}
		if ($compareType == "Region" && trim($validCodes) > "")
			{
			switch($table)
				{
				case "dw":
					$s_where = "and RegionNumber in ($validCodes)";
					break;
				case "sd":
					$s_where = "and region_number in ($validCodes)";
					break;
				default:
					$s_where = "and c\$region_number in ($validCodes)";
					break;
				}
			}
		}
	return $s_where;
	}
function f_getDW_data($companyCode,$i_recordType,$validCodes,$compareType)
	{
	$o_conn=mssql_connect("CHA-SSQL05.huskyclt.com,1433","crm","seeaream");
	mssql_select_db("RightNow_CRM",$o_conn);
	$s_where = f_getRegion($companyCode,"dw");
	$s_sql = "select * from dbo.tblSales_RecordType$i_recordType where CompanyCode = '$companyCode' $s_where order by CompanyCode, AccountNumber, GRA, RegionNumber, SalesChannel";
	$a_data = array();
	$o_data = mssql_query($s_sql,$o_conn);
	if (!$o_data) 
		{
		echo "DW SQL Error.\n";
		return $a_data;
		}
	$i_rows = mssql_num_rows($o_data);
	$i_row = 0;
	while($row = mssql_fetch_object($o_data))
		{
		$RecordType			= trim($row->RecordType);
		$CompanyCode		= trim($row->CompanyCode);
		$AccountNumber		= trim($row->AccountNumber);
		$GRA				= trim($row->GRA);
		$RegionNumber		= trim($row->RegionNumber);
		$SalesChannel		= trim($row->SalesChannel);
		$ChainHead			= trim($row->ChainHead);
		$RegionName			= trim($row->RegionName);
		$TerritoryName		= trim($row->TerritoryName);
		$TerritoryNumber	= trim($row->TerritoryNumber);
		$SuperCategory		= trim($row->SuperCategory);
		$ProductGroup		= trim($row->ProductGroup);
		$SubGroup			= trim($row->SubGroup);
		$Model				= trim($row->Model);
		$SalesYTD			= trim($row->SalesYTD);
		$SalesMTD			= trim($row->SalesMTD);
		$SalesYTD_PrevYear	= trim($row->SalesYTD_PrevYear);
		$SalesMTD_PrevYear	= trim($row->SalesMTD_PrevYear);
		$SalesPrevYear		= trim($row->SalesPrevYear);
		$SalesMTD_PrevMonth	= trim($row->SalesMTD_PrevMonth);
		$OpenOrderTotal		= trim($row->OpenOrderTotal);
		$BackOrder			= trim($row->BackOrder);
		$OpenOrderFuture	= trim($row->OpenOrderFuture);
		$BrandCode			= trim($row->BrandCode);
		if ($SalesChannel == "" || $SalesChannel == "N/A") {$SalesChannel_key = "X";}else{$SalesChannel_key = $SalesChannel;}
		if ($RegionNumber == "" || $RegionNumber == "N/A") {$RegionNumber_key = "X";}else{$RegionNumber_key = $RegionNumber;}
		if ($validCodes == "")
			{
			$SalesChannel_key = "X";
			$RegionNumber_key = "X";
			}
		if ($compareType == "Region")
			{
			$RegionNumber_key = $RegionNumber;
			$SalesChannel_key = "X";
			}
		if ($compareType == "Channel")
			{
			$SalesChannel_key = $SalesChannel;
			$RegionNumber_key = "X";
			}
		if ($companyCode == "GBS" && strlen($TerritoryNumber) == 7)
			{
			$RegionNumber_key = "X";
			$RegionNumber = substr($TerritoryNumber,0,3);
			$TerritoryNumber = substr($TerritoryNumber,4,3);
			}
		if ($companyCode == "USF")
			{
			if ($AccountNumber == '25568' || $AccountNumber == '85568')
				{
				$RegionNumber 	= "74";
				}
			$AccountNumber 		= intval($AccountNumber);
			$GRA				= intval($GRA);
			$RegionNumber_key 	= "X";
			$SalesChannel_key 	= "X";
			}
		if ($companyCode == "CDA")
			{
			$AccountNumber 		= substr("00000".intval($AccountNumber),-5);
			$GRA				= intval($GRA);
			$RegionNumber_key 	= substr("00".intval($RegionNumber),-2);
			$SalesChannel_key 	= "X";
			}
		if ($companyCode == "UAS")
			{
			$AccountNumber 		= intval($AccountNumber);
			}
		$s_key = $CompanyCode."_".$AccountNumber."_".trim($GRA)."_".$RegionNumber_key."_".$SalesChannel_key;
		$entry = array();
		$entry[] = round($SalesYTD,2);			// 0
		$entry[] = round($SalesYTD_PrevYear,2);	// 1
		if (!isset($a_data[$s_key])) {$a_data[$s_key] = array();}
		$a_data[$s_key][] = $entry;
		$i_row++;
		}
	mssql_free_result($o_data);
	return $a_data;
	}
function f_getSD_data($companyCode,$i_record_type,$o_conn,$validCodes,$compareType)
	{
	$s_where = f_getRegion($companyCode,"sd");
	$s_sql = "select record_type, org_id, company_code, account_number, gra, region_number, territory_number, territory_desc as 'sales_channel', sum(convert(lytd,decimal(14,2))) as 'lytd', sum(convert(ytd,decimal(14,2))) as 'ytd', max(update_date) as 'update_date' from ps_sales_details d where company_code = '$companyCode' and d.record_type = $i_record_type $s_where group by record_type, org_id, company_code, account_number, gra, region_number, territory_number, territory_desc order by company_code, account_number, gra, region_number, territory_desc";
	$a_data = array();
	$o_data=odbc_exec($o_conn,$s_sql);
	if (!$o_data) {echo "ORG SQL Error.\n";return $a_data;}
	$i_rows = odbc_num_rows($o_data);
	while($row = odbc_fetch_object($o_data))
		{
		$record_type	= trim($row->record_type);
		$orgID			= trim($row->org_id);
		$CompanyCode	= trim($row->company_code);
		$AccountNumber	= trim($row->account_number);
		$GRA			= trim($row->gra);
		$RegionNumber	= trim($row->region_number);
		$TerritoryNumber= trim($row->territory_number);
		$SalesChannel	= trim($row->sales_channel);
		$lytd			= trim($row->lytd);
		$ytd			= trim($row->ytd);
		$UpdateDate		= trim($row->update_date);
		if ($SalesChannel == "" || $SalesChannel == "N/A") {$SalesChannel_key = "X";}else{$SalesChannel_key = $SalesChannel;}
		if ($RegionNumber == "" || $RegionNumber == "N/A") {$RegionNumber_key = "X";}else{$RegionNumber_key = $RegionNumber;}
		if ($validCodes == "")
			{
			$SalesChannel_key = "X";
			$RegionNumber_key = "X";
			}
		if ($compareType == "Region")
			{
			$SalesChannel_key = "X";
			}
		if ($compareType == "Channel")
			{
			$RegionNumber_key = "X";
			}
		if ($companyCode == "UAS")
			{
			$AccountNumber 		= intval($AccountNumber);
			}
		if ($companyCode == "USF")
			{
			if ($AccountNumber == '25568' || $AccountNumber == '85568')
				{
				$RegionNumber 	= "74";
				}
			$AccountNumber 		= intval($AccountNumber);
			$GRA				= intval($GRA);
			$RegionNumber_key 	= "X";
			$SalesChannel_key 	= "X";
			}
		if ($companyCode == "CDA")
			{
			$AccountNumber 		= substr("00000".intval($AccountNumber),-5);
			$GRA				= intval($GRA);
			$RegionNumber_key 	= substr("00".intval($RegionNumber),-2);
			$SalesChannel_key 	= "X";
			}
		$s_key = $CompanyCode."_".$AccountNumber."_".trim($GRA)."_".$RegionNumber_key."_".$SalesChannel_key;
		$entry = array();
		$entry[] = round($ytd,2);	// 0
		$entry[] = round($lytd,2);	// 1
		$a_data[$s_key] = $entry;
		}
	odbc_free_result($o_data);
	return $a_data;
	}
/* -=- Start Processing -=- */
$o_conn4 = mssql_connect("localhost,1433","phpsql","phpsql");
if (!$o_conn4) {lib_logit("Unable to open control data.");exit(1);}
$db = mssql_select_db("crm_logs",$o_conn4);
if (!$db) {lib_logit("Unable to choose db.");exit(1);}
$s_TG = f_gettimeGroup($i_timeGroup);
$s_sql = "select country, timeGroup, companyCode, compareType, validCodes, org_table, sales_details, csv_export_3, csv_export_4, crm_odbc_server, crm_host_name from dbo.global_update_control where active = 1 $s_CC $s_TG order by country, companyCode, updateOrder";
$o_control_data = mssql_query($s_sql,$o_conn4);
if (!$o_control_data) {lib_logit("Unable to query control data.");exit(1);}
$i_company_count = mssql_num_rows($o_control_data);
if ($i_company_count == 0) {echo "No Companies in list.";exit();}
$a_control_data = f_control_array($o_control_data);
mssql_data_seek($o_control_data,0);
$i_curr_company = 1;
while ($row_loop = mssql_fetch_array($o_control_data))
	{
	$crm_odbc_server= trim($row_loop["crm_odbc_server"]);
	$crm_host_name	= trim($row_loop["crm_host_name"]);
	$sales_details	= trim($row_loop["sales_details"]);
	$csv_export_3	= trim($row_loop["csv_export_3"]);
	$csv_export_4	= trim($row_loop["csv_export_4"]);
	$compareType	= trim($row_loop["compareType"]);
	$companyCode	= trim($row_loop["companyCode"]);
	$validCodes		= trim($row_loop["validCodes"]);
	$timeGroup		= trim($row_loop["timeGroup"]);
	$country		= trim($row_loop["country"]);
	$o_CRM 			= odbc_connect($crm_odbc_server,"","");
	if (!$o_CRM) {lib_logit("Unable to open CRM data.");continue;}
	$a_DW_RT1		= f_getDW_data($companyCode,1,$validCodes,$compareType);
	$a_SD_RT1		= f_getSD_data($companyCode,1,$o_CRM,$validCodes,$compareType);
	$i_row_dw		= count($a_DW_RT1);
	$i_row_sd		= count($a_SD_RT1);
	$a_keys			= array_keys($a_DW_RT1);
	$i_row			= 1;
	$i_all_good		= 0;
	$i_not_matching	= 0;
	$i_sd_missing	= 0;
	foreach($a_keys as $key)
		{
		if (array_key_exists($key,$a_SD_RT1))
			{
			$a_line	= $a_DW_RT1[$key];
			$i_ytd	= $a_SD_RT1[$key][0];
			$i_lytd	= $a_SD_RT1[$key][1];
			foreach($a_line as $model)
				{
				if (round($i_ytd,2) == round($model[0],2) && round($i_lytd,2) == round($model[1],2))
					{
					$i_all_good++;
					}else{
					lib_logit("Nomatch: $key |".round($i_lytd,2)."|=|".round($model[1],2)."| |".round($i_ytd,2)."|=|".round($model[0],2)."|");
					$i_not_matching++;
					}
				}
			}else{
			lib_logit("Missed.: $key");
			$i_sd_missing++;
			}
		$i_row++;
		}
	//if ($i_company_count > 1) {echo "$companyCode: $i_curr_company of $i_company_count\n";}
	if ($i_row_sd != $i_row_dw || $i_sd_missing > 0 || $i_not_matching > 0)
		{
		lib_logit("Code........: \t$companyCode");
		lib_logit("Country.....: \t$country");
		lib_logit("DW Rows.....: \t$i_row_dw");
		lib_logit("SD Rows.....: \t$i_row_sd");
		lib_logit("Good Accts..: \t$i_all_good");
		lib_logit("SD Missing..: \t$i_sd_missing");
		lib_logit("Not Matching: \t$i_not_matching");
		if ($i_curr_company < $i_company_count) {lib_logit("");}
		}else{
		//lib_logit("$companyCode All Good.");
		}
	unset($a_orgs);
	unset($a_SD_RT1);
	unset($a_DW_RT1);
	unset($a_keys);
	$i_curr_company++;
	}
if ($i_send_email == 1)
	{
	$s_table = urlencode($s_output);
	$s_title = urlencode("Sales Detail Data Warehouse to Sales Detail Comparison.");
	$s_url = "http://cha-ssql03.huskyclt.com/email/em.php?a=$i_email_list&b=$s_table&c=$s_title";
	$s_open = file($s_url);
	if (isset($s_open)) {foreach($s_open as $s_line) {echo $s_line;}}
	}else{
	echo $s_output;
	}
exit(0);
?>
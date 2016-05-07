<?php
/*
RT1: $ Summary
RT2: $ by Group
RT3: $ by Model
RT4: Qty by Model
RT5: Qty by Model, Brand Article
RT7: Qty by Group
*/
include_once("..\lib\lib_curl_send.php");
echo date("H:i:s")." Start.\n";
ini_set("memory_limit","3072M");
ini_set('output_buffering',"32768");
set_time_limit(0);
$i_req_timeout	= 300;
$i_row_mod		= 500;
$i_sleepTime	= 60;
$i_timeGroup	= 0;
$s_CC			= "";
$i_acct_split	= 0;
$b_force		= false;
$a_control_data	= array();
if (isset($argv[1])) {$i_timeGroup = intval($argv[1]);}
if (isset($argv[2])) {$s_CC = $argv[2];}
if (isset($argv[3])) {$i_acct_split = $argv[3];}
if (isset($argv[4])) {if (intval($argv[4]) == 1) {$b_force = true;}}
lib_logit("clearit_");
lib_logit("clearit_","orgs");
lib_logit("clearit_","error");
lib_logit("clearit_","failed");
lib_logit(var_export($argv,true));
echo var_export($argv,true)."\n";
function lib_logit_daily($message,$s_suffix="")
	{
	if ($s_suffix > "") {$s_suffix = "_$s_suffix";}
	$s_logfile = "log\\".basename(__FILE__)."_log_".date("N")."$s_suffix.txt";
	$s_date = date("Y-m-d H:i:s");
	if ($message == "clearit_")
		{
		if (file_exists($s_logfile)) {unlink($s_logfile);}
		}else{
		$s_message = str_replace("\0","",$message);
		$fp=fopen($s_logfile,"a");
		fputs($fp,$s_date." ".$s_message."\n");
		fclose($fp);
		}
	}
function lib_logit($message,$s_suffix="",$b_skipStamp=false)
	{
	global $i_timeGroup, $s_CC, $i_acct_split;
	if ($i_acct_split > "") {$s_suff2 = "_".$s_CC."_".$i_acct_split;}else{$s_suff2 = "";}
	
	if ($s_suffix > "") {$s_suffix = "_$s_suffix";}
	$s_logfile = "log\\".basename(__FILE__)."_log_TG".$i_timeGroup.$s_suff2.$s_suffix.".txt";
	$s_date = date("Y-m-d H:i:s");
	if ($b_skipStamp) {$s_date="";}
	if ($message == "clearit_")
		{
		if (file_exists($s_logfile)) {unlink($s_logfile);}
		}else{
		$s_message = str_replace("\0","",$message);
		$fp=fopen($s_logfile,"a");
		fputs($fp,$s_date." ".$s_message."\n");
		fclose($fp);
		}
	}
function f_clean_me($invalue)
	{
	$outvalue = $invalue;
	$outvalue = str_replace("\\","\\\\",$outvalue);
	$outvalue = str_replace("'","\'",$outvalue);
	$outvalue = utf8_encode($outvalue);
	return $outvalue;
	}
function f_doneYet($i_recordType=1,$i_sleepTime=60)
	{
	do	{
		lib_logit("Sleeping for: $i_sleepTime on $i_recordType");
		sleep($i_sleepTime);
		} while (!f_checkCompletion($i_recordType));
	}
function f_get_maxID($o_CRM,$companyCode)
	{
	if ($companyCode == "UAS")
		{
		$s_sql = "select max(id) as 'id' from CO\$ps_sales_details";
		}else{
		$s_sql = "select max(id) as 'id' from ps_sales_details";
		}
	$id = 0;
	$o_res = odbc_exec($o_CRM,$s_sql);
	while(odbc_fetch_row($o_res)) {$id = intval(odbc_result($o_res,1));}
	$id++;
	if ($id < 1) {$id = 1;}
	return $id;
	}
function f_open_db()
	{
	$server="CHA-SSQL05.huskyclt.com,1433";
	$username="crm";
	$password="seeaream";
	$usedb="RightNow_CRM";
	$sqlconnect=mssql_connect($server,$username,$password);
	$sqldb=mssql_select_db($usedb,$sqlconnect);
	return $sqlconnect;
	}
function f_open_db4()
	{
	$server="localhost,1433";
	$username="phpsql";
	$password="phpsql";
	$usedb="crm_logs";
	$sqlconnect=mssql_connect($server,$username,$password);
	$sqldb=mssql_select_db($usedb,$sqlconnect);
	return $sqlconnect;
	}
function f_close_db($sqlconnect) {mssql_close($sqlconnect);}
function f_checkCompletion($i_recordType)
	{
	$s_today = date("Y-m-d")." 00:00:00";
	$b_checkCompletion = false;
	$sqlconnect = f_open_db();
	if ($i_recordType == 0)
		{
		$s_sql = "select Completed from dbo.LoadCompleted where [Table] = 'tblCustomer' and Started > '$s_today' and Completed is not null";
		}else{
		$s_sql = "select Completed from dbo.LoadCompleted where [Table] = 'tblSales_RecordType$i_recordType' and Started > '$s_today' and Completed is not null";
		}
	lib_logit($s_sql);
	$result = mssql_query($s_sql,$sqlconnect);
	while ($row = mssql_fetch_array($result)) {$b_checkCompletion = true;}
	f_close_db($sqlconnect);
	return $b_checkCompletion;
	}
function f_control_array($o_recT)
	{
	$a_temp = array();
	while ($rowT = mssql_fetch_array($o_recT))
		{
		$companyCode	= $rowT["companyCode"];
		$compareType	= $rowT["compareType"];
		$validCodes		= $rowT["validCodes"];
		$org_table		= $rowT["org_table"];
		$sales_details	= $rowT["sales_details"];
		$csv_export_3	= $rowT["csv_export_3"];
		$crm_odbc_server= $rowT["crm_odbc_server"];
		$crm_host_name	= $rowT["crm_host_name"];
		$entry		= array();
		$entry[] 	= trim($compareType);
		$entry[] 	= trim($validCodes);
		$entry[] 	= trim($crm_odbc_server);
		$entry[] 	= trim($crm_host_name);
		$a_temp[$companyCode] = $entry;
		}
	lib_logit(var_export($a_temp,true));
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
function f_comp_split ($companyCode,$i_acct_split,$s_table)
	{
	$s_filter = "";
	switch($s_table)
		{
		case "dw":
			$sfield = "AccountNumber";
			break;
		case "sd":
			$sfield = "account_number";
			break;
		default:
			$sfield = "c\$account_number";
			break;
		}
	switch($companyCode)
		{
		case "USF":
			switch($s_table)
			{
			case "dw":
				$sfield = "RegionNumber";
				break;
			case "sd":
				$sfield = "region_number";
				break;
			default:
				$sfield = "c\$husq_cbdm_territory_num";
				break;
			}
			if ($i_acct_split == -1)
				{
				switch($s_table)
					{
					case "dw":
						$sfield = "AccountNumber";
						break;
					case "sd":
						$sfield = "account_number";
						break;
					default:
						$sfield = "c\$account_number";
						break;
					}
				$s_filter = "and $sfield = '23530'";
				}
			if ($i_acct_split == 1)
				{
				$s_filter = "and $sfield in ('01','001','0001')";
				}
			if ($i_acct_split == 2)
				{
				$s_filter = "and $sfield in ('02','002','0002')";
				}
			if ($i_acct_split == 3)
				{
				$s_filter = "and $sfield in ('03','003','0003')";
				}
			if ($i_acct_split == 4)
				{
				$s_filter = "and $sfield in ('04','004','0004')";
				}
			if ($i_acct_split == 5)
				{
				$s_filter = "and $sfield in ('05','005','0005')";
				}
			if ($i_acct_split == 6)
				{
				$s_filter = "and $sfield in ('06','006','0006')";
				}
			if ($i_acct_split == 7)
				{
				$s_filter = "and $sfield in ('07','007','0007')";
				}
			if ($i_acct_split == 8)
				{
				$s_filter = "and $sfield in ('08','008','0008')";
				}
			if ($i_acct_split == 74)
				{
				$s_filter = "and $sfield in ('74','074','0074')";
				}
			break;
		case "CDA":
			switch($s_table)
			{
			case "dw":
				$sfield = "RegionNumber";
				break;
			case "sd":
				$sfield = "region_number";
				break;
			default:
				$sfield = "c\$husq_cbdm_territory_num";
				break;
			}
			if ($i_acct_split == 1)
				{
				$s_filter = "and $sfield in ('01','001')";
				}
			if ($i_acct_split == 3)
				{
				$s_filter = "and $sfield in ('03','003')";
				}
			if ($i_acct_split == 4)
				{
				$s_filter = "and $sfield in ('04','004')";
				}
			break;
		case "FRY":
			switch($s_table)
				{
				case "dw":
					$sfield = "RegionNumber";
					break;
				case "sd":
					$sfield = "region_number";
					break;
				default:
					$sfield = "c\$region_number";
					break;
				}
			if ($i_acct_split == 1)	 {$s_filter = "and $sfield = '22101'";}
			if ($i_acct_split == 2)	 {$s_filter = "and $sfield = '22104'";}
			if ($i_acct_split == 3)	 {$s_filter = "and $sfield = '22001'";}
			if ($i_acct_split == 4)	 {$s_filter = "and $sfield = '22103'";}
			if ($i_acct_split == 5)	 {$s_filter = "and $sfield = '22102'";}
			if ($i_acct_split == 6)	 {$s_filter = "and $sfield = '22002'";}
			if ($i_acct_split == 7)	 {$s_filter = "and $sfield = '22003'";}
			if ($i_acct_split == 8)	 {$s_filter = "and $sfield = '22009'";}
			if ($i_acct_split == 9)	 {$s_filter = "and $sfield in ('N/A','22005','22100','20002','22109','22000')";}
			break;
		case "FR1":
			switch($s_table)
				{
				case "dw":
					$sfield = "RegionNumber";
					break;
				case "sd":
					$sfield = "region_number";
					break;
				default:
					$sfield = "c\$region_number";
					break;
				}
			if ($i_acct_split == 1)	 {$s_filter = "and $sfield = '22002'";}
			if ($i_acct_split == 2)	 {$s_filter = "and $sfield = '22001'";}
			if ($i_acct_split == 3)	 {$s_filter = "and $sfield = '22003'";}
			if ($i_acct_split == 4)	 {$s_filter = "and $sfield in ('N/A','22102','22101','22103','22104','22004')";}
			break;
		case "FDS":
			switch($s_table)
				{
				case "dw":
					$sfield = "TerritoryNumber";
					break;
				case "sd":
					$sfield = "territory_number";
					break;
				default:
					$sfield = "c\$territory_number";
					break;
				}
			if ($i_acct_split == 1)		{$s_filter = "and $sfield = '4FS057'";}
			if ($i_acct_split == 2)		{$s_filter = "and $sfield = '4FS032'";}
			if ($i_acct_split == 3)		{$s_filter = "and $sfield = '4FS038'";}
			if ($i_acct_split == 4)		{$s_filter = "and $sfield = '4FS039'";}
			if ($i_acct_split == 5)		{$s_filter = "and $sfield = '4FS055'";}
			if ($i_acct_split == 6)		{$s_filter = "and $sfield = '4FS034'";}
			if ($i_acct_split == 7)		{$s_filter = "and $sfield = '4FS059'";}
			if ($i_acct_split == 8)		{$s_filter = "and $sfield = '4FS056'";}
			if ($i_acct_split == 9)		{$s_filter = "and $sfield = '4FS005'";}
			if ($i_acct_split == 10)	{$s_filter = "and $sfield = '4FS006'";}
			if ($i_acct_split == 11)	{$s_filter = "and $sfield = '4FS015'";}
			if ($i_acct_split == 12)	{$s_filter = "and $sfield in ('4FS068','4FS067','1FS024','1FS012','4FS065','1FS025','4FS066','1FS022','N/A','4FS071','1FS0240','1FS0120','4FS099','4FS098','4FS069','1FS017','4FS008','4FS095','4FS999','4FS060','4FS061','4FS062','4FS100','4FS101','4FS216')";}
			break;
		case "UAS":
			switch($s_table)
			{
			case "dw":
				$sfield = "substring(TerritoryNumber,1,3)";
				break;
			case "sd":
				$sfield = "region_number";
				break;
			default:
				$sfield = "c\$region_number";
				break;
			}
			$s_filter = "and $sfield = '$i_acct_split'";
			break;
		case "OSH":
			if ($i_acct_split == 1)	{$s_filter = "and $sfield <= '5669'";}
			if ($i_acct_split == 2)	{$s_filter = "and $sfield > '5669'";}
			break;
		case "GBS":
			 if ($i_acct_split == 1)
				 {
				switch($s_table)
					{
					case "dw":
						$sfield = "AccountNumber";
						break;
					case "sd":
						$sfield = "account_number";
						break;
					default:
						$sfield = "c\$account_number";
						break;
					}
				//$s_filter = "and $sfield in ('100020')";
				 }
			break;
		default:
			$s_filter = "";
			break;
		}
	return $s_filter;
	}
function f_getDW_data($companyCode,$i_recordType,$validCodes,$compareType,$i_acct_split)
	{
	$o_conn=mssql_connect("CHA-SSQL05.huskyclt.com,1433","crm","seeaream");
	mssql_select_db("RightNow_CRM",$o_conn);
	$s_where = f_getRegion($companyCode,"dw");
	$s_split = f_comp_split($companyCode,$i_acct_split,"dw");
	$s_sql = "select * from dbo.tblSales_RecordType$i_recordType where CompanyCode = '$companyCode' $s_where $s_split order by CompanyCode, AccountNumber, GRA, RegionNumber, SalesChannel";
	lib_logit($s_sql);
	$a_data = array();
	$o_data = mssql_query($s_sql,$o_conn);
	if (!$o_data) 
		{
		lib_logit("DW SQL Error.");
		return $a_data;
		}
	$i_rows = mssql_num_rows($o_data);
	lib_logit("RT: $i_recordType DW Rows to Export: $i_rows.");
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
		$SalesYTD			= round($row->SalesYTD,2);
		$SalesMTD			= trim($row->SalesMTD);
		$SalesYTD_PrevYear	= round($row->SalesYTD_PrevYear,2);
		$SalesMTD_PrevYear	= trim($row->SalesMTD_PrevYear);
		$SalesPrevYear		= trim($row->SalesPrevYear);
		$SalesMTD_PrevMonth	= trim($row->SalesMTD_PrevMonth);
		$OpenOrderTotal		= trim($row->OpenOrderTotal);
		$BackOrder			= trim($row->BackOrder);
		$OpenOrderFuture	= trim($row->OpenOrderFuture);
		$BrandCode			= trim($row->BrandCode);
		$BrandCode = "H";
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
		if ($companyCode == "GBS")
			{
			if (strlen($TerritoryNumber) == 7)
				{
				$RegionNumber_key = "X";
				$RegionNumber = substr($TerritoryNumber,0,3);
				$TerritoryNumber = substr($TerritoryNumber,4,3);
				}else{
				$RegionNumber_key = "X";
				$TerritoryNumber = "---";
				$RegionNumber = "---";
				}
			if ($SuperCategory == "N/A") {$SuperCategory = "---";}
			if ($ProductGroup == "N/A") {$ProductGroup = "---";}
			}
		if ($companyCode == "USF")
			{
			if ($AccountNumber == '25568' || $AccountNumber == '85568')
				{
				$RegionNumber 		= "74";
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
			$RegionNumber_key 	= substr("00".intval($RegionNumber),-2);
			$SalesChannel_key 	= "X";
			}
		if ($companyCode == "UAS")
			{
			$AccountNumber 		= intval($AccountNumber);
			}
		$s_key = $CompanyCode."_".$AccountNumber."_".trim($GRA)."_".$RegionNumber_key."_".$SalesChannel_key;
		$entry = array();
		$entry[] = $RecordType;					// 0
		$entry[] = $CompanyCode;				// 1
		$entry[] = $AccountNumber;				// 2
		$entry[] = $GRA;						// 3
		$entry[] = $RegionNumber;				// 4
		$entry[] = $SalesChannel;				// 5
		$entry[] = $ChainHead;					// 6
		$entry[] = $RegionName;					// 7
		$entry[] = $TerritoryName;				// 8
		$entry[] = $TerritoryNumber;			// 9
		$entry[] = $SuperCategory;				//10
		$entry[] = $ProductGroup;				//11
		$entry[] = $SubGroup;					//12
		$entry[] = $Model;						//13
		$entry[] = $SalesYTD;					//14
		$entry[] = $SalesMTD;					//15
		$entry[] = $SalesYTD_PrevYear;			//16
		$entry[] = $SalesMTD_PrevYear;			//17
		$entry[] = $SalesPrevYear;				//18
		$entry[] = $SalesMTD_PrevMonth;			//19
		$entry[] = $OpenOrderTotal;				//20
		$entry[] = $BackOrder;					//21
		$entry[] = $OpenOrderFuture;			//22
		$entry[] = $BrandCode;					//23
		if (!isset($a_data[$s_key])) {$a_data[$s_key] = array();}
		$a_data[$s_key][] = $entry;
		$i_row++;
		}
	mssql_free_result($o_data);
	return $a_data;
	}
function f_getOG_data($companyCode,$o_conn,$validCodes,$compareType,$i_acct_split)
	{
	$s_where = f_getRegion($companyCode,"");
	$s_split = f_comp_split($companyCode,$i_acct_split,"");
	switch($companyCode)
		{
		case "USF":
			$s_sql = "select org_id, c\$company_code as 'company_code', c\$account_number as 'account_number', c\$good_receiving_address as 'gra', c\$husq_cbdm_territory_num as 'region_number', c\$husq_mbdm_territory_num as 'territory_number', c\$src_sales_channel as 'sales_channel' from orgs where c\$company_code = '$companyCode' $s_where $s_split order by c\$company_code, c\$account_number, c\$good_receiving_address, c\$husq_cbdm_territory_num, c\$husq_mbdm_territory_num";
			break;
		case "CDA":
			$s_sql = "select org_id, 'CDA' as 'company_code', c\$account_number as 'account_number', c\$good_receiving_address as 'gra', c\$husq_cbdm_territory_num as 'region_number', c\$husq_mbdm_territory_num as 'territory_number', c\$src_sales_channel as 'sales_channel' from orgs where c\$company_id = 1226 $s_where $s_split order by c\$account_number, c\$good_receiving_address, c\$husq_cbdm_territory_num, c\$husq_mbdm_territory_num";
			break;
		case "UAS":
			$s_sql = "select org_id, 'UAS' as 'company_code', c\$account_number as 'account_number', 1 as 'gra', c\$region_number as 'region_number', '' as 'territory_number', c\$src_sales_channel as 'sales_channel' from orgs where c\$company_code IS NULL $s_where $s_split order by c\$company_code, c\$account_number, c\$good_receiving_address, c\$region_number, c\$src_sales_channel";
			break;
		default:
			$s_sql = "select org_id, c\$company_code as 'company_code', c\$account_number as 'account_number', c\$good_receiving_address as 'gra', c\$region_number as 'region_number', '' as 'territory_number', c\$src_sales_channel as 'sales_channel' from orgs where c\$company_code = '$companyCode' $s_where $s_split order by c\$company_code, c\$account_number, c\$good_receiving_address, c\$region_number, c\$src_sales_channel";
			break;
		}
	lib_logit($s_sql);
	$a_data = array();
	$o_data=odbc_exec($o_conn,$s_sql);
	if (!$o_data)
		{
		lib_logit("SD SQL Error.");
		return $a_data;
		}
	$i_rows = odbc_num_rows($o_data);
	lib_logit("Org Rows to Export: $i_rows.");
	while($row = odbc_fetch_object($o_data))
		{
		$orgID			= trim($row->org_id);
		$CompanyCode	= trim($row->company_code);
		$AccountNumber	= trim($row->account_number);
		$GRA			= trim($row->gra);
		$RegionNumber	= trim($row->region_number);
		$TerritoryNumber= trim($row->territory_number);
		$SalesChannel	= trim($row->sales_channel);
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
				$RegionNumber 		= "74";
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
		$entry[] = $CompanyCode;	// 0
		$entry[] = $AccountNumber;	// 1
		$entry[] = $GRA;			// 2
		$entry[] = $RegionNumber;	// 3
		$entry[] = $SalesChannel;	// 4
		$entry[] = $orgID;			// 5
		$entry[] = $TerritoryNumber;// 6
		$a_data[$s_key] = $entry;
		}
	odbc_free_result($o_data);
	return $a_data;
	}
function f_getSD_data($companyCode,$i_record_type,$o_conn,$validCodes,$compareType,$i_acct_split)
	{
	if ($companyCode == "UAS")
		{
		$s_mytable = "CO\$ps_sales_details";
		}else{
		$s_mytable = "ps_sales_details";
		}
	$s_where = f_getRegion($companyCode,"sd");
	$s_split = f_comp_split($companyCode,$i_acct_split,"sd");
	$s_sql = "select record_type, org_id, company_code, account_number, gra, region_number, territory_number, territory_desc as 'sales_channel', sum(convert(lytd,decimal(14,2))) as 'lytd', sum(convert(ytd,decimal(14,2))) as 'ytd', max(update_date) as 'update_date' from $s_mytable d where company_code = '$companyCode' and d.record_type = $i_record_type $s_where $s_split group by record_type, org_id, company_code, account_number, gra, region_number, territory_number, territory_desc order by company_code, account_number, gra, region_number, territory_desc";
	lib_logit($s_sql);
	$a_data = array();
	$o_data=odbc_exec($o_conn,$s_sql);
	if (!$o_data) 
		{
		lib_logit("ORG SQL Error.");
		return $a_data;
		}
	$i_rows = odbc_num_rows($o_data);
	lib_logit("RT: $i_record_type SD Rows to Export: $i_rows.");
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
				$RegionNumber 		= "74";
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
		$entry[] = $CompanyCode;	// 0
		$entry[] = $AccountNumber;	// 1
		$entry[] = $GRA;			// 2
		$entry[] = $RegionNumber;	// 3
		$entry[] = $SalesChannel;	// 4
		$entry[] = $UpdateDate;		// 5
		$entry[] = $orgID;			// 6
		$entry[] = $record_type;	// 7
		$entry[] = $lytd;			// 8
		$entry[] = $ytd;			// 9
		$entry[] = $TerritoryNumber;//10
		$a_data[$s_key] = $entry;
		}
	odbc_free_result($o_data);
	return $a_data;
	}
function f_build_data($a_model,$i_org_id)
	{
	global $i_newID;
	$i_newID++;
	$RecordType			= f_clean_me($a_model[0]);
	$CompanyCode		= f_clean_me($a_model[1]);
	$AccountNumber		= f_clean_me($a_model[2]);
	$GRA				= f_clean_me($a_model[3]);
	$RegionNumber		= f_clean_me($a_model[4]);
	$SalesChannel		= f_clean_me($a_model[5]);
	$ChainHead			= f_clean_me($a_model[6]);
	$RegionName			= f_clean_me($a_model[7]);
	$TerritoryName		= f_clean_me($a_model[8]);
	$TerritoryNumber	= f_clean_me($a_model[9]);
	$SuperCategory		= f_clean_me($a_model[10]);
	$ProductGroup		= f_clean_me($a_model[11]);
	$SubGroup			= f_clean_me($a_model[12]);
	$Model				= f_clean_me($a_model[13]);
	$SalesYTD			= f_clean_me($a_model[14]);
	$SalesMTD			= f_clean_me($a_model[15]);
	$SalesYTD_PrevYear	= f_clean_me($a_model[16]);
	$SalesMTD_PrevYear	= f_clean_me($a_model[17]);
	$SalesPrevYear		= f_clean_me($a_model[18]);
	$SalesMTD_PrevMonth	= f_clean_me($a_model[19]);
	$OpenOrderTotal		= f_clean_me($a_model[20]);
	$BackOrder			= f_clean_me($a_model[21]);
	$OpenOrderFuture	= f_clean_me($a_model[22]);
	$BrandCode			= f_clean_me($a_model[23]);
	$TerritoryName = "---";
	if ($CompanyCode <= "")			{$CompanyCode = "---";}
	if ($AccountNumber <= "")		{$AccountNumber = "---";}
	if ($GRA <= "")					{$GRA = "---";}
	if ($RegionNumber <= "")		{$RegionNumber = "---";}
	if ($SalesChannel <= "")		{$SalesChannel = "---";}
	if ($ChainHead <= "")			{$ChainHead = "---";}
	if ($RegionName <= "")			{$RegionName = "---";}
	if ($TerritoryName <= "")		{$TerritoryName = "---";}
	if ($TerritoryNumber <= "")		{$TerritoryNumber = "---";}
	if ($SuperCategory <= "")		{$SuperCategory = "---";}
	if ($ProductGroup <= "")		{$ProductGroup = "---";}
	if ($SubGroup <= "")			{$SubGroup = "---";}
	if ($Model <= "")				{$Model = "---";}
	if ($BrandCode <= "")			{$BrandCode = "---";}
	if ($SalesYTD <= "")			{$SalesYTD = 0;}
	if ($SalesMTD <= "")			{$SalesMTD = 0;}
	if ($SalesYTD_PrevYear <= "")	{$SalesYTD_PrevYear = 0;}
	if ($SalesMTD_PrevYear <= "")	{$SalesMTD_PrevYear = 0;}
	if ($SalesPrevYear <= "")		{$SalesPrevYear = 0;}
	if ($SalesMTD_PrevMonth <= "")	{$SalesMTD_PrevMonth = 0;}
	if ($OpenOrderTotal <= "")		{$OpenOrderTotal = 0;}
	if ($BackOrder <= "")			{$BackOrder = 0;}
	if ($OpenOrderFuture <= "")		{$OpenOrderFuture = 0;}
	
	if ($CompanyCode == "N/A")		{$CompanyCode = "---";}
	if ($AccountNumber == "N/A")	{$AccountNumber = "---";}
	if ($GRA == "N/A")				{$GRA = "---";}
	if ($RegionNumber == "N/A")		{$RegionNumber = "---";}
	if ($SalesChannel == "N/A")		{$SalesChannel = "---";}
	if ($ChainHead == "N/A")		{$ChainHead = "---";}
	if ($RegionName == "N/A")		{$RegionName = "---";}
	if ($TerritoryName == "N/A")	{$TerritoryName = "---";}
	if ($TerritoryNumber == "N/A")	{$TerritoryNumber = "---";}
	if ($SuperCategory == "N/A")	{$SuperCategory = "---";}
	if ($ProductGroup == "N/A")		{$ProductGroup = "---";}
	if ($SubGroup == "N/A")			{$SubGroup = "---";}
	if ($Model == "N/A")			{$Model = "---";}
	if ($BrandCode == "N/A")		{$BrandCode = "---";}

	$s_sendme = "";
	$s_sendme .= $i_newID						."|||";
	$s_sendme .= $i_org_id						."|||";
	$s_sendme .= $RecordType					."|||";
	$s_sendme .= $CompanyCode					."|||";
	$s_sendme .= $AccountNumber					."|||";
	$s_sendme .= $GRA							."|||";
	$s_sendme .= $RegionNumber					."|||";
	$s_sendme .= $SalesChannel					."|||";
	$s_sendme .= $ChainHead						."|||";
	$s_sendme .= $RegionName					."|||";
	$s_sendme .= $TerritoryName					."|||";
	$s_sendme .= $TerritoryNumber				."|||";
	$s_sendme .= $SuperCategory					."|||";
	$s_sendme .= $ProductGroup					."|||";
	$s_sendme .= $SubGroup						."|||";
	$s_sendme .= $Model							."|||";
	$s_sendme .= $SalesYTD						."|||";
	$s_sendme .= $SalesMTD						."|||";
	$s_sendme .= $SalesYTD_PrevYear				."|||";
	$s_sendme .= $SalesMTD_PrevYear				."|||";
	$s_sendme .= $SalesPrevYear					."|||";
	$s_sendme .= $SalesMTD_PrevMonth			."|||";
	$s_sendme .= $OpenOrderTotal				."|||";
	$s_sendme .= $BackOrder						."|||";
	$s_sendme .= $OpenOrderFuture				."|||";
	$s_sendme .= $BrandCode						."|||";
	$s_sendme .= "@@@";
	return $s_sendme;
	}
function f_send_request($data,$RecordCount,$crm_odbc_server,$crm_host_name,$i_org_id,$key,$i_req_timeout)
	{
	$i_return = 0;
	if ($data == "") {return $i_return;}
	$API_URL = "$crm_host_name/php/mail/cci/sales_details_sql_api.php";
	$options = array('proxyhost' => 'usproxy.hvwan.net:8080','timeout' => $i_req_timeout);
	$headers = array('Content-Type:application/x-www-form-urlencoded;charset=UTF-8',);
	$r = new HttpRequest($API_URL, HttpRequest::METH_POST);
	$r->addPostFields(array('act' => 'ins_org', 'bd' => $data));
	$r->setOptions($options);
	$r->setHeaders($headers);
	try
		{
		$del_response = $r->send()->getBody();
		if (strpos($del_response,"error"))
			{
			lib_logit(str_repeat("-",80),"failed");
			lib_logit("$RecordCount,$crm_odbc_server,$crm_host_name,$i_org_id,$key","failed");
			lib_logit($data,"failed");
			lib_logit("$i_org_id $key","orgs");
			}else{
			$i_return = intval($del_response);
			if (intval($RecordCount) != $i_return)
				{
				lib_logit(str_repeat("-",80),"failed");
				lib_logit("$RecordCount,$crm_odbc_server,$crm_host_name,$i_org_id,$key","failed");
				lib_logit($data,"failed");
				lib_logit("$i_org_id $key","orgs");
				}
			}
		} 
	catch (HttpException $ex)
		{
		lib_logit("$RecordCount,$crm_odbc_server,$crm_host_name,$i_org_id,$key","error");
		lib_logit("$i_org_id $key","orgs");
		if (isset($ex->innerException))
			{
			lib_logit("innerException: ".$ex->innerException->getMessage(),"error");
			}
		if (isset($ex->getMessage))
			{
			lib_logit("getMessage: ".$ex->getMessage(),"error");
			}
		if (isset($r->getResponseCode))
			{
			lib_logit("getResponseCode: ".$r->getResponseCode(),"error");
			}
		lib_logit("HttpException: $ex","error");
		lib_logit("$data","error");
		}
	return $i_return;
	}
/* -=- Start Processing -=- */
$o_conn4 = f_open_db4();
if (!$o_conn4) {lib_logit("Unable to open control data.");exit(1);}
$s_timeGroup = f_gettimeGroup($i_timeGroup);
$s_cc_filter = "";
if ($s_CC > "") {$s_cc_filter = "and companyCode = '$s_CC'";}
$s_sql = "select timeGroup, companyCode, compareType, validCodes, org_table, sales_details, csv_export_3, csv_export_4, crm_odbc_server, crm_host_name from dbo.global_update_control where active = 1 $s_timeGroup $s_cc_filter order by timeGroup, updateOrder, companyCode";
lib_logit($s_sql);
$o_control_data = mssql_query($s_sql,$o_conn4);
if (!$o_control_data) {lib_logit("Unable to query control data.");exit(1);}
$i_company_count = mssql_num_rows($o_control_data);
if ($i_company_count == 0) {echo "No Companies in list.";exit();}
$a_control_data = f_control_array($o_control_data);
lib_logit("Companies to process: $i_company_count");
lib_logit(str_repeat("-",80));
if (date("w") != 6 && date("w") != 0)
	{
	f_doneYet(1,$i_sleepTime);
	f_doneYet(4,$i_sleepTime);
	f_doneYet(3,$i_sleepTime);
	}
mssql_data_seek($o_control_data,0);
$i_curr_company = 1;
$crm_odbc_server_hold = "empty";
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
	$o_CRM 			= odbc_connect($crm_odbc_server,"","");
	if (!$o_CRM) {lib_logit("Unable to open CRM data.");continue;}
	// Delete those with no name --------------------------------------------------------------------------------------
	lib_logit("Processing $companyCode, company $i_curr_company of $i_company_count companies.");
	$s_split = f_comp_split($companyCode,$i_acct_split,"sd");
	$s_split = urlencode($s_split);
	$s_url = "$crm_host_name/php/mail/cci/sales_details_sql_api.php?act=del_zero&cc=$companyCode&flt=$s_split";
	lib_logit($s_url);
	$s_open = file($s_url);
	if ($s_open) {foreach($s_open as $s_line) {lib_logit("del_zero: ".$s_line);}}
	sleep(30);
	// Delete those with no name --------------------------------------------------------------------------------------
	if ($crm_odbc_server_hold != $crm_odbc_server)
		{
		$i_newID_test = -1;
		$i_newID = 0;
		do	{
			$i_newID_test = f_get_maxID($o_CRM,$companyCode);
			sleep(120);
			$i_newID = f_get_maxID($o_CRM,$companyCode);
			lib_logit("ID Check: $i_newID_test / $i_newID");
			echo("ID Check Offset..: ".($i_newID-$i_newID_test)."\n");
			} while ($i_newID_test != $i_newID);
		}else{
		lib_logit("Starting ID: $i_newID");
		}
	$a_SD_RT1		= f_getSD_data($companyCode,1,$o_CRM,$validCodes,$compareType,$i_acct_split);
	$a_orgs			= f_getOG_data($companyCode,$o_CRM,$validCodes,$compareType,$i_acct_split);
	$a_DW_RT1		= f_getDW_data($companyCode,1,$validCodes,$compareType,$i_acct_split);
	$a_DW_RT3		= f_getDW_data($companyCode,3,$validCodes,$compareType,$i_acct_split);
	$a_DW_RT4		= f_getDW_data($companyCode,4,$validCodes,$compareType,$i_acct_split);
	$i_row_sd		= count($a_SD_RT1);
	$i_row_dw		= count($a_DW_RT1);
	$i_row_org		= count($a_orgs);
	$a_keys			= array_keys($a_DW_RT1);
	$i_row_key		= count($a_keys);
	$i_row			= 1;
	$i_sent 		= 0;
	$i_skip 		= 0;
	$i_not_real		= 0;
	$i_failed 		= 0;
	// -----------------------------------------------------------------------------------------------------------------
	// lib_logit("a_DW_RT1 ".count($a_keys),"array_keys");
	// lib_logit(var_export($a_keys,true),"array_keys");
	// lib_logit("a_SD_RT1 ".count(array_keys($a_SD_RT1)),"array_keys");
	// lib_logit(var_export(array_keys($a_SD_RT1),true),"array_keys");
	// lib_logit("a_orgs ".count(array_keys($a_orgs)),"array_keys");
	// lib_logit(var_export(array_keys($a_orgs),true),"array_keys");
	// lib_logit("a_DW_RT3 ".count(array_keys($a_DW_RT3)),"array_keys");
	// lib_logit(var_export($a_DW_RT3,true),"array_keys");
	// lib_logit("a_DW_RT4 ".count(array_keys($a_DW_RT4)),"array_keys");
	// lib_logit(var_export($a_DW_RT4,true),"array_keys");
	// -----------------------------------------------------------------------------------------------------------------
	lib_logit("Rows from SD: $i_row_sd. | Rows from DW: $i_row_dw. | Rows from Keys: $i_row_key. | Org rows: $i_row_org.");
	echo date("H:i:s")." $companyCode Rows: $i_row_dw\n";
	foreach($a_keys as $key)
		{
		$i_percent = 0;
		if ($i_row > 0 && $i_sent > 0) {$i_percent = number_format(($i_sent/$i_row)*100,1);}
		if (($i_row % $i_row_mod) == 0) {echo date("H:i:s")." $companyCode Row.: $i_row $i_percent% \n";}
		$i_linesToSend	= 0;
		$i_org_id	= -1;	
		$i_lytd_c	= 9999999999;
		$i_ytd_c	= 9999999999;
		if ($i_row_sd > 0 && array_key_exists($key,$a_SD_RT1))
			{
			$i_org_id	= intval($a_SD_RT1[$key][6]);	
			$i_lytd_c	= $a_SD_RT1[$key][8];
			$i_ytd_c	= $a_SD_RT1[$key][9];
			}
		if ($i_org_id == -1 && $i_row_org > 0 && array_key_exists($key,$a_orgs))
			{
			$i_org_id	= intval($a_orgs[$key][5]);	
			$i_lytd_c	= 9999999999;
			$i_ytd_c	= 9999999999;
			}
		$a_line			= $a_DW_RT1[$key];
		$i_record_type	= 1;
		$i_org_rows		= count($a_line);
		$i_linesToSend	= $i_linesToSend + $i_org_rows;
		$s_sendme		= "";
		$i_ytd_3		= 0;
		$i_lytd_3		= 0;
		$i_row_3		= 0;
		$i_row_4		= 0;
		$i_ytd_1_real	= 0;
		$i_ytd_3_real	= 0;
		$i_ytd_1_real_l	= 0;
		$i_ytd_3_real_l	= 0;
		$i_row_3_real	= 0;
		$i_row_4_real	= 0;
		foreach($a_line as $model)
			{
			$i_ytd_1	= $model[14];
			$i_lytd_1	= $model[16];
			if (array_key_exists($key,$a_DW_RT3))
				{
				$a_temp3	= $a_DW_RT3[$key];
				$i_ytd_3	= 0;
				$i_lytd_3	= 0;
				$i_row_3	= 0;
				foreach($a_temp3 as $myrow)
					{
					$i_ytd_3	= $model[14];
					$i_lytd_3	= $model[16];
					$i_row_3++;
					}
				}
			if (array_key_exists($key,$a_DW_RT4))
				{
				$a_temp4	= $a_DW_RT4[$key];
				$i_row_4	= 0;
				foreach($a_temp4 as $myrow) {$i_row_4++;}
				}
			if (	(floatval($i_lytd_c) == floatval($i_lytd_1)) && 
					(floatval($i_lytd_c) == floatval($i_lytd_3)) && 
					(floatval($i_ytd_c) == floatval($i_ytd_1)) && 
					(floatval($i_ytd_c) == floatval($i_ytd_3)) && 
					(floatval($i_row_3) == floatval($i_row_4)) && 
					($b_force === false) &&
					$i_org_id > 0
					)
				{
				$b_skipit = 1;
				$i_skip++;
				}else{
				lib_logit("$i_row of $i_row_dw |Sent: $i_sent| |$i_percent%| |$key| |$i_org_id| |$i_lytd_c| == |$i_lytd_1| |$i_lytd_c| == |$i_lytd_3| |$i_ytd_c| == |$i_ytd_1| |$i_ytd_c| == |$i_ytd_3| |$i_row_3| == |$i_row_4| |$b_force|");
				$b_skipit = 0;
				$s_sendme .= f_build_data($model,$i_org_id);
				$i_ytd_1_real = $i_ytd_1_real + $model[14];
				$i_ytd_1_real_l = $i_ytd_1_real_l + $model[16];
				}
			}
		// End RT1. -----------------------------------------------------------------------------------------------
		
		// Start RT3. ---------------------------------------------------------------------------------------------
		if (array_key_exists($key,$a_DW_RT3) && $b_skipit == 0)
			{
			$a_line = $a_DW_RT3[$key];
			$i_record_type = 3;
			$i_org_rows = count($a_line);
			$i_linesToSend = $i_linesToSend + $i_org_rows;
			foreach($a_line as $model)
				{
				$s_sendme .= f_build_data($model,$i_org_id);
				$i_ytd_3_real = $i_ytd_3_real + $model[14];
				$i_ytd_3_real_l = $i_ytd_3_real_l + $model[16];
				$i_row_3_real++;
				}
			}
		// End RT3. -----------------------------------------------------------------------------------------------
		
		// Start RT4. ---------------------------------------------------------------------------------------------
		if (array_key_exists($key,$a_DW_RT4) && $b_skipit == 0)
			{
			$a_line = $a_DW_RT4[$key];
			$i_record_type = 4;
			$i_org_rows = count($a_line);
			$i_linesToSend = $i_linesToSend + $i_org_rows;
			foreach($a_line as $model)
				{
				$s_sendme .= f_build_data($model,$i_org_id);
				$i_row_4_real++;
				}
			}
		// End RT4. -----------------------------------------------------------------------------------------------

		// Begin Check. -------------------------------------------------------------------------------------------
		if	(
			strlen($s_sendme) > 0 && 
			$b_skipit == 0 && 
			round($i_ytd_1_real_l,2) == round($i_ytd_3_real_l,2) && 
			round($i_ytd_1_real,2) == round($i_ytd_3_real,2) && 
			round($i_row_3_real,2) == round($i_row_4_real,2)
			)
			{
			// Begin Delete prior account records. ----------------------------------------------------------------
			if ($i_org_id > 0)
				{
				$API_URL = "$crm_host_name/php/mail/cci/sales_details_sql_api.php";
				$options = array('proxyhost' => 'usproxy.hvwan.net:8080');
				$headers = array('Content-Type:application/x-www-form-urlencoded;charset=UTF-8',);
				$r = new HttpRequest($API_URL, HttpRequest::METH_POST);
				$r->addPostFields(array('act' => "del_org", 'cc' => $companyCode, "org" => $i_org_id));
				$r->setOptions($options);
				$r->setHeaders($headers);
				try {$del_response = $r->send()->getBody();} 
				catch (HttpException $ex) {echo($ex);}
				}
			// End Delete prior account records. ------------------------------------------------------------------

			// Begin Send new data. -------------------------------------------------------------------------------
			$send_response = f_send_request($s_sendme,$i_linesToSend,$crm_odbc_server,$crm_host_name,intval($i_org_id),$key,$i_req_timeout);
			
			// Report Problems ------------------------------------------------------------------------------------
			if (intval($i_linesToSend) != intval($send_response))
				{
				echo "Awooo-Kah! |$key|$i_org_id| $i_linesToSend / $send_response \n";
				$i_failed++;
				if (isset($del_response)) {lib_logit("$i_row of $i_row_dw |$key|$i_org_id| Rows Deleted: $del_response. | Rows Sent: $i_linesToSend. | Rows Back: $send_response.");}
				}
			$i_sent++;
			// End Send new data. ---------------------------------------------------------------------------------
			
			}else{
			if ($b_skipit == 0)
				{
				$i_not_real++;
				lib_logit("$i_row of $i_row_dw |Sent: $i_sent| |$key|$i_org_id| NoReal: ($i_ytd_1_real_l == $i_ytd_3_real_l) && ($i_ytd_1_real == $i_ytd_3_real) && ($i_row_3_real == $i_row_4_real) |Size: ".strlen($s_sendme)."|$b_skipit == 0|" );
				}
			}
			// End Check. ---------------------------------------------------------------------------------------------
			// End Send. ----------------------------------------------------------------------------------------------
		$i_row++;
		}
	echo date("H:i:s")." Company: $i_curr_company/$i_company_count $companyCode $i_row_dw. Sent: $i_sent. Percent: $i_percent. Skip: $i_skip. Not Real: $i_not_real. Failed: $i_failed.\n";
	lib_logit("Results..: ".str_repeat("-",100));
	lib_logit("Total....: $i_row_dw.");
	lib_logit("Sent.....: $i_sent.");
	lib_logit("Percent..: $i_percent%.");
	lib_logit("Skipped..: $i_skip.");
	lib_logit("Not Real.: $i_not_real.");
	lib_logit("Failed...: $i_failed.");
	$s_percent = $i_percent;
	if ($i_percent < 10) {$s_percent = " $i_percent";}
	lib_logit_daily("$companyCode $i_acct_split \tNot Real: $i_not_real. \tFailed: $i_failed. \tSent: $s_percent% \tRead: $i_row_dw.","results");
	unset($a_orgs);
	unset($a_SD_RT1);
	unset($a_DW_RT1);
	unset($a_DW_RT3);
	unset($a_DW_RT4);
	unset($a_keys);
	$crm_odbc_server_hold = $crm_odbc_server;
	$i_curr_company++;
	}
lib_logit("End ".str_repeat("-",100));
echo date("H:i:s")." End.\n";
exit(0);
?>
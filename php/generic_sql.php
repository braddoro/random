<?php
class c_generic_sql
	{
	private $g_showSQL = false;
	private $g_showRowNumber = false;
	private $g_showFieldlist = false;
	private $g_bottomHeader = false;
	private $g_showFieldSize = false;
	private $g_showLabel = false;
	public function gen_query($s_title,$i_type,$server,$username,$password,$dbname,$s_sql,$i_random=0,$b_useKeywordChecking=true,$s_font_size=".66em",$b_showRowNumber=false,$b_showSQL=false,$b_showFieldlist=false,$b_bottomHeader=false,$b_bare=false,$b_showFieldSize=false,$b_showLabel=false)
		{
		$this->g_showSQL = $b_showSQL;
		$this->g_showRowNumber = $b_showRowNumber;
		$this->g_showFieldlist = $b_showFieldlist;
		$this->g_bottomHeader = $b_bottomHeader;
		$this->g_showFieldSize = $b_showFieldSize;
		$this->g_showLabel = $b_showLabel;
		$s_error = "";
		if ($b_useKeywordChecking)
			{
			$s_sql = $this->sql_safe_words($s_sql);
			$s_error = "<br/><strong>Unacceptable SQL Keyword</strong><br/>";
			}
		$q_data = "";
		$i_rows = 0;
		$i_cols = 0;
		$s_return = $this->add_scripts($s_font_size);
		if ($i_type == 1)
			{
			$o_conn=$this->connect_odbc($server,$username,$password,$dbname);
			if ($o_conn)
				{
				$q_data=$this->query_odbc($o_conn,$s_sql);
				if (!$b_bare)
					{
					if ($q_data)
						{
						$s_return .= $this->show_odbc($s_title,$i_random,$q_data,$s_sql,$i_random);
						odbc_close($o_conn);
						} else {
						$s_return = $s_error.$s_sql;
						}
					} else {
					$s_return = $q_data;
					}
				}
			}
		if ($i_type == 2)
			{
			$o_conn=$this->connect_mssql($server,$username,$password,$dbname);
			if ($o_conn)
				{
				$q_data=$this->query_mssql($o_conn,$s_sql);
				if ($q_data)
					{
					$s_return .= $this->show_mssql($s_title,$i_random,$q_data,$s_sql,$i_random);
					mssql_close($o_conn);
					} else {
					$s_return = $s_error.$s_sql;
					}
				}
			}
		return $s_return;
		}
	private function add_scripts($s_font_size)
		{
		// color_td_even {background-color:#E3E3E3;} .color_td_odd {background-color:#F5F5F5;}
		$s_return = '
<style type="text/css">
.font_family_sans {font-family:arial;}
.font_family_mono {font-family:courier;font-size:.9em;}
.font_size_bigger {font-size:1.5em;font-weight:bold;font-family:arial;}
.font_size_big {font-size:1em;}
.font_size_cell {font-size:'.$s_font_size.';}
.font_strong {font-weight:bold;}
.font_align_left {text-align:left;}
.font_align_right {text-align:right;}
.color_gray2 {background-color:#5E5E5E;}
.color_gray1 {background-color:#A3A3A3;}
.color_th {background-color:#DCDCDC;}
.color_td_even {background-color:#DED9BA;}
.color_td_odd {background-color:#FFFFA3;}
.bord_top_th {border-top:1px solid #5E5E5E;}
.bord_bot_td {border-bottom:1px solid #5E5E5E;}
.bord_left_td {border-left:1px solid #5E5E5E;border-bottom:1px solid #5E5E5E;}
.bord_right_td {border-right:1px solid #5E5E5E;border-bottom:1px solid #5E5E5E;}
.bord_all {border:1px solid #999999;padding:3px;}
</style>
<script language="javascript" type="text/javascript">
function collapseMe(s_container) {
if (document.getElementById(s_container)) {
	if (document.getElementById(s_container).style.display == "block") {
		document.getElementById(s_container).style.display = "none";
	} else {
		document.getElementById(s_container).style.display = "block";
	}
}
}
</script>
';
	return $s_return;
	}
	private function sql_safe_words($s_sql)
		{
		$s_safe = "_removed_";
		$s_sql_out = $s_sql;
		$s_sql_out = str_replace("insert into ",$s_safe,	strtolower($s_sql_out));
		$s_sql_out = str_replace("update ",		$s_safe,	strtolower($s_sql_out));
		$s_sql_out = str_replace("delete from ",$s_safe,	strtolower($s_sql_out));
		$s_sql_out = str_replace("alter ",		$s_safe,	strtolower($s_sql_out));
		$s_sql_out = str_replace("truncate ",	$s_safe,	strtolower($s_sql_out));
		$s_sql_out = str_replace("drop ",		$s_safe,	strtolower($s_sql_out));
		$s_sql_out = str_replace("kill ",		$s_safe,	strtolower($s_sql_out));
		$s_sql_out = str_replace(" into ",		$s_safe,	strtolower($s_sql_out));
		$s_sql_out = str_replace("dbcc ",		$s_safe,	strtolower($s_sql_out));
		$s_sql_out = str_replace("cursor ",		$s_safe,	strtolower($s_sql_out));
		$s_sql_out = str_replace("create ",		$s_safe,	strtolower($s_sql_out));
		$s_sql_out = str_replace("shutdown ",	$s_safe,	strtolower($s_sql_out));
		$s_sql_out = str_replace("exec ",		$s_safe,	strtolower($s_sql_out));
		$s_sql_out = str_replace("restore ",	$s_safe,	strtolower($s_sql_out));
		return $s_sql_out;
		}
	private function connect_odbc($server,$username,$password,$dbname)
		{
		$o_conn=odbc_connect($server,$username,$password);
		return $o_conn;
		}
	private function query_odbc($o_conn,$s_sql)
		{
		$q_data=odbc_exec($o_conn,$s_sql);
		return $q_data;
		}
	private function show_odbc($s_title,$i_random,$q_data,$s_sql,$i_random)
		{
		$i_rows = odbc_num_rows($q_data);
		$i_cols = odbc_num_fields($q_data);
		$s_fieldList = "";
		$s_output = "";
		if ($s_title > "")
			{
			$s_output .= '<div class="font_family_sans font_size_big font_strong">'.$s_title.'</div>'."\n";
			}
		if ($this->g_showSQL)
			{
			$s_output .= "<div class='font_size_cell font_family_sans' onclick='collapseMe(\"div_$i_random\");' style='cursor:pointer;' title='Click to show/hide SQL.'>SQL Results: $i_rows rows in $i_cols columns at ".date("F j, Y, g:i a")."</div>"."\n";
			$s_output .= '<div class="color_th font_family_mono bord_all" id="div_'.$i_random.'" style="display:none;">'."\n";
			$s_output .= str_replace("\t","&nbsp;&nbsp;&nbsp;&nbsp;",str_replace("\n","<br />",$s_sql));
			$s_output .= '</div>'."\n";
			$s_output .= '<br/>'."\n";
			}
		if ($this->g_showLabel)
			{
			$s_output .= "<div class='font_size_cell font_family_sans' ><strong>Results: $i_rows</strong></div>"."\n";
			$s_output .= '<br/>'."\n";
			}
		$s_output .= '<table class="font_size_cell font_family_sans color_gray2" border="0" cellpadding="1" cellspacing="0">'."\n";
		$s_border = "bord_left_td";
		$s_header = "<tr>\n";
		$s_header2 = "";
		if ($this->g_showRowNumber)
			{
			$s_header = '<th class="'.$s_border.' bord_top_th color_th">row</th>'."\n";
			$s_header2 = '<tr><th class="'.$s_border.' bord_bot_td color_th">row</th>'."\n";
			}
		for($i=1;$i<=$i_cols;$i++)
			{
			$s_columnname = odbc_field_name($q_data,$i);
			//$pattern = '/(\w+) (\d+), (\d+)/i';
			//$replacement = '${1}1,$3';
			$pattern = '[_]';
			$replacement = ' ';
			$s_columnname = preg_replace($pattern,$replacement,$s_columnname);

			if ($i == $i_cols) {$s_border = "bord_left_td bord_right_td";}
			$s_header .= '<th class="'.$s_border.' bord_top_th color_th">'.$s_columnname.'</th>'."\n";
			$s_header2 .= '<th class="'.$s_border.' bord_bot_td color_th">'.$s_columnname.'</th>'."\n";
			$s_fieldList .= odbc_field_name($q_data,$i).", <br/>";
			}
		$s_header .= "</tr>\n";
		$s_output .= $s_header;
		$i_row = 1;
		while (odbc_fetch_row($q_data))
			{
			$s_border = "bord_left_td";
			if ($i_row % 2)
				{
				$s_bgcolor = "color_td_even";
				} else {
				$s_bgcolor = "color_td_odd";
				}
			$s_output .= '<tr>'."\n";
			if ($this->g_showRowNumber)
				{
				$s_output .= '<th class="'.$s_border.' '.' color_th" align="right">'.$i_row.'</th>'."\n";
				}
			for($i=1;$i<=$i_cols;$i++)
				{
				if ($i == $i_cols)
					{
					$s_border = "bord_left_td bord_right_td";
					}
				$s_ftype = odbc_field_type($q_data,$i);
				$s_length = odbc_field_len($q_data,$i);
				if ($s_ftype == "integer" || $s_ftype == "bigint" || $s_ftype == "bit" || $s_ftype == "smallint")
					{
					$s_align = "font_align_right";
					$s_ftitle = "$s_ftype";
					} else {
					$s_align = "font_align_left";
					$s_ftitle = "$s_ftype($s_length)";
					}
				$temp = odbc_result($q_data,$i);
				if ($temp == NULL)
					{
					$temp = "&nbsp;";
					}
				$s_output .= '<td class="'.$s_border.' '.$s_bgcolor.' '.$s_align.'" title="'.$s_ftitle.'">'.$temp.'</td>'."\n";
				}
			$s_output .= '</tr>'."\n";
			$i_row++;
			}
		if ($this->g_bottomHeader)
			{
			$s_output .= $s_header2;
			}
		$s_output .= '</table>'."\n";
		if ($this->g_showFieldlist)
			{
			$s_output .= '<br />'."\n";
			$s_output .= "<div class='font_size_cell font_family_sans font_strong' onclick='collapseMe(\"div2_$i_random\");' style='cursor:pointer;' title='Click to show/hide field list.'>FieldList</div>"."\n";
			$s_output .= '<div class="color_th font_family_mono bord_all" id="div2_'.$i_random.'" style="display:none;">'."\n";
			$s_output .= $s_fieldList;
			$s_output .= '</div>'."\n";
			$s_output .= '</div>';
			}
		odbc_free_result($q_data);
		return $s_output;
		}
	private function connect_mssql($server,$username,$password,$dbname)
		{
		$o_conn=mssql_connect($server,$username,$password);
		if (!$o_conn)
			{
			$sqldb=mssql_select_db($dbname,$o_conn);
			}
		return $o_conn;
		}
	private function query_mssql($o_conn,$s_sql)
		{
		$q_data=mssql_query($s_sql,$o_conn);
		return $q_data;
		}
	private function show_mssql($s_title,$i_random,$q_data,$s_sql,$i_random)
		{
		$i_rows = mssql_num_rows($q_data);
		$i_cols = mssql_num_fields($q_data);
		$s_output = "";
		$s_fieldList = "";
		if ($s_title > "")
			{
			$s_output .= '<div class="font_family_sans font_size_big font_strong">'.$s_title.'</div>'."\n";
			}
		if ($this->g_showSQL)
			{
			$s_output .= "<div class='font_size_cell font_family_sans' onclick='collapseMe(\"div_$i_random\");' style='cursor:pointer;' title='Click to show/hide SQL.'>SQL Results: $i_rows rows in $i_cols columns at ".date("F j, Y, g:i a")."</div>"."\n";
			$s_output .= '<div class="color_th font_family_mono bord_all" id="div_'.$i_random.'" style="display:none;">'."\n";
			$s_output .= str_replace("\t","&nbsp;&nbsp;&nbsp;&nbsp;",str_replace("\n","<br />",$s_sql));
			$s_output .= '</div>'."\n";
			$s_output .= '<br/>'."\n";
			}
		$s_output .= '<table class="font_family_sans font_size_cell color_gray2" border="0" cellpadding="1" cellspacing="0">'."\n";
		$s_border = "bord_left_td";
		$s_header = "";
		$s_header2 = "";
		if ($this->g_showRowNumber)
			{
			$s_header = '<tr><th class="'.$s_border.' bord_top_th color_th">row</th>'."\n";
			$s_header2 = '<tr><th class="'.$s_border.' bord_bot_td color_th">row</th>'."\n";
			}
		for($i=0;$i<$i_cols;$i++)
			{
			if ($i == $i_cols-1)
				{
				$s_border = "bord_left_td bord_right_td";
				}
			$s_header .= '<th class="'.$s_border.' bord_top_th color_th">'.mssql_field_name($q_data,$i).'</th>'."\n";
			$s_header2 .= '<th class="'.$s_border.' bord_bot_td color_th">'.mssql_field_name($q_data,$i).'</th>'."\n";
			$s_fieldList .= mssql_field_name($q_data,$i).", <br/>";
			}
		$s_header .= "</tr>\n";
		if ($i_rows > 0)
			{
			$s_output .= $s_header;
			}
		$i_row = 1;
		while ($row = mssql_fetch_array($q_data))
			{
			$s_border = "bord_left_td";
			if ($i_row % 2)
				{
				$s_bgcolor = "color_td_even";
				} else {
				$s_bgcolor = "color_td_odd";
				}
			$s_output .= '<tr>'."\n";
			if ($this->g_showRowNumber)
				{
				$s_output .= '<th class="'.$s_border.' '.' color_th" align="right">'.$i_row.'</th>'."\n";
				}
			for($i=0;$i<$i_cols;$i++)
				{
				if ($i == $i_cols-1)
					{
					$s_border = "bord_left_td bord_right_td";
					}
				$temp = $row[$i];
				if ($temp === NULL)
					{
					$temp = "&nbsp;";
					}
				$s_ftype = mssql_field_type($q_data,$i);
				$s_length = mssql_field_length($q_data,$i);
				if ($s_ftype == "int" || $s_ftype == "bigint" || $s_ftype == "bit" || $s_ftype == "smallint" || $s_ftype == "datetime")
					{
					$s_align = "font_align_right";
					$s_ftitle = "$s_ftype";
					} else {
					$s_align = "font_align_left";
					$s_ftitle = "$s_ftype($s_length)";
					}
				if ($this->g_showFieldSize)
					{
					$_mytitle = $s_ftitle;
					} else {
					$_mytitle = "";
					}
				$s_output .= '<td class="'.$s_border.' '.$s_bgcolor.' '.$s_align.'" title="'.$_mytitle.'">'.$temp.'</td>'."\n";
				}
			$s_output .= '</tr>'."\n";
			$i_row++;
			}
		if ($this->g_bottomHeader)
			{
			if ($i_rows > 0)
				{
				$s_output .= $s_header2;
				}
			}
		$s_output .= '</table>'."\n";
		if ($this->g_showFieldlist)
			{
			$s_output .= '<br />'."\n";
			$s_output .= "<div class='font_size_cell font_family_sans font_strong' onclick='collapseMe(\"div2_$i_random\");' style='cursor:pointer;' title='Click to show/hide field list.'>FieldList</div>"."\n";
			$s_output .= '<div class="color_th font_family_mono bord_all" id="div2_'.$i_random.'" style="display:none;">'."\n";
			$s_output .= $s_fieldList;
			$s_output .= '</div>'."\n";
			$s_output .= '</div>';
			}
		mssql_free_result($q_data);
		return $s_output;
		}
	}
?>
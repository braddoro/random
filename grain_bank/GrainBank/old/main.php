<?php
require_once('php/data_library.php');
require_once("php/show_panel_lib_c.php");
$data = New DataLibrary();
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html>
<head>
<title>Grain Bank</title>
<link rel="stylesheet" type="text/css" href="css/main.css">	
<script type="text/javascript" src="js/ajax.js"></script>
</head>
<body>
<div id="md_t" class="mdiv-t">top</div>
<div id="md_l" class="mdiv-l"><?php 
		$output = '';
		$response = $data->getData('CALL grainBalance(1)');
		if(!$response['status']) {
			$output = $data->parseErrors($response['message']);
			exit(1);
		}
		$result = $response['result'];
		$crow = 0;
		$output .= "<table>
<thead>
<tr>
<th>Name</th>
<th>Balance</th>
<th>Change</th>
</tr>
</thead>";
		while ($row = $result->fetch() ) {
		  $rstyle = ($crow & 1) ? 'tr-odd' : 'tr-eve';
		  $disp = null;
			$disp[] = $row["grain_type"];
			$disp[] = $row["balance"];
			$disp[] = date("m/d/Y",strtotime($row["lastChange"]));
			$output .= "<tr id='table_balance_$crow' class='$rstyle'>";
			foreach ($disp as $value) {
				$output .= "<td>$value</td>\n";
			}
			//echo "</tr>\n";
			//$tot = $tot + $c2;
			$crow++;
		}
		$result->closeCursor();
		unset($response);
		$objPanel = new panelControl();

		$output .= "</tbody>
<tfoot>
<tr>
<th>Total</th>
<th class='tdr'>" . $tot. "</th>
<th>&nbsp;</th>
</tr>
</tfoot>
</table>
";
		
		echo $objPanel->main(				
				$headDivName = "user",
				$bodyDivName = "user",
				$headContent = "Users",
				$bodyContent = $output,
				$panelWidth  = "1000px");
	?></div>
<div id="md_r" class="mdiv-r"><?php 
		$output = '';
		$sql = "SELECT userID, userName FROM grain_user WHERE active = 'Y' order by userName";
		$response = $data->getData($sql);
		if(!$response['status']) {
			$output = $data->parseErrors($response['message']);
		}
		$result = $response['result'];
		while ($row = $result->fetch()) {
			$output .= '<a href="' . basename(__FILE__) . '?id=' . $row["userID"] . '">' . $row["userName"] .'</a><br />';
		}
		$result->closeCursor();
		unset($response);
		$objPanel = new panelControl();
		echo $objPanel->main(				
				$headDivName = "user",
				$bodyDivName = "user",
				$headContent = "Users",
				$bodyContent = $output,
				$panelWidth  = "225px");
	?></div>
<div id="md_b" class="mdiv-b">bottom</div>
</body>
</html>
<script type="text/javascript">
function getMain(num){
	var retval;
	switch(num){
	case 0:
		retval = 'Grain Bank Tracker';
		break;
	case 1:
		retval = '';
		break;
	case 2:
		retval = '';
		break;
	case 3:
		retval = '';
		break;
	default:
		retval = '';
		break;
	}
	return retval;
}
document.getElementById("md_t").innerHTML = this.getMain(0);
//document.getElementById("md_l").innerHTML = this.getMain(1);
//document.getElementById("md_r").innerHTML = this.getMain(2);
document.getElementById("md_b").innerHTML = this.getMain(3);
</script>	
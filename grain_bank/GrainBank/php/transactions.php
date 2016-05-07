<?php
require_once('data_library.php');
$data = New DataLibrary();

if(isset($_POST)){
	switch($_POST['type']) {
	case 'add':
		//$guid = $data->generateGUID();
		$userID = 	intval($_POST['element_0']);
		$grainID = 	intval($_POST['element_1']);
		$orderID = 	intval($_POST['element_2']);
		$amount = 	intval($_POST['element_3']);
		$sql = "INSERT INTO grain_inventory (grainID_fk, userID_fk, orderID, transactionAmount) VALUES ($grainID, $userID, $orderID, $amount);";
		$result = $data->getData($sql);
		if(!$result['status']) {
			echo('Failed<br />');
			echo($data->parseErrors($result['message']));
		}
		break;
	case 'edit':
		break;
	default:
		break;
	}	
}
?>
<!DOCTYPE html>
<html>
<head>
<link rel="stylesheet" type="text/css" href="../css/grain.css">
<link rel="stylesheet" type="text/css" href="../css/view.css">
<link type="text/javascript" href="../css/view.js"></script>	
<script type="text/javascript" src="../js/calendar.js"></script>
<script type="text/javascript" src="../js/view_library.js"></script>	
<script>view = new ViewLibrary();</script>
<title>Grain Bank</title>
</head>
<body>
<span>Grain Balance</span>
<table>
<thead>
<tr>
<th>Name</th>
<th>Balance</th>
<th>Change</th>
</tr>
</thead>
<?php
$response = $data->getData('CALL grainBalance(1)');
if(!$response['status']) {
	echo('Failed<br />');
	echo($data->parseErrors($response['message']));
}
$result = $response['result'];
echo "<tbody>\n";
$crow = 0;
while ($row = $result->fetch() ) {
  $rstyle = ($crow & 1) ? 'tr-odd' : 'tr-eve';
  $disp = null;
	$disp[] = $row["grain_type"];
	$disp[] = $row["balance"];
	$disp[] = date("m/d/Y",strtotime($row["lastChange"]));
	
	echo "<tr id='table_balance_$crow' class='$rstyle'>";
	foreach ($disp as $value) {
		echo "<td>$value</td>\n";
	}
	echo "</tr>\n";
	$tot = $tot + $c2;
	$crow++;
}
$result->closeCursor();
unset($response);
?>
</tbody>
<tfoot>
<tr>
<th>Total</th>
<th class='tdr'><?php echo $tot; ?></th>
<th>&nbsp;</th>
</tr>
</tfoot>
</table>
<br>
<span style='spanl'>Grain Transactions <img src="../icon/add2.jpg" style="border:none;max-width:16px;max-height:16px;" alt="Click to show hide the input form." onClick="view.showIt('addTrans')"></span>
<div id="addTrans" style="display:none;">
	<div style="width:50%">
		<h1><a>Add Transaction</a></h1>
		<form id="formTrans" class="appnitro" method="post" action="grain.php">
			<ul >
		
		<li id="li_0" >
		<label class="description" for="element_0">User Name </label>
		<div>
			<input id="element_0" name="element_0" class="element text medium" type="text" maxlength="255" value=""/> 
		</div> <p class="guidelines" id="guide_0"><small>Select a user.</small></p> 
		</li>		

		<li id="li_1" >
		<label class="description" for="element_1">Grain Type </label>
		<div>
			<input id="element_1" name="element_1" class="element text medium" type="text" maxlength="255" value=""/> 
		</div> <p class="guidelines" id="guide_1"><small>What kind of grain is this?</small></p> 
		</li>		
		
		<li id="li_2" >
		<label class="description" for="element_2">Order ID </label>
		<div>
			<input id="element_2" name="element_2" class="element text small" type="text" maxlength="255" value=""/> 
		</div> <p class="guidelines" id="guide_2"><small>Which order ID was this transaction with?</small></p> 
		</li>		
		
		<li id="li_3" >
		<label class="description" for="element_3">Grain Amount </label>
		<div>
			<input id="element_3" name="element_3" class="element text small" type="text" maxlength="255" value=""/> 
		</div><p class="guidelines" id="guide_3"><small>Use a negative number to subtract from the bank.</small></p> 
		</li>		

		<li class="buttons">
			<input type="hidden" name="type" value="add" />
	    <input type="hidden" name="form_id" value="1057011" />
			<input id="saveForm" class="button_text" type="submit" name="submit" value="Submit" />
		</li>
			</ul>
		</form>	
	</div>
</div>

<table>
<thead>
<tr>
<th>Name</th>
<th class='tdr'>Order ID</th>
<th class='tdr'>Amount</th>
<th>Date</th>
</tr>
<?php
$response = $data->getData('CALL transactionDetail(1)');
if(!$response['status']) {
	echo('Failed<br />');
	echo($data->parseErrors($response['message']));
}
$result = $response['result'];
echo "<tbody>\n";
$crow = 0;
while ($row = $result->fetch() ) {
  $rstyle = ($crow & 1) ? 'tr-odd' : 'tr-eve';
  $disp = null;
	$disp[] = $row["grain_type"];
	$disp[] = $row["orderID"];
	$disp[] = $row["transactionAmount"];
	$disp[] = date("m/d/Y",strtotime($row["transactionDate"]));
	echo "<tr id='table_transaction_$crow' class='$rstyle'>";
	foreach ($disp as $value) {
		echo "<td>$value</td>\n";
	}
	echo "</tr>\n";
	$crow++;
}
$result->closeCursor();
unset($response);
?>
</tbody>
</table>
</body>
</html>

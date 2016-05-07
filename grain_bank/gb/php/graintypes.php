<?php
require_once('data_library.php');
$data = New DataLibrary();

$form_grainID = 0;
$form_grain_type = '';
$form_call = 'add';
$showInput='none';

if(isset($_POST)){
	$type = (isset($_POST['call'])) ? $_POST['call'] : 'bad';
	
	switch($type) {
	case 'add':
		$guid = $data->generateGUID();
		$GrainName = addslashes($_POST['element_0']);
		$sql = "INSERT INTO grain_type (grain_type, grainUUID) VALUES ('$GrainName', '$guid');";
		$result = $data->getData($sql);
		if(!$result['status']) {
			echo($data->parseErrors($result['message']));
		}
		break;
	case 'edit':
		$GrainID = intval($_POST['ID']);
		$GrainName = addslashes($_POST['element_0']);
		$sql = "UPDATE grain_type SET grain_type = '$GrainName' WHERE grainID = $GrainID;";
		$result = $data->getData($sql);
		if(!$result['status']) {
			echo($data->parseErrors($result['message']));
		}
		break;
	default:
		break;
	}	
}

if(isset($_GET)){
	//var_dump($_GET);
	$type = (isset($_GET['call'])) ? $_GET['call'] : 'bad';
	
	switch($type) {
	case 'add':
		break;
	case 'edit':
		$showInput='block';
		$GrainID = intval($_GET['id']);
		$sql = "SELECT * FROM grain_type WHERE grainID = $GrainID;";
		$response = $data->getData($sql);
		if(!$response['status']) {
			echo($data->parseErrors($response['message']));
		}
		$result = $response['result'];
		while ($row = $result->fetch() ) {
			$form_grainID = $row["grainID"];
			$form_grain_type = $row["grain_type"];
		}
		$form_call = 'edit';
		$result->closeCursor();
		unset($response);
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
<span>Grain Types <img src="../icon/add2.jpg" style="border:none;max-width:16px;max-height:16px;" alt="Click to show hide the input form." onClick="view.showIt('addGrain')"></span>
<div id="addGrain" style="display:<?php echo($showInput); ?>;">
	<div style="width:50%">
	<form id="formTrans" method="post" action="<?php echo(basename(__FILE__)); ?>">
		<table>
			<tr>
				<td>Grain Name</td>
				<td><input id="element_0" name="element_0" width="100" type="text" maxlength="255" value="<?php echo($form_grain_type); ?>"/></td>
			</tr>

			<tr>
				<td>&nbsp;</td>
				<td>
					<input type="hidden" name="ID" value="<?php echo($form_grainID); ?>" />
					<input type="hidden" name="call" value="<?php echo($form_call); ?>" />
					<input type="hidden" name="form_id" value="grainType" />
					<input id="saveForm" class="button_text" type="submit" name="submit" value="Submit" />
				</td>
			</tr>
		</table>
		</form>	
	</div>
	
</div>
<table>
<thead>
<tr>
<th>Name</th>
<th>Date Added</th>
<th>Edit</th>
<th>Delete</th>
</tr>
</thead>
<?php
$response = $data->getData('SELECT grainID, grain_type, grainUUID, addedDate FROM grain_type order by grain_type;');
if(!$response['status']) {
	echo($data->parseErrors($response['message']));
	exit(1);
}
$result = $response['result'];
echo "<tbody>\n";
$crow = 0;
while ($row = $result->fetch() ) {
  $rstyle = ($crow & 1) ? 'tr-odd' : 'tr-eve';
  $disp = null;
	$disp[] = $row["grain_type"];
	$disp[] = date("m/d/Y",strtotime($row["addedDate"]));
	$disp[] = '<a href="' . basename(__FILE__) . '?id=' . $row["grainID"] . '&call=edit">' . '<img src="../icon/edit.jpg" style="border:none;max-width:16px;max-height:16px;" alt="Click to edit the row.">' . ' </a>';
	$disp[] = '<a href="' . basename(__FILE__) . '?id=' . $row["grainID"] . '&call=del">' . '<img src="../icon/delete.jpg" style="border:none;max-width:16px;max-height:16px;" alt="Click to delete the row.">' . ' </a>';
	echo "<tr id='table_grainType_$crow' class='$rstyle'>";	
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
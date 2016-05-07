<!DOCTYPE html>
<html>
<head>
<link rel="stylesheet" type="text/css" href="grain.css">
<title>Grain Bank</title>
</head>
<body>
<span>Grain Remaining</span>
<table>
<thead>
  <tr>
		<th>Name</th>
		<th>Balance</th>
		<th>Change</th>
	</tr>
</thead>
<?php
			//$tranDate = intval($_POST['element_4_3']) . '-' . intval($_POST['element_4_2']) . '-' . intval($_POST['element_4_1']);
			//, '$tranDate'
			//, transactionDate
		
		//$sql = 'SELECT u.userID, u.userUUID, u.userName, u.emailAddress, u.active, u.addedDate FROM grain_user u;';
		//$sql = "INSERT INTO grain_inventory (userUUID, userName, emailAddress, active) VALUES ($guid, $grain, '', '', '');";



// http://www.phpro.org/tutorials/Introduction-to-PHP-PDO.html#4.3

$hostname = 'localhost';
$username = 'brad';
$password = 'alvahugh1';
$dbname = 'grain_bank';

try {
    $conn = new PDO("mysql:host=$hostname;dbname=grain_bank", $username, $password);
    }
catch(PDOException $e)
    {
    echo $e->getMessage();
    }
try {
	$sql = 'CALL grainBalance(1)';
        $result = $conn->query($sql);
        $result->setFetchMode(PDO::FETCH_ASSOC);
} catch (PDOException $pe) {
	die("Error occurred:" . $pe->getMessage());
}
echo "<tbody>\n";
$crow = 0;
while ($row = $result->fetch() ) {
  $rstyle = ($crow & 1) ? 'tr-odd' : 'tr-eve';
	$c1 = $row["grain_type"];
	$c2 = $row["balance"];
	$c3 = date("m/d/Y",strtotime($row["lastChange"]));

	echo "<tr class='$rstyle'><td>$c1</td><td class='tdr'>$c2</td><td>$c3</td></tr>\n";
	$tot = $tot + $c2;
	$crow++;
}
$result->closeCursor();

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
<span>Grain Transactions</span>
<table>
<thead>
  <tr>
		<th>Name</th>
		<th class='tdr'>Order ID</th>
		<th class='tdr'>Amount</th>
		<th>Date</th>
	</tr>
<?php
try {
	$sql = 'CALL transactionDetail(1)';
        $result = $conn->query($sql);
	$result->setFetchMode(PDO::FETCH_ASSOC);
} catch (PDOException $pe) {
	die("Error occurred:" . $pe->getMessage());
}

echo "<tbody>\n";
$crow = 0;
while ($row = $result->fetch() ) {
  $rstyle = ($crow & 1) ? 'tr-odd' : 'tr-eve';
	$c1 = $row["grain_type"];
	$c2 = $row["orderID"];
	$c3 = $row["transactionAmount"];
	$c4 = date("m/d/Y",strtotime($row["transactionDate"]));
	echo "<tr class='$rstyle'><td>$c1</td><td class='tdr'>$c2</td><td class='tdr'>$c3</td><td>$c4</td></tr>\n";
	$crow++;
}
$result->closeCursor();
$conn = null;
?>
</tbody>
</table>
</body>
</html>

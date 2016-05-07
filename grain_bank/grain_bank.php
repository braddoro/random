<!DOCTYPE html>
<html>
<head>
<style></style>
<style type="text/css">
body {
  display: block;
  margin: .8em;
  background-color: #F5F5F5;
}
table {
  display: table;
  border-collapse: separate;
  border-spacing: .0em;
  font-family: arial;
  font-size: .9em;
  background-color: #FFFFF0
}
tbody {
  display: table-row-group;
  vertical-align: middle;
  border-color: inherit;
  background-color: #FFFFF0
}
thead {
  display: table-header-group;
  vertical-align: middle;;
  background-color: #DCDCDC;
  text-align: left;
}
tfoot {
  display: table-footer-group;
  vertical-align: middle;
  border-color: inherit;
  background-color: #DCDCDC;
  text-align: left;
}
th {
  display: table-cell;
  vertical-align: inherit;
  font-weight: bold;
  padding-right: 1em;
}
tr {
  display: table-row;
  vertical-align: inherit;
  border-color: inherit;

}
.tr-odd {
  display: table-row;
  vertical-align: inherit;
  border-color: inherit;
  background-color: #FFFFD1;
}
.tr-eve {
  display: table-row;
  vertical-align: inherit;
  border-color: inherit;
  background-color: #C7DEC7;
}
td {
  display: table-cell;
  vertical-align: inherit;
  padding-right: 1em;

}
tdl {
  display: table-cell;
  vertical-align: inherit;

}
.tdr {
  display: table-cell;
  vertical-align: inherit;
  text-align: right;
  padding-right: 1em;
}
span {
  font-family: arial, Sans-serif;
  font-size: 1.1em;
  font-weight: bold;
  background-color: #DCDCDC;
  color: #000000;
  padding-left: .5em;
  padding-right: .5em;
}
div.form {
  font-family: arial, Sans-serif;
  font-size: .9em;
  font-weight: normal;
  background-color: #DCDCDC;
  color: #000000;
  padding: .5em;
  width: 33%;
}
</style>
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
<hr>
<div class="form">
<form action="grain.php">
User Name<br><input type="text" name="userName" value="1"><br>
Transaction Amount<br><input type="text" name="transactionAmount" value="0"><br>
Grain Type<br><input type="text" name="grainType" value="0"><br>
Order ID<br><input type="text" name="orderID" value="0"><br>
Date<br><input type="text" name="transactionDate" value="1/1/2015"><br><br>
<input type="submit" value="Submit">
</form>
</div>
</body>
</html>

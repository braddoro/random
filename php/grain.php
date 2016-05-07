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
  font-size: .75em;
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
  background-color: #FFFFF0;
  text-align: left;
}
tfoot {
  display: table-footer-group;
  vertical-align: middle;
  border-color: inherit;
  background-color: #FFFFF0;
  text-align: left;
}
th {
  display: table-cell;
  vertical-align: inherit;
  font-weight: bold;
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
}
.tdr {
  display: table-cell;
  vertical-align: inherit;
  text-align: right;
}
span {
  font-family: arial, Sans-serif;
  font-size: 0.875em;
  font-weight: bold;
  background-color: #FFFFF0;
  color: #00000;
  padding-left: .5em;
  padding-right: .5em;
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
$servername = 'localhost';
$username = 'webuser';
$password = 'alvahugh1';
$dbname = 'grain_bank';

$sql = "select t.grain_type, sum(i.transactionAmount) as 'balance', DATE_FORMAT(max(i.transactionDate),'%m/%d/%Y') as 'lastChange' from grain_bank.grain_type t left join grain_bank.grain_inventory i on t.grainID = i.grainID_fk group by t.grain_type order by t.grain_type";
$conn = mysql_connect($servername, $username, $password, $dbname);
if (!$conn) {die("Connection failed: " . mysql_error());}
if (!mysql_select_db($dbname)) {die("Unable to select mydbname: " . mysql_error());}
$result = mysql_query($sql);
if (!$result) {die("Result failed: " . mysql_error());}
echo "<tbody>\n";
$crow = 0;
while ($row = mysql_fetch_assoc($result)) {
  $rstyle = ($crow & 1) ? 'tr-odd' : 'tr-eve';
	$c1 = $row["grain_type"];
	$c2 = $row["balance"];
	$c3 = $row["lastChange"];
	echo "<tr class='$rstyle'><td>$c1</td><td class='tdr'>$c2</td><td>$c3</td></tr>\n";
	$tot = $tot + $c2;
	$crow++;
}
mysql_free_result($result);
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
$sql = "select t.grain_type,  i.transactionAmount, i.orderID, DATE_FORMAT(i.transactionDate,'%m/%d/%Y') as 'transactionDate' from grain_bank.grain_type t left join grain_bank.grain_inventory i on t.grainID = i.grainID_fk order by i.transactionDate, t.grain_type, i.transactionAmount desc";
$conn = mysql_connect($servername, $username, $password, $dbname);
if (!$conn) {die("Connection failed: " . mysql_error());}
if (!mysql_select_db($dbname)) {die("Unable to select mydbname: " . mysql_error());}
$result = mysql_query($sql);
if (!$result) {die("Result failed: " . mysql_error());}
echo "<tbody>\n";
$crow = 0;
while ($row = mysql_fetch_assoc($result)) {
  $rstyle = ($crow & 1) ? 'tr-odd' : 'tr-eve';
	$c1 = $row["grain_type"];
	$c2 = $row["orderID"];
	$c3 = $row["transactionAmount"];
	$c4 = $row["transactionDate"];
	echo "<tr class='$rstyle'><td>$c1</td><td class='tdr'>$c2</td><td class='tdr'>$c3</td><td>$c4</td></tr>\n";
	$crow++;
}
mysql_free_result($result);
?>
</tbody>
<tfoot>
	<tr>
		<th>&nbsp;</th>
		<th>&nbsp;</th>
		<th class='tdr'></th>
		<th>&nbsp;</th>
	</tr>
</tfoot>
</table>

</body>
</html>

<?php
require_once('library.php');
$lib = new GrainLibrary;
$thispage = $_SERVER['PHP_SELF'];

$post_username = '';
$post_lastname = '';
$post_email = '';
$post_active = '';

//if( isset($_POST['formSubmit']) && $_GET['post_userID'] ) {

// Add
//
if( isset($_POST['formSubmit']) && $_POST['post_u1111serID'] ) {


if(isset($_POST['f_username'])) {
	$post_username = htmlspecialchars($_POST['f_username']);
}
if(isset($_POST['f_lastname'])) {
	$post_lastname = htmlspecialchars($_POST['f_lastname']);
}
if(isset($_POST['f_email'])) {
	$post_email = htmlspecialchars($_POST['f_email']);
}
if(isset($_POST['f_active'])) {
	$post_active = htmlspecialchars($_POST['f_active']);
}

}


// Edit
//
if( $_GET['ID'] ) {
	$userID = intval($_GET['ID']);
	$sql = "SELECT user_id, user_name, email FROM users where user_id = $userID";
	$data = $lib->getData($sql);
	if(!$data['status']) {
		echo($data['message']);
	}
	$result = $data['result'];
	while ($row = mysql_fetch_assoc($result)) {
		$post_username = $row["user_name"];
//		$post_last_name = $row["user_name"];
		$post_email = $row["email"];
		$post_active = $row["active"];
	}
	if($result){mysql_free_result($result);}
}

// Delete
//

// Show
//

?>
<!DOCTYPE html>
<html>
<head>
<link rel="stylesheet" type="text/css" href='grain.css'>
	<title>Grain Bank Users</title>
</head>
<body>
<form action='<?php echo($thispage) ?>' method='post' name='userinput'>
<table class='form' border='0'>
<tr>
	<td width='100px'>First Name</td>
	<td><input type="text" name="f_username" value='<?php echo($post_username) ?>' 
class='field'></td>
</tr>
<tr>
	<td>Last Name</td>
	<td><input type="text" name="f_lastname" value='<?php echo($post_lastname) ?>' 
class='field'></td>
</tr>
<tr>
	<td>Email</td>
	<td><input type="text" name="f_email" value='<?php echo($post_email) ?>' class='field'></td>
</tr>
<tr>
	<td>Active</td>
	<td><input type="text" name="f_active" value='<?php echo($post_active) ?>' class='field'></td>
</tr>
<tr>
	<td>&nbsp;</td>
	<td><input name='formSubmit' type="submit" value="Submit"></td>
</tr>

</table>
</form>
<br>

<?php
$sql = "SELECT user_id, user_name, email FROM users";
$data = $lib->getData($sql);
if(!$data['status']) {
	echo($data['message']);
}
$result = $data['result'];
echo "<table>\n";
echo "<tbody>\n";
echo "<tr><td class='tdl'>User ID</td><td class='tdl'>User Name</td><td class='tdl'>Email</td><td class='tdl'></td></tr>\n";
$crow = 0;
$tot = 0;
while ($row = mysql_fetch_assoc($result)) {
  	$rstyle = ($crow & 1) ? 'tr-odd' : 'tr-eve';
	$c1 = $row["user_id"];
	$c2 = $row["user_name"];
	$c3 = $row["email"];
	echo "<tr class='$rstyle'><td>$c1</td><td>$c2</td><td>$c3</td> <td> <a href='" . $thispage . "?ID=". $c1. "'> $c1 </a>  </td>  </tr>\n";
	$tot = $tot + 1;
	$crow++;
}
if($result){mysql_free_result($result);}
?>
</tbody>
<tfoot>
	<tr>
		<th class='tdr' colspan='4'><?php echo $tot; ?></th>
	</tr>
</tfoot>
</table>
<br>
</body>
</html>

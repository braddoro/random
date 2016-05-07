<?php
class GrainLibrary{

public function getData($sql) {
	$arr_return = array(status => true, result => null, message => null);

	$servername = 'localhost';
	$username = 'webuser';
	$password = 'alvahugh1';
	$dbname = 'grain_bank';
	$conn = mysql_connect($servername, $username, $password, $dbname);
	if (!$conn) {
		$arr_return['status'] = false;
		$arr_return['message'] = mysql_error();
	}
	if (!mysql_select_db($dbname)) {
		$arr_return['status'] = false;
		$arr_return['message'] = mysql_error();
	}
	$result = mysql_query($sql);
	if (!$result) {
		$arr_return['status'] = false;
		$arr_return['message'] = mysql_error();
	}
	$arr_return['result'] = $result;
	unset($conn);

	return $arr_return;
}
}
?>

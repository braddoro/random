<?php
class ServerCode {
	function __construct() {
//		var_dump($arguments);
		//echo(__METHOD__ . ': ' . __LINE__ .'<br />');	
        //$this->grainType();

	}

	public function users() {
		//echo(__METHOD__ . ': ' . __LINE__ .'<br />');	
		//var_dump($ARGUEMENTS);
		require_once('php/data_library.php');
		$data = New DataLibrary();
		
		$sql = "SELECT userID, userUUID, userName, emailAddress, active, addedDate FROM grain_user;";
		$response = $data->getData($sql);
		if(!$response['status']) {
			echo($data->parseErrors($response['message']));
		}
		$result = $response['result'];
		$rows = array();
		while ($row = $result->fetch()) {
			$rows[] = array('userID'			=> $row['userID'], 
		                    'userUUID'		=> $row['userUUID'],
		                    'userName'		=> $row['userName'], 
		                    'emailAddress'	=> $row['emailAddress'],
  		                    'active'			=> $row['active'],
  		                    'addedDate'		=> $row['addedDate']);
		}
		$result->closeCursor();
		unset($response);
		return json_encode($rows);
	}
}	
//$this->grainType();
//echo(__FILE__ . ': ' . __LINE__ .'<br />');	
$Foo = New ServerCode();
echo $Foo->users();
?>

<?php
class ServerCode {
	function __construct() {
//		var_dump($arguments);
		//echo(__METHOD__ . ': ' . __LINE__ .'<br />');	
        //$this->grainType();

	}

	public function grainType() {
		//echo(__METHOD__ . ': ' . __LINE__ .'<br />');	
		//var_dump($ARGUEMENTS);
		require_once('php/data_library.php');
		$data = New DataLibrary();
		
		$sql = "SELECT grainID, grainUUID, grain_type, addedDate FROM grain_type;";
		$response = $data->getData($sql);
		if(!$response['status']) {
			echo($data->parseErrors($response['message']));
		}
		$result = $response['result'];
		$rows = array();
		while ($row = $result->fetch()) {
			$rows[] = array(
				'grainID' => $row['grainID'], 
				'grainUUID' => $row['grainUUID'], 
				'grainType' => $row['grain_type'],
				'addedDate' => $row['addedDate']
				);
		}
		$result->closeCursor();
		unset($response);
		return json_encode($rows);
	}
}	
//$this->grainType();
//echo(__FILE__ . ': ' . __LINE__ .'<br />');	
$Foo = New ServerCode();
echo $Foo->grainType();
?>

<?php
class ServerCode {
	function __construct() {
//		var_dump($arguments);
		//echo(__METHOD__ . ': ' . __LINE__ .'<br />');	
        //$this->grainType();

	}

	public function transaction() {
		//echo(__METHOD__ . ': ' . __LINE__ .'<br />');	
		//var_dump($ARGUEMENTS);
		require_once('php/data_library.php');
		$data = New DataLibrary();
		
		$sql = "select 
	t.grain_type, 
	sum(i.transactionAmount) as 'balance',
	max(transactionDate) as 'lastChange'
from grain_bank.grain_type t
left join grain_bank.grain_inventory i
	on t.grainID = i.grainID_fk
where
	i.userID_fk = 1
group by 
	t.grain_type
order by 
	t.grain_type";
	
		$response = $data->getData($sql);
		if(!$response['status']) {
			echo($data->parseErrors($response['message']));
		}
		$result = $response['result'];
		$rows = array();
		while ($row = $result->fetch()) {
			$rows[] = array(
				'grain_type'	=> $row['grain_type'], 
				'balance'		=> $row['balance'], 
				'lastChange' 	=> $row['lastChange']
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
echo $Foo->transaction();
?>

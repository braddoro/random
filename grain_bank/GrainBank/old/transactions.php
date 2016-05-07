<?php
class ServerCode {
	public function transaction() {
		//echo(__METHOD__ . ': ' . __LINE__ .'<br />');	
		//var_dump($ARGUEMENTS);
		require_once('php/data_library.php');
		$data = New DataLibrary();
		
		$sql = "select 
	t.grain_type, 
	i.transactionAmount,
	i.orderID,
	i.transactionDate
from grain_bank.grain_type t
left join grain_bank.grain_inventory i
	on t.grainID = i.grainID_fk
where 
	i.userID_fk = 1
order by 
	i.transactionDate,
	i.transactionAmount desc,
	t.grain_type;";
	
		$response = $data->getData($sql);
		if(!$response['status']) {
			echo($data->parseErrors($response['message']));
		}
		$result = $response['result'];
		$rows = array();
		while ($row = $result->fetch()) {
			$rows[] = array(
				'grain_type'			=> $row['grain_type'], 
				'transactionAmount'	=> $row['transactionAmount'], 
				'orderID' 				=> $row['orderID'],
				'transactionDate'		=> $row['transactionDate']
				);
		}
		$result->closeCursor();
		unset($response);
		return json_encode($rows);
	}
}	
$Foo = New ServerCode();
echo $Foo->transaction();
?>

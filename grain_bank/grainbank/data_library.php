<?php
class DataLibrary {
	private $hostname;
	private $username;
	private $password;
	private $dbname;
	public  $debugLevel;

	function __construct() {
		$this->hostname = 'localhost';
		$this->username = 'brad';
		$this->password = 'alvahugh1';
		$this->dbname = 'grain_bank';
	}
	
	// GetData
	// 
	// http://www.phpro.org/tutorials/Introduction-to-PHP-PDO.html#4.3
	//
	public function getData($sql) {
		$retArr = array('status' => true, 'result' => null, 'message' => array());

		try {
		    $conn = new PDO("mysql:host=$this->hostname;dbname=$this->dbname", $this->username, $this->password);
		    }
		catch(PDOException $e)
		    {
				$retArr['status'] = false;
				$retArr['message'][] = 'Failed to connect.';
				$retArr['message'][] = $e->getMessage();
				$retArr['message'][] = $conn;
				$retArr['message'][] = 'Error Code: ' . $e->getCode();
				$retArr['message'][] = 'Error File: ' . basename($e->getFile());
				$retArr['message'][] = 'Error Line: ' . $e->getLine();
				return $retArr;
				}
		try {
      $retArr['result'] = $conn->query($sql);
      //$retArr['result'] = $retArr['result']->setFetchMode(PDO::FETCH_ASSOC);
				} 
		catch (PDOException $pe) 
				{
			$retArr['status'] = false;
			$retArr['message'][] = 'Failed to query.';
			$retArr['message'][] = $pe->getMessage();
			$retArr['message'][] = $sql;
			$retArr['message'][] = 'Error Code: ' . $pe->getCode();
			$retArr['message'][] = 'Error File: ' . basename($pe->getFile());
			$retArr['message'][] = 'Error Line: ' . $pe->getLine();
			return $retArr;
				}
		
		return $retArr;
	}

	// parseErrors
	//
	public function parseErrors($inArr) {
		$outStr = null;
		foreach($inArr as $key => $element) {
			$outStr .= $key . " - " . $element."<br />";
		}
		return $outStr;
	}

	// getGUID
	//	
	public function generateGUID() {
    if (function_exists('com_create_guid')){
        return com_create_guid();
    }else{
        mt_srand((double)microtime()*10000); 
        $charid = strtoupper(md5(uniqid(rand(), true)));
        $hyphen = chr(45);// "-"
        $uuid = chr(123)// "{"
            .substr($charid, 0, 8).$hyphen
            .substr($charid, 8, 4).$hyphen
            .substr($charid,12, 4).$hyphen
            .substr($charid,16, 4).$hyphen
            .substr($charid,20,12)
            .chr(125);// "}"
        return $uuid;
    }
	}
}	
?>
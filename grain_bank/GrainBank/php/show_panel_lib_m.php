<?php
	class data {
		function getData($sql){
			$link = mysql_connect($server='65.175.107.2:3306',$username='webapp',$password='alvahugh');
			if (!$link) {
			    die('Error: ' . mysql_error());
			    exit;
			}
			
			mysql_select_db("braddoro");
			$q_data = mysql_query($sql);
			if (!$q_data) {
			    die('Error: ' . mysql_error());
			    exit;
			}
		
			return $q_data;
		}	
	}
?>
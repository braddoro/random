<?php
class display {

	public function __construct() {

	}

	function pageHead() {
		$output = 'This is a Head.';
		echo $output;	
	}
	
	function pageBody() {
		$output = 'This is a Body.';
		echo $output;	
	}

	function pageFoot() {
		$output = 'This is a Foot.';
		echo $output;	
	}
}
?>

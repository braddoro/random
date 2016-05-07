<?php 
	class panelControl {
		public function main(			
			$headDivName,
			$bodyDivName,
			$headContent,
			$bodyContent,
			$panelWidth
		) {
			include_once "show_panel_lib_v.php";
			$objThis_V = new panelView();
			$s_main = $objThis_V->panelContainer(
				$headDivName=$headDivName,
				$bodyDivName=$bodyDivName,
				$headContent=$headContent,
				$bodyContent=$bodyContent,
				$panelWidth=$panelWidth
				);
			
			return $s_main;
		}
	}
?>
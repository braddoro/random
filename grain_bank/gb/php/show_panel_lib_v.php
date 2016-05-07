<?php
	class panelView {
		function panelContainer(
			$headDivName,
			$bodyDivName,
			$headContent,
			$bodyContent,
			$panelWidth
			) {

			$s_panelContainer = "<div style='width:$panelWidth;'>";
			$s_panelContainer .= "<div id='head_$headDivName' name='head_$headDivName' class='phead'>$headContent</div>";
			$s_panelContainer .= "<div id='body_$bodyDivName' name='body_$bodyDivName' class='pbody'>$bodyContent</div>";
			$s_panelContainer .= "</div><br />\n";
			
			return $s_panelContainer;
		}	
	}
?>
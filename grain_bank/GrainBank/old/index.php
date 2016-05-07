<?php
require_once 'display.php';
$disp = new display();
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
	<title>untitled</title>
	<meta http-equiv="content-type" content="text/html;charset=utf-8" />
	<meta name="generator" content="Geany 1.23.1" />
</head>
<body>
	<div id='page_head'>
	<?php
		try {
			echo( $disp->pageHead() );
		} catch (Exception $e) {
			echo $e->getMessage(), "\n";
		}
	?>
	</div>
	<div id='page_body'>
	<?php
		try {
			echo( $disp->pageBody() );
		} catch (Exception $e) {
			echo $e->getMessage(), "\n";
		}
	?>
	</div>
	<div id='page_foot'>
	<?php
		try {
			echo( $disp->pageFoot() );
		} catch (Exception $e) {
			echo $e->getMessage(), "\n";
		}
	?>
	</div>
</body>
</html>

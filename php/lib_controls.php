<?php
	class c_controls
	{
	public function search_list()
		{
		$s_html = "";
		$s_html .= "<input type='text' id='' name='' size='10' value=''>";
		$s_html .= "<button type='button'>Search</button><br/>";
		$s_html .= "<select id='' name='' size=''></select>";
		$s_html .= "<option value='one'>one</option>";
		$s_html .= "<option value='two'>two</option>";
		$s_html .= "</select>";
		return $s_html;
		}
	}
?>

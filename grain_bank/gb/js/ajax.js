function http_post_request(url, sPostString) {
	var results = "";
	var ajax_http_request = false;
	var AjaxTime = new Date();
	sPostString += "&AjaxTime=" + AjaxTime.getTime();
	if (window.XMLHttpRequest) {
		ajax_http_request = new XMLHttpRequest();
		if (ajax_http_request.overrideMimeType) {
			ajax_http_request.overrideMimeType("text/html");
		}
	} else if (window.ActiveXObject) {
		try {
			ajax_http_request = new ActiveXObject("Msxml2.XMLHTTP");
		} catch (e0) {
			try {
				ajax_http_request = new ActiveXObject("Microsoft.XMLHTTP");
			} catch (e1) {
				var strErr = "Object Error";
				strErr += "\nNumber: " + e1.number;
				strErr += "\nDescription: " + e1.description;
				results = strErr;
			}
		}
	}
	if (!ajax_http_request) {
		results = "Cannot create XML/HTTP instance";
	} else {
		ajax_http_request.open("POST", url, false);
		ajax_http_request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
		ajax_http_request.setRequestHeader("Content-length", sPostString.length);
		ajax_http_request.setRequestHeader("Connection", "close");
		ajax_http_request.send(sPostString);
		results = ajax_http_request.responseText;
	}
	return results;
}
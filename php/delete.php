$rval['status'] = $this->STATUS_FAILURE;
$rval['errorMessage'] = CMR_Utility::getString(__CLASS__, 'failure.within', 'Failure within') + ' APPS.XXATDEAS_CUST_MAINT_REG_PKG.CREATE_' . $arguments['CONTACT_POINT_TYPE'] . '_CONTACT_POINT : ' . $procArgs['X_RETURN_CODE'];
$rval['errors'] = $procArgs;
eventLog($this->app, 'Error', __METHOD__, $this->controller->session['username'], $rval['errorMessage'], $procArgs);
return $rval;


public function getString($class, $keyStr, $default='') {
    $output = "!" . $default;
    $lang_file = "lang/en/cmr/server/" . $class . "_en.properties";
    $getDefault = TRUE;
    if (file_exists($lang_file)){
        $lang_array = parse_ini_file($lang_file, true);
        if(array_key_exists($keyStr, $lang_array['locale_strings'])){
            $output = "^" . $lang_array['locale_strings'][$keyStr];
            $getDefault = FALSE;
        }else{
            eventLog('CMR', 'Warn', 'Locale Server', $this->controller->session["username"], 'Missing language key.', "Unable to locate language key |$keyStr| for class |$class| in " . __METHOD__ . ".");
        }
    }else{
        eventLog('CMR', 'Warn', 'Locale Server', $this->controller->session["username"], 'Missing language file.', "Unable to locate language key |$keyStr| for class for class |$class| in file |$lang_file| " . __METHOD__ . ".");
    }
    return $output;
}
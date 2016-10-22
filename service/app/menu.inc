<?php

$v->registerState('menuoption', function() {
global $v;

    $option = $v->getLastInput();

	processMenu($v, $option);

	return $v->tropo->RenderJson();
});
			

$v->registerState('menuagain', function() {
global $v;

    $option = $v->getLastInput();

	switch($option) {
		case 0:
			generateMainMenu($v);
			break;

		default:
			processMenu($v, $option);
	}

	return $v->tropo->RenderJson();
});


function generateMainMenu($v) {

	$menu = "";
	
	$menu .= "1 is Emergency forwarding, ";
	$menu .= "2 recalls your Emergency contacts, ";
	$menu .= "3 retrieves emergency data, ";
	$menu .= "4 loads your calendar, ";
	$menu .= "5 to record an alert, ";
	$menu .= "6 will broadcast your alert, ";

	$options = array("attempts" => 3, "bargein" => true, "choices" => "[1 DIGITS]", "name" => "menuoption", "timeout" => 15);
	$v->tropo->ask("Please select an option,  $menu", $options);

	$v->tropo->on(array("event" => "continue", "next" => $v->getStateURL("menuoption")));
}

			

function processMenu($v, $option) {
	switch($option) {
		case 1:
			processEmergencyForwardList($v);
			return;
		case 2:
			processEmergencyContactList($v);
			break;
		case 3:
			getEmergencyData($v);
			break;
		case 4:
			getNextCalendar($v);
			break;
		case 5:
			processRecordMessage($v);
			break;
		case 6:
			processBroadcastMessage($v);
			break;

		case 9:
			$v->tropo->say("Thank you and goodbye");
			// TODO: This doesn't work!?!?
			$v->tropo->hangup();
			return;
	}

	$options = array("attempts" => 3, "bargein" => true, "choices" => "[1 DIGITS]", "name" => "another", "timeout" => 999);
	$v->tropo->ask("Request another service, or press zero to hear the menu,", $options);
	$v->tropo->on(array("event" => "continue", "next" => $v->getStateURL("menuagain")));

}

?>

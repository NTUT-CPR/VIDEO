(function (){
    var boxs = {
		'uv_box':'', 
		'temperature_box':'',
		'county_box':'',
		'aqi_box':''
		};
    var push_time = 1000;
    var interval_id = undefined;
    var area_id = 1;
	var boxs_iot ={'rood_id_box':'','iot_temperature_box':''}
    
	window.set_data_push = function (_boxs){
        boxs['uv_box'] = _boxs['uv_box'];
        boxs['temperature_box'] = _boxs['temperature_box'];
		boxs['county_box'] = _boxs['county_box'];
		boxs['aqi_box'] = _boxs['aqi_box'];
    }

    window.set_area_id = function (_area_id){
        area_id = parseInt(area_id);
    }//api id.

    window.star_push = function () {
        if (typeof interval_id != "undefined") {
            return;
        }
        interval_id = setInterval(function (){
            get_opendata();
        }, push_time);
    }//start js
	
	
    window.stop_push = function () {
        clearInterval(interval_id);
    }

	//===============================================
    function get_opendata() {
        $.getJSON('https://cp.freddyma.com/api/get_opendata/' + area_id, function (data) {
            set_uv(data['uv']);
            set_temperature(data['weather']);
			set_country(data['uv']);
			set_aqi(data['aqi']);
        });
    }//get opendata
    function set_uv(uv) {
        $(boxs['uv_box']).html(uv['UVI']);
    }
    
    function set_temperature(weather) {
        $(boxs['temperature_box']).html(weather['Temperature']);
    }
	
	function set_country(uv) {
        $(boxs['county_box']).html(uv['County']);
    }
	function set_aqi(aqi) {
        $(boxs['aqi_box']).html(aqi['AQI']);
		/*console.log(aqi['AQI']);
		if(aqi['AQI']>100)aqi['AQI']=50; 
		$(boxs['aqi_box']).html(aqi['AQI']);*/
			//boxs['aqi_box']=50;
    }	
	//================================================
/*	
    function get_iot() {
        $.getJSON('https://cp.freddyma.com/api/get_iot/' + area_id, function (data) {
            set_rootid(data['root_id']);
            set_iot_temperature(data['iot_temperature']);
        });
    }//get iot
	function set_rootid(root_id) {
        $(boxs_iot['rood_id_box']).html(data[0]['root_id']);
    }/*
    function set_iot_temperature(iot_temperature) {
        $(boxs_iot['iot_temperature_box']).html(data[0]['temperature']]);
    }*/
	//================================================
	
	
	

})()
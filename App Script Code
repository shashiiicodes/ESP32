var sheet_id = "1qRP9c61m-0cFPG49rc82sW-entvBJ560m51fIqmFmiw";
var sheet_name = "ESP_DATA";
function doGet(e){
var ss = SpreadsheetApp.openById(sheet_id);
var sheet = ss.getSheetByName(sheet_name);
var serial_number = e.parameter.serial_number;
var date_time = e.parameter.date_time;
var brightness = e.parameter.brightness;
var light = e.parameter.light;
var light_status = e.parameter.light_status;
var soil_moisture = e.parameter.soil_moisture;
var moisture = e.parameter.moisture;
var valve_status = e.parameter.valve_status;
var humidity = e.parameter.humidity;
var temperature = e.parameter.temperature;


sheet.appendRow([serial_number,date_time,brightness,light,light_status,soil_moisture,moisture,valve_status,humidity,temperature]);
}

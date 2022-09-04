apiEndpoint = "https://8f91tmq4za.execute-api.ap-southeast-2.amazonaws.com/default/ledTest"


function ready(){
    let led = document.getElementById("led");
    led.addEventListener("input", setColour, false);
}

function setColour(){
    const hexColor = document.getElementById("led").value
    const red = parseInt(hexColor.substr(1,2), 16);
    const green = parseInt(hexColor.substr(3,2), 16);
    const blue = parseInt(hexColor.substr(5,2), 16);
    console.log("RGB Values: " + red + ", " + green + ", " + blue);
    sendData(red, green, blue);
}

function sendData(red, green, blue){
    let formData = {
        "red": red,
        "green": green,
        "blue": blue
    };
    $.post({
        url: apiEndpoint,
        data: JSON.stringify(formData),
        dataType: 'json'
    }).then(function(responseData) {
        console.log(`Success: `);
        console.log(responseData);
    }).fail(function(data){
        console.log("Data not submitted" + data);
    })
}
import QtQuick 2.0
// A HistoryGraph alapvetően egy Canvas, mivel arra lehet rajzolni.
Canvas {
    // Vannak neki további tulajdonságai, amiket kintről kap
    property var graphTimestamps;
    property var graphVelocities;
    property var graphAccelerations;
    property var graphCarHeights;


    // A Canvas Paint signalja esetén újra kell rajzolni a grafikont.
    onPaint: {
        // A grafikon rajzolát itt, a QML oldalon, JavaScripben megírva történik.
        // Kell egy "drawing context", amire rajzolni tudunk.
        var context = getContext("2d");

        context.fillStyle = "white"
        context.fillRect(0, 0, width, height);

        switch(graphCarHeights[graphCarHeights.length-1]) {
          case 24:
            context.drawImage(image1, width*0.8, height*0.1, width*0.2, height*0.7);
          break;
          case 23:
              context.drawImage(image2, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 22:
              context.drawImage(image3, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 21:
              context.drawImage(image4, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 20:
              context.drawImage(image5, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 19:
              context.drawImage(image6, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 18:
              context.drawImage(image7, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 17:
              context.drawImage(image8, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 16:
              context.drawImage(image9, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 15:
              context.drawImage(image10, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 14:
              context.drawImage(image11, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 13:
              context.drawImage(image12, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 12:
              context.drawImage(image13, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 11:
              context.drawImage(image14, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 10:
              context.drawImage(image15, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 9:
              context.drawImage(image16, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 8:
              context.drawImage(image17, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 7:
              context.drawImage(image18, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 6:
              context.drawImage(image19, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 5:
              context.drawImage(image20, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 4:
              context.drawImage(image21, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 3:
              context.drawImage(image22, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 2:
              context.drawImage(image23, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          case 1:
              context.drawImage(image24, width*0.8, height*0.1, width*0.2, height*0.7);
            break;
          default:
            // code block
        }



        // Vízszintes vonalak berajzolása a függőleges pozíció, a szín és a függőleges
        //  skálázás megadásával. (Erre van külön függvényünk.)
        drawHorizontalLine(context, height*0.571, "rgba(0,0,0,1)", 1.0)

        // Az adatsorok megrajzolása (a graphTimestamps értékét jelenleg nem használjuk).
        //  Ez is külön függvénybe került.
        //drawDataset(context, graphVelocities, "rgba(110,220,110,1)", 5.0);
        //drawDataset(context, graphAccelerations, "rgba(220,110,110,1)", 5.0);
        drawTrack(context, graphCarHeights, "rgba(220,110,220,1)", height/28);
        drawDataset(context, graphCarHeights, "rgba(110,110,220,1)", height/60);
    } // onPaint vége

    // Vízszintes vonal berajzolása.
    function drawHorizontalLine(context, y, strokeStyle)
    {
        context.beginPath();
        context.lineWidth = 1;
        context.strokeStyle = strokeStyle;
        // Mozgás a vonal elejére.
        context.moveTo(0, y);
        // Vonal végére mozgás.
        context.lineTo(width, y) ;
        context.stroke();
    }

    function drawTrack(context, datarow, strokeStyle, verticalScaler)
    {
        var offsetY = height*0.571;
        var offsetX = width*0.79;

        context.fillStyle = "rgba(128,128,128,1)";

        // A vektoron végigmenve visszafele behúzzuk a nyomvonalat.
        for(var i=datarow.length-1; i>=0; i--)
        {
            var strength = verticalScaler * (24 - datarow[i]);
            context.fillRect(offsetX - 10 * (datarow.length-1 - i), offsetY - strength/2, 11, strength);
        }
    }

    function drawDataset(context, datarow, strokeStyle, verticalScaler)
    {
        var offsetY = height/2;
        var offsetX = width*0.8;

        context.beginPath();
        context.lineWidth = 2;
        context.strokeStyle = strokeStyle;

        context.moveTo(offsetX, offsetY - verticalScaler * datarow[datarow.length-1]);
        // A vektoron végigmenve visszafele behúzzuk a grafikon szakaszait.
        for(var i=datarow.length-1; i>=0; i--)
        {
            context.lineTo(offsetX - 10 * (datarow.length-1 - i), offsetY - verticalScaler * datarow[i]);
        }
        context.stroke();
    }

    Image{
        id: image1
        visible: false
        source: '8-512 (1).png'
    }
    Image{
        id: image2
        visible: false
        source: '8-512 (2).png'
    }
    Image{
        id: image3
        visible: false
        source: '8-512 (3).png'
    }
    Image{
        id: image4
        visible: false
        source: '8-512 (4).png'
    }
    Image{
        id: image5
        visible: false
        source: '8-512 (5).png'
    }
    Image{
        id: image6
        visible: false
        source: '8-512 (6).png'
    }
    Image{
        id: image7
        visible: false
        source: '8-512 (7).png'
    }
    Image{
        id: image8
        visible: false
        source: '8-512 (8).png'
    }
    Image{
        id: image9
        visible: false
        source: '8-512 (9).png'
    }
    Image{
        id: image10
        visible: false
        source: '8-512 (10).png'
    }
    Image{
        id: image11
        visible: false
        source: '8-512 (11).png'
    }
    Image{
        id: image12
        visible: false
        source: '8-512 (12).png'
    }
    Image{
        id: image13
        visible: false
        source: '8-512 (13).png'
    }
    Image{
        id: image14
        visible: false
        source: '8-512 (14).png'
    }
    Image{
        id: image15
        visible: false
        source: '8-512 (15).png'
    }
    Image{
        id: image16
        visible: false
        source: '8-512 (16).png'
    }
    Image{
        id: image17
        visible: false
        source: '8-512 (17).png'
    }
    Image{
        id: image18
        visible: false
        source: '8-512 (18).png'
    }
    Image{
        id: image19
        visible: false
        source: '8-512 (19).png'
    }
    Image{
        id: image20
        visible: false
        source: '8-512 (20).png'
    }
    Image{
        id: image21
        visible: false
        source: '8-512 (21).png'
    }
    Image{
        id: image22
        visible: false
        source: '8-512 (22).png'
    }
    Image{
        id: image23
        visible: false
        source: '8-512 (23).png'
    }
    Image{
        id: image24
        visible: false
        source: '8-512 (24).png'
    }

}


import QtQuick 2.0
// A HistoryGraph alapvetően egy Canvas, mivel arra lehet rajzolni.
Canvas {
    // Vannak neki további tulajdonságai, amiket kintről kap
    property var graphTimestamps;
    property var graphVelocities;
    property var graphAccelerations;
    property var carHeight;


    // A Canvas Paint signalja esetén újra kell rajzolni a grafikont.
    onPaint: {
        // A grafikon rajzolát itt, a QML oldalon, JavaScripben megírva történik.
        // Kell egy "drawing context", amire rajzolni tudunk.
        var context = getContext("2d");

        context.fillStyle = "white"
        context.fillRect(0, 0, width, height);
        switch(carHeight) {
          case 24:
            context.drawImage(image1, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
          break;
          case 23:
              context.drawImage(image2, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 22:
              context.drawImage(image3, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 21:
              context.drawImage(image4, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 20:
              context.drawImage(image5, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 19:
              context.drawImage(image6, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 18:
              context.drawImage(image7, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 17:
              context.drawImage(image8, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 16:
              context.drawImage(image9, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 15:
              context.drawImage(image10, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 14:
              context.drawImage(image11, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 13:
              context.drawImage(image12, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 12:
              context.drawImage(image13, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 11:
              context.drawImage(image14, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 10:
              context.drawImage(image15, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 9:
              context.drawImage(image16, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 8:
              context.drawImage(image17, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 7:
              context.drawImage(image18, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 6:
              context.drawImage(image19, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 5:
              context.drawImage(image20, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 4:
              context.drawImage(image21, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 3:
              context.drawImage(image22, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 2:
              context.drawImage(image23, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          case 1:
              context.drawImage(image24, parent.width*0.02, parent.height*0.1, parent.width*0.2, parent.height*0.7);
            break;
          default:
            // code block
        }



        // Vízszintes vonalak berajzolása a függőleges pozíció, a szín és a függőleges
        //  skálázás megadásával. (Erre van külön függvényünk.)
        drawHorizontalLine(context, 0.0, "rgba(0,0,0,1)", 1.0)
       // drawHorizontalLine(context, 5.0, "rgba(100,100,100,1)", 5.0)
       // drawHorizontalLine(context, -5.0, "rgba(100,100,100,1)", 5.0)
       // drawHorizontalLine(context, 10.0, "rgba(0,0,0,1)", 5.0)
       // drawHorizontalLine(context, -10.0, "rgba(0,0,0,1)", 5.0)

        // Az adatsorok megrajzolása (a graphTimestamps értékét jelenleg nem használjuk).
        //  Ez is külön függvénybe került.
        //drawDataset(context, graphVelocities, "rgba(110,220,110,1)", 5.0);
        //drawDataset(context, graphAccelerations, "rgba(220,110,110,1)", 5.0);
    } // onPaint vége

    // Vízszintes vonal berajzolása.
    function drawHorizontalLine(context, dataValue, strokeStyle, verticalScaler)
    {
        var offset = height/2;

        context.beginPath();
        context.lineWidth = 1;
        context.strokeStyle = strokeStyle;
        // Mozgás a vonal elejére.
        context.moveTo(0, parent.height*0.571);
        // Vonal végére mozgás.
        context.lineTo(width,  parent.height*0.571) ;
        context.stroke();
    }

    function drawDataset(context, datarow, strokeStyle, verticalScaler)
    {
        var offset = height/2;

        context.beginPath();
        context.lineWidth = 3;
        context.strokeStyle = strokeStyle;
        context.moveTo(0, offset-datarow[0]);
        // A vektoron végigmenve behúzzuk a grafikon szakaszait.
        for(var i=0; i<graphVelocities.length;i++)
        {
            context.lineTo(10*i, offset - verticalScaler * datarow[i]);
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


function tableNums(){
    const tableBody = document.getElementById("table-body");
    let i = 5;
    //creates new cell in new row
    while(i <=45){
        //create new row
        const newRow = document.createElement("tr");

        //create first cell
        const newCell = document.createElement("td");
        const textNode = document.createTextNode(i);
        newCell.appendChild(textNode);
        newRow.appendChild(newCell);
       
        //create squared cell
        const newsquare = document.createElement("td");
        const textSqaure = document.createTextNode(Math.pow(i,2));
        newsquare.appendChild(textSqaure);
        newRow.appendChild(newsquare);

        //create cubed cell
        const newCubed = document.createElement("td");
        const textCubed = document.createTextNode(Math.pow(i,3));
        newCubed.appendChild(textCubed);
        newRow.appendChild(newCubed);

        //add row to the table
        tableBody.appendChild(newRow);

        ++i;
    }
}

let fib =[];
function fibonacci(num) {
    
    let num1 = 0;
    let num2 = 1;
    let sum;
    fib[0] = num1;
    fib[1] = num2
    if (num === 1) {
        return num1;
    } else if (num === 2) {
        return num2;
    } else {
        for (let i = 3; i <= num; i++) {
            sum = num1 + num2;
            num1 = num2;
            num2 = sum;
            fib[i-1]= sum;
        }
        printFibs();
        return num2;
    }
    
}


function printFibs(){
    const text = fib.join("-");
    let sp = document.getElementById("fib");
    sp.textContent = text;
}


function sortNums() {
    document.getElementById("unsorted").textContent = "Unsorted random Numbers"
    Array.prototype.sortDescending = function() {
        this.sort(function(a, b) {
          return b - a; // Sort in descending order
        });
        return this; 
      };

    const numbers = [];
    for(let i=0; i<20;++i){
    numbers[i] = Math.floor(Math.random() *(300 - 0) );
    }
    document.getElementById("nums1").textContent = numbers;
    numbers.sortDescending();

    document.getElementById("sorted").textContent = "Sorted Random Numbers";
      document.getElementById("nums2").textContent = numbers;
}
    

function words(){
    function paragraphToArray(paragraph) {
        return paragraph.split(" "); 
      }
      
      let myParagraph = document.getElementById("paragraph").textContent;
      let wordArray = paragraphToArray(myParagraph);
      document.getElementById("Unsorted").innerHTML = wordArray.join(" | ");
      wordArray.sort();
      myParagraph = wordArray.join(" | ");
      document.getElementById("sorted").innerHTML = myParagraph;
      console.log(myParagraph); 

}



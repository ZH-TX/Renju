/**
 * UI 界面
 * @author zhtx
 * 
 * 1. 悔棋功能(最重要的一个功能), 提示功能, 优化算法
 * 
 */

var canvas = document.getElementById("chess");
var context = canvas.getContext("2d");
var me = true;              // 判断该轮黑白棋落子权
var over = false;           // 判断游戏是否结束
var chessBoard = [];        // 棋盘二维数组,存储棋盘信息

/**
 * 开始按钮逻辑:初始化棋盘,并让电脑黑棋先行(7,7)位置
 */
function startGame() {
    
    // 初始化棋盘信息二维数组的形式表示
    for (var i = 0; i < 15; i++) {
        chessBoard[i] = [];
        for (var j = 0; j < 15; j++) {
            chessBoard[i][j] = 0;
        }
    }
    
    // 清除棋盘
    cleanChess();
    // 绘制棋盘
    drawChess();
    
    // 轮到玩家(白棋)行棋
    me = true;
    // 重置游戏结束标志
    over = false;
    
    // 初始化赢法统计数组
    for (var i = 0; i < count; i++) {
        myWin[i] = 0;
        aiWin[i] = 0;
    }
    
    // 让电脑先行，(7,7)处绘制黑棋，并存储信息
    oneStep(7, 7, false);
    chessBoard[7][7] = 2;   //2 为电脑下的子
}

/**
 * 清除棋盘
 */
function cleanChess() {
    context.fillStyle = "#DCBEA2";
    context.fillRect(0, 0, canvas.width, canvas.height);
}

/**
 * 绘制棋盘
 */
function drawChess() {
    for (var i = 0; i < 15; i++) {
        context.strokeStyle = "#4E4E4E";
        // context.translate(0.5,0.5);
        // context.lineWidth = .5;

        context.beginPath();
        context.moveTo(15.5 + i *30, 15);
        context.lineTo(15.5 + i *30, canvas.height - 15);
        context.closePath();
        context.stroke();

       
        
        context.beginPath();
        context.moveTo(15, 15.5 + i *30);
        context.lineTo(canvas.width - 15, 15.5 + i * 30);
        context.closePath();
        context.stroke();
    }
}

/**
 * 绘制棋子
 * @param i     棋子x轴位置
 * @param j     棋子y轴位置
 * @param me    棋子颜色
 */
function oneStep(i, j ,me) {
    context.beginPath();
    context.arc(15 + i * 30, 15 + j * 30, 13, 0, 2 * Math.PI);
    context.closePath();
    var gradient = context.createRadialGradient(15 + i * 30 + 2, 15 + j * 30 - 2, 13, 15 + i * 30 + 2, 15 + j * 30 - 2, 0);
    if (me) {
        // context.textAlign = "center";    
        // context.fillStyle ="red"
        // context.font = "10px serif";
        // context.fillText("21564564", 15 + i * 30, 15 + j * 30);
        gradient.addColorStop(0, "#D1D1D1");
        gradient.addColorStop(1, "#F9F9F9");
       
    } else {
        gradient.addColorStop(0, "#0A0A0A");
        gradient.addColorStop(1, "#636766");
    }
    context.fillStyle = gradient;
    context.fill();
}


/**
 * canvas 鼠标点击事件
 * @param e
 */
canvas.onclick = function(e) {
    if (over) {
        return;
    }

    var x = e.offsetX;
    var y = e.offsetY;
    var i = Math.floor(x / 30);
    var j = Math.floor(y / 30);

    // 如果该位置没有棋子,则允许落子
    if(chessBoard[i][j] == 0) {
        // 绘制棋子(玩家)
        oneStep(i, j, me);
        // console.log(`我的得分:${fiveGo().myScore}`)

        // 改变棋盘信息(该位置有棋子)   
        chessBoard[i][j] = 1;      //玩家为赋值为1

        // 遍历赢法统计数组
        for (var k = 0; k < count; k++) {
            if (wins[i][j][k]) {
                // 如果存在赢法,则玩家此赢法胜算+1(赢法为5胜取胜)
                myWin[k] ++;
                // 如果存在赢法,则电脑此赢法胜算赋值为6(永远不等于5,永远无法在此处取胜)
                aiWin[k] = 6;
                // 玩家落子后,此处赢法数组凑够5,玩家取胜
                if (myWin[k] == 5) {
                    window.alert("还是你厉害,狗子打不过你");
                    
                    // 游戏结束
                    over = true;
                }
            }

        }
        //fiveGo2()

        // 如果游戏没有结束,轮到电脑行棋
        if (!over) {
            me = !me;
            fiveGo();
            // console.log(`AI  得分:${aiScore[i][j]}`);
        }
    }
};



var wins = [];      // 赢法统计数组
var count = 0;      // 赢法统计数组的计数器

// 初始化    赢法统计数组
for (var i = 0; i < 15; i++) {
    wins[i] = [];
    for (var j = 0; j < 15; j++) {
        wins[i][j] = []
    }
}

var myWin = [];
var aiWin = [];

// 阳线纵向90°的赢法  S-N
for (var i = 0; i < 15; i++) {
    for (var j = 0; j < 11; j++) {  //為什麼是十一有點不太理解
        for (var k = 0; k < 5; k++) {
            wins[i][j + k][count] = true; //1
        }
        count++;
    }
}

// 阳线横向0°的赢法  W-E
for (var i = 0; i < 15; i++) {
    for (var j = 0; j < 11; j++) {
        for (var k = 0; k < 5; k++) {
            wins[j + k][i][count] = true;
        }
        count++;
    }
}

// 阴线斜向135°的赢法     NE135
for (var i = 0; i < 11; i++) {
    for (var j = 0; j < 11; j++) {
        for (var k = 0; k < 5; k++) {
            wins[i + k][j + k][count] = true;
        }
        count++;
    }
}

// 阴线斜向45°的赢法   NE45
for (var i = 0; i < 11; i++) {
    for (var j = 14; j > 3; j--) {
        for (var k = 0; k < 5; k++) {
            wins[i + k][j - k][count] = true;
        }
        count++;
    }
}

/**
 * AI  (这里变量在各个文件中需要小心)
 * 电脑算法  ( 可以分清, 进攻, 与防御两种状态)
 * 1. 这里评分体系还是有些问题, 没有考虑各种活与眠的状态 加入一些状态改变权重
 * 
 * 
 */
function fiveGo() {
    if (over) {
        return;
    }

    var u = 0;              // 电脑预落子的x位置
    var v = 0;              // 电脑预落子的y位置
    var myScore = [];       // 玩家的分数
    var aiScore = [];   // 电脑的分数
    var max = 0;            // 最优位置的分数

    // 初始化分数的二维数组
    for (var i = 0; i < 15; i++) {
        myScore[i] = [];
        aiScore[i] = [];
        for (var j = 0; j < 15; j++) {
            myScore[i][j] = 0;
            aiScore[i][j] = 0;
        }
    }
    // 贪心算法 
    // 通过赢法统计数组为两个二维数组分别计分 (评分体系,没有使用极大/极小值) 有时还是会出错判断体系有问题
    for (var i = 0; i < 15; i++) {
        for (var j = 0; j < 15; j++) {
            if (chessBoard[i][j] === 0) {
                for (var k = 0; k < count; k++) {  //count 计步器
                    if (wins[i][j][k]) {

                        if (myWin[k] == 1) {
                            myScore[i][j] += 15;
                        } else if (myWin[k] == 2) {
                            myScore[i][j] += 400;
                        } else if (myWin[k] == 3) {
                            myScore[i][j] += 1800;
                        } else if (myWin[k] == 4) {
                            myScore[i][j] += 100000;
                        }
                        //ai 计数 (ai先下有优势分值大一些)
                        if (aiWin[k] == 1) {
                            aiScore[i][j] += 35;
                        } else if (aiWin[k] == 2) {
                            aiScore[i][j] += 800;
                        } else if (aiWin[k] == 3) {
                            aiScore[i][j] += 15000;
                        } else if (aiWin[k] == 4) {
                            aiScore[i][j] += 800000;
                        }

                        
                    }
                    // console.log(`我的得分:myScore[i][j]`);
                    // console.log(`AI  得分:aiScore[i][j]`);
                    
                    
                }
                
                // 如果玩家(i,j)处比目前最优的分数大，则落子在(i,j)处
                if (myScore[i][j] > max) {
                    max = myScore[i][j];
                    u = i;
                    v = j;
                } else if (myScore[i][j] == max) {
                    
                    // 如果玩家(i,j)处和目前最优分数一样大，则比较电脑在该位置和预落子的位置的分数
                    if (aiScore[i][j] > aiScore[u][v]) {
                        u = i;
                        v = j;
                    }
                }
              
                // 如果电脑(i,j)处比目前最优的分数大，则落子在(i,j)处
                if (aiScore[i][j] > max) {
                    
                    max  = aiScore[i][j];
                    u = i;
                    v = j;
                } else if (aiScore[i][j] == max) {
                    // 如果电脑(i,j)处和目前最优分数一样大，则比较玩家在该位置和预落子的位置的分数
                    if (myScore[i][j] > myScore[u][v]) {
                        u = i;
                        v = j;
                    }
                }
            }  
        }
       
    }

    oneStep(u, v, false);
    chessBoard[u][v] = 2;

    for (var k = 0; k < count; k++) {
        if (wins[u][v][k]) {
            aiWin[k] ++;
            myWin[k] = 6;
            if (aiWin[k] === 5) {
                window.alert("连狗子都打不过, 你吃屎去吧 ");
                over = true;
            }
        }
    }

    if (!over) {
       me = !me;
    }


    // return {myScore,aiScore}
}



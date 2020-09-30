#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 15
#define CHARSIZE 2
#define AIFIRST 1
#define NOONEWIN 0
#define AIWIN 1
#define HUMWIN 2
#define WHITE 2
#define BLACK 1
#define FIVE 5
#define TOP 5

void initRecordBorard(void);
void recordtoDisplayArray(void);
void displayBoard(void);
void nextStepGenerator(void);                                  //生成下一步需要什么的，算法核心
int winnerJudger(void);                                        //判断胜负的
void changeBoard(struct NextStep,int);                         //把nextstep的东西写进棋盘里
int numberInARowRight(int color, int rows, int cols);          //判断本行向右有几子连住
int numberInARowDown(int color, int rows, int cols);           //判断本行向下有几子连住
int numberInARowCornerLeft(int color, int rows, int cols);     //判断本行向左下有几子连住
int numberInARowCornerRight(int color, int rows, int cols);    //判断本行向右下有几子连住
int scoreGenerator(int rows, int cols, int preColer);          //生成一个点的分数
void generateNextStep(void);                                   //不用了
void topScore(int top);                                        //把前top大的数存到一个新数组里
int oppSocreGenerator(int rows, int cols, int preColer);                                   //计算对方分数的


//棋盘使用的是GBK编码，每一个中文字符占用2个字节。

//棋盘基本模板 
char aInitDisplayBoardArray[SIZE][SIZE * CHARSIZE + 1] =
{
		"┏┯┯┯┯┯┯┯┯┯┯┯┯┯┓",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┗┷┷┷┷┷┷┷┷┷┷┷┷┷┛"
};
//此数组用于显示棋盘 
char aDisplayBoardArray[SIZE][SIZE * CHARSIZE + 1];

char play1Pic[] = "●";//黑棋子;
char play1CurrentPic[] = "▲";

char play2Pic[] = "◎";//白棋子;
char play2CurrentPic[] = "△";


//此数组用于记录棋盘格局 
int aRecordBoard[SIZE][SIZE];

//记录本局是人先还是机先
int AIfirstHandMode;

//记录下一步下哪个颜色的子
int goCloro = 1;
//定义落子位置的结构的文字版
struct NextStep
{
	char col;
	int row;
};

struct NextStep AINextStep; //记录AI落子位置的结构
struct NextStep HumNextStep;  //记录人落子位置的结构

//可能的棋盘格局
int aPossibleBoard[SIZE][SIZE];

//记录连珠信息的结构
struct SingleInfo
{
	int linkNum;
	int oppNum;
};

int scoreNextStep[SIZE][SIZE];
int topGetScore[TOP];
int topChoices[TOP][2];

int main()
{
	initRecordBorard();  //初始化棋盘格局
	printf("人先请输入0，机先请输入1：");
	scanf_s
	
	
	
	("%d", &AIfirstHandMode);
	recordtoDisplayArray();
	displayBoard();
	while (getchar() != '\n')
	{
		;
	}
	if (AIfirstHandMode == AIFIRST)                                        //如果机器先走，则直接开始一次搜索
	{
		nextStepGenerator();
		recordtoDisplayArray();
		goCloro = 3 - goCloro;
		displayBoard();
		printf("AI落子在%c %d\n", AINextStep.col, AINextStep.row);
	}

	while (winnerJudger() == NOONEWIN)
	{
		scanf_s("%c", &HumNextStep.col);
		scanf_s("%d", &HumNextStep.row);
		while (getchar() != '\n')
		{
			;
		}
		changeBoard(HumNextStep, goCloro);
		recordtoDisplayArray();
		goCloro = 3 - goCloro;
		displayBoard();
		nextStepGenerator();
		recordtoDisplayArray();
		goCloro = 3 - goCloro;
		displayBoard();
		printf("AI落子在%c %d\n", AINextStep.col, AINextStep.row);
	}

	if (winnerJudger() == BLACK)
	{
		printf("BLACK wins.\n");
	}
	if (winnerJudger() == WHITE)
	{
		printf("WHITE wins.\n");
	}
		return 0;
}

//初始化棋盘格局 
void initRecordBorard(void) {
	//通过双重循环，将aRecordBoard清0
	for (size_t i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			aRecordBoard[i][j] = 0;
		}
	}
}

//将aRecordBoard中记录的棋子位置，转化到aDisplayBoardArray中
void recordtoDisplayArray(void) {
	//第一步：将aInitDisplayBoardArray中记录的空棋盘，复制到aDisplayBoardArray中
	for (size_t i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE * CHARSIZE + 1; j++)
		{
			aDisplayBoardArray[i][j] = aInitDisplayBoardArray[i][j];
		}
	}
	//第二步：扫描aRecordBoard，当遇到非0的元素，将●或者◎复制到aDisplayBoardArray的相应位置上
	//注意：aDisplayBoardArray所记录的字符是中文字符，每个字符占2个字节。●和◎也是中文字符，每个也占2个字节。
	for (size_t i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (aRecordBoard[i][j] == 1)
			{
				for (size_t k = 0; k < CHARSIZE; k++)
				{
					aDisplayBoardArray[i][j * CHARSIZE + k] = play1Pic[k];
				}
			}
			if (aRecordBoard[i][j] == 2)
			{
				for (size_t k = 0; k < CHARSIZE; k++)
				{
					aDisplayBoardArray[i][j * CHARSIZE + k] = play2Pic[k];
				}
			}
		}
	}
}


//显示棋盘格局 
void displayBoard(void) {
	int i;
	//第一步：清屏
	system("cls");   //清屏  
	//第二步：将aDisplayBoardArray输出到屏幕上
	for (size_t i = 0; i < SIZE; i++)
	{
		printf("%s\n", aDisplayBoardArray[i]);
	}
	//第三步：输出最下面的一行字母A B .... 
	for (size_t i = 0; i < SIZE; i++)
	{
		printf("%c ", 'A' + i);
	}
	printf("\n");
}

void nextStepGenerator(void)
{
	int preColer = goCloro;
	//初始化得分数组
	for (size_t i = 0; i < SIZE; i++)
	{
		for (size_t j = 0; j < SIZE; j++)
		{
			scoreNextStep[i][j] = 0;
		}
	}
	for (size_t rows = 0; rows < SIZE; rows++)
	{
		for (size_t cols = 0; cols < SIZE; cols++)
		{
			scoreNextStep[rows][cols] = scoreGenerator(rows, cols, goCloro);
		}
	}
	for (size_t i = 0; i < FIVE; i++)
	{
		topGetScore[i] = -10000;
	}
	topScore(TOP);
	//给这五个选择再模拟一步
	/*for (size_t top = 0; top < TOP; top++)
	{
		for (size_t j = 0; j < SIZE; j++)
		{
			for (size_t k = 0; k < SIZE; k++)
			{
				aPossibleBoard[j][k] = aRecordBoard[j][k];
			}
		}
		//把新选的这一步下进去

		aPossibleBoard[topChoices[top][0]][topChoices[top][1]] = goCloro;
		int preColer = 3 - goCloro;
		int scoreNextStep[SIZE][SIZE];
		//用相同的方式给对手打分
		for (size_t i = 0; i < SIZE; i++)
		{
			for (size_t j = 0; j < SIZE; j++)
			{
				scoreNextStep[i][j] = 0;
			}
		}
		for (size_t rows = 0; rows < SIZE; rows++)
		{
			for (size_t cols = 0; cols < SIZE; cols++)
			{
				scoreNextStep[rows][cols] = scoreGenerator(rows, cols, goCloro);
			}
		}
		int topChoices[TOP][2];   //记录最好的top个选择
		int topGetScore[TOP];     //这五个top选择的新得分
		topScore(TOP);            //把这五个新位置从scorenextstep里选出来，存到topchoice里
		int bestChoice[2];
		int maxScore = -10000;
		for (size_t i = 0; i < SIZE; i++)               //选出最高得分
		{
			for (size_t j = 0; j < SIZE; j++)
			{
				if (scoreNextStep[i][j] > maxScore)
				{
					maxScore = scoreNextStep[i][j];
				}
			}
		}
		topGetScore[top] = maxScore;
	}
	//从五个分数中选择最低的，因为我是两步，现在记录的是对方的得分
	int choiceInTop;
	int least = 10000;
	for (size_t i = 0; i < TOP; i++)
	{
		if (topGetScore[i] < least)
		{
			least = topGetScore[i];
			choiceInTop = i;
		}
	}*/
	//得到最终的结果
	aRecordBoard[topChoices[0][0]][topChoices[0][1]] = goCloro;
	AINextStep.row = topChoices[0][0] + 1;
	AINextStep.col = 'a' + topChoices[0][1];
}

int winnerJudger(void)
{
	int winner = 0;
	//四个循环,第一个看横着的
	for (size_t rows = 0; rows < SIZE; rows++)
	{
		for (int cols = 0; cols < SIZE - 4; cols++)
		{
			if (numberInARowRight(BLACK, rows, cols) == 5)
			{
				winner = BLACK;
				break;
			}
			if (numberInARowRight(WHITE, rows, cols) == 5)
			{
				winner = WHITE;
				break;
			}
		}
	}
	//第二个看竖着的
	for (size_t rows = 0; rows < SIZE - 4; rows++)
	{
		for (int cols = 0; cols < SIZE; cols++)
		{
			if (numberInARowDown(BLACK, rows, cols) == 5)
			{
				winner = BLACK;
				break;
			}
			if (numberInARowDown(WHITE, rows, cols) == 5)
			{
				winner = WHITE;
				break;
			}
		}
	}
	//第三个看左下的
	for (size_t rows = 4; rows < SIZE; rows++)
	{
		for (int cols = 4; cols < SIZE; cols++)
		{
			if (numberInARowCornerLeft(BLACK, rows, cols) == 5)
			{
				winner = BLACK;
				break;
			}
			if (numberInARowCornerLeft(WHITE, rows, cols) == 5)
			{
				winner = WHITE;
				break;
			}
		}
	}
	for (size_t rows = 0; rows < SIZE - 4; rows++)
	{
		for (int cols = 0; cols < SIZE - 4; cols++)
		{
			if (numberInARowCornerRight(BLACK, rows, cols) == 5)
			{
				winner = BLACK;
				break;
			}
			if (numberInARowCornerRight(WHITE, rows, cols) == 5)
			{
				winner = WHITE;
				break;
			}
		}
	}
	return winner;
}

void changeBoard(struct NextStep nextStepPosition, int color)
{
	aRecordBoard[nextStepPosition.row - 1][nextStepPosition.col - 'a'] = color;
}

int numberInARowRight(int color, int rows, int cols)
{
	int length = 0;
	for (size_t site = 0; site < FIVE; site++)
	{
		if (aRecordBoard[rows][cols + site] == color)
		{
			length++;
		}
		else
		{
			break;
		}
	}
	return length;
}

int numberInARowDown(int color, int rows, int cols)
{
	int length = 0;
	for (size_t site = 0; site < FIVE; site++)
	{
		if (aRecordBoard[rows + site][cols] == color)
		{
			length++;
		}
		else
		{
			break;
		}
	}
	return length;
}

int numberInARowCornerLeft(int color, int rows, int cols)
{
	int length = 0;
	for (size_t site = 0; site < FIVE; site++)
	{
		if (aRecordBoard[rows + site][cols - site] == color)
		{
			length++;
		}
		else
		{
			break;
		}
	}
	return length;
}

int numberInARowCornerRight(int color, int rows, int cols)
{
	int length = 0;
	for (size_t site = 0; site < FIVE; site++)
	{
		if (aRecordBoard[rows + site][cols +site] == color)
		{
			length++;
		}
		else
		{
			break;
		}
	}
	return length;
}

int scoreGenerator(int rows, int cols, int preColer)
{
	int myscore, oppScore;
	struct SingleInfo aroundInfo[4];
	if (aRecordBoard[rows][cols] == 0)
	{
		//首先计算左右连了几个
		int leftrightlength = 0;
		//算左边连了几个
		for (size_t site = 1; cols - site >= 0 && aRecordBoard[rows][cols - site] == preColer; site++)
		{
			leftrightlength++;
		}
		//判断左边头上是死是活
		aroundInfo[0].oppNum = 0;
		if (cols - leftrightlength - 1 < 0 || aRecordBoard[rows][cols - leftrightlength - 1] == 3 - preColer)
		{
			aroundInfo[0].oppNum++;
		}
		int rightLength = 0;
		for (size_t site = 1; cols + site < SIZE && aRecordBoard[rows][cols + site] == preColer; site++)
		{
			leftrightlength++;
			rightLength++;
		}
		//判断右边是死是活
		if (cols + rightLength + 1 >= SIZE || aRecordBoard[rows][cols + rightLength + 1] == 3 - preColer)
		{
			aroundInfo[0].oppNum++;
		}
		leftrightlength++;              //算上这个点本身
		aroundInfo[0].linkNum = leftrightlength;
		//然后计算上下连了几个
		int upDownLength = 0;
		//算上边连了几个
		for (size_t site = 1; rows - site >= 0 && aRecordBoard[rows - site][cols] == preColer; site++)
		{
			upDownLength++;
		}
		//判断上边头上是死是活
		aroundInfo[1].oppNum = 0;
		if (rows - upDownLength - 1 < 0 || aRecordBoard[rows - upDownLength - 1][cols] == 3 - preColer)
		{
			aroundInfo[1].oppNum++;
		}
		int downLength = 0;
		//算下边连了几个
		for (size_t site = 1; rows + site < SIZE && aRecordBoard[rows + site][cols] == preColer; site++)
		{
			upDownLength++;
			downLength++;
		}
		//判断下边是死是活
		if (rows + downLength + 1 >= SIZE || aRecordBoard[rows + downLength + 1][cols] == 3 - preColer)
		{
			aroundInfo[1].oppNum++;
		}
		upDownLength++;              //算上这个点本身
		aroundInfo[1].linkNum = upDownLength;
		//然后计算左下右上连了几个
		int cornerLeftDownLength = 0;
		//算左下连了几个
		for (size_t site = 1; cols - site >= 0 && rows + site < SIZE && aRecordBoard[rows + site][cols - site] == preColer; site++)
		{
			cornerLeftDownLength++;
		}
		//判断左边头上是死是活
		aroundInfo[2].oppNum = 0;
		if (cols - cornerLeftDownLength - 1 < 0 || rows + cornerLeftDownLength + 1 >= SIZE || aRecordBoard[rows][cols - cornerLeftDownLength - 1] == 3 - preColer)
		{
			aroundInfo[2].oppNum++;
		}
		int cornerRightUpLength = 0;
		//算右上连了几个
		for (size_t site = 1; cols + site < SIZE && rows - site >= 0 && aRecordBoard[rows - site][cols + site] == preColer; site++)
		{
			cornerLeftDownLength++;
			cornerRightUpLength++;
		}
		//判断右边是死是活
		if (cols + cornerRightUpLength + 1 >= SIZE || rows - cornerRightUpLength - 1 < 0 || aRecordBoard[rows][cols + cornerLeftDownLength + 1] == 3 - preColer)
		{
			aroundInfo[2].oppNum++;
		}
		cornerLeftDownLength++;              //算上这个点本身
		aroundInfo[2].linkNum = cornerLeftDownLength;
		//然后计算左上右下连了几个
		int cornerLeftUpLength = 0;
		//算左上连了几个
		for (size_t site = 1; cols - site >= 0 && rows - site >= 0 && aRecordBoard[rows - site][cols - site] == preColer; site++)
		{
			cornerLeftUpLength++;
		}
		//判断左上头上是死是活
		aroundInfo[3].oppNum = 0;
		if (cols - cornerLeftUpLength - 1 < 0 || rows - cornerLeftUpLength - 1 < 0 || aRecordBoard[rows - cornerLeftUpLength - 1][cols - cornerLeftUpLength - 1] == 3 - preColer)
		{
			aroundInfo[3].oppNum++;
		}
		int cornerRightDownLength = 0;
		//算右下连了几个
		for (size_t site = 1; cols + site < SIZE && rows + site < SIZE && aRecordBoard[rows + site][cols + site] == preColer; site++)
		{
			cornerLeftUpLength++;
			cornerRightDownLength++;
		}
		//判断右下头上是死是活
		if (cols + cornerRightDownLength + 1 >= SIZE || rows + cornerRightDownLength + 1 >= SIZE || aRecordBoard[rows + cornerRightDownLength + 1][cols + cornerRightDownLength + 1] == 3 - preColer)
		{
			aroundInfo[3].oppNum++;
		}
		cornerLeftUpLength++;              //算上这个点本身
		aroundInfo[3].linkNum = cornerLeftDownLength;
		int live[6] = { 0, 0, 0, 0, 0, 0 }, half[6] = { 0, 0, 0, 0, 0, 0 }, dead[6] = { 0, 0, 0, 0, 0, 0 };          //计算死、半、活的各个长度都有多少个，为了让后面的判断好看所以搞了长度为6
		//先计算有多少活
		for (size_t liveLinkLength = 0; liveLinkLength < 6; liveLinkLength++)
		{
			for (size_t directionOfGo = 0; directionOfGo < 4; directionOfGo++)
			{
				if (aroundInfo[directionOfGo].oppNum == 0 && aroundInfo[directionOfGo].linkNum == liveLinkLength)
				{
					live[liveLinkLength]++;
				}
			}
		}
		//计算有多少半
		for (size_t halfLinkLength = 0; halfLinkLength < 6; halfLinkLength++)
		{
			for (size_t directionOfGo = 0; directionOfGo < 4; directionOfGo++)
			{
				if (aroundInfo[directionOfGo].oppNum == 1 && aroundInfo[directionOfGo].linkNum == halfLinkLength)
				{
					half[halfLinkLength]++;
				}
			}
		}
		//计算有多少死
		for (size_t deadLinkLength = 0; deadLinkLength < 6; deadLinkLength++)
		{
			for (size_t directionOfGo = 0; directionOfGo < 4; directionOfGo++)
			{
				if (aroundInfo[directionOfGo].oppNum == 2 && aroundInfo[directionOfGo].linkNum == deadLinkLength)
				{
					dead[deadLinkLength]++;
				}
			}
		}
		//有活五直接绝杀比赛
		if (live[5] >= 1 || half[5] >= 1 || dead[5] >= 1)
		{
			myscore = 1000;
		}
		else if (live[4] >= 1)
		{
			myscore = 500;
		}
		else if (half[4] >= 1 && live[3] >= 1)
		{
			myscore = 200;
		}
		else if ((half[4] >= 1 || live[3] >= 1) && (half[3] >= 1 || live[2] >= 1))
		{
			myscore = 175;
		}
		else if ((half[4] >= 1 || live[3] >= 1) && half[2] >= 1)
		{
			myscore = 150;
		}
		else if (half[4] >= 1 || live[3] >= 1)
		{
			myscore = 125;
		}
		else if ((half[3] >= 1 || live[2] >= 1) && dead[2] >= 1)
		{
			myscore = 70;
		}
		else if (half[3] >= 1 || live[2] >= 1)
		{
			myscore = 65;
		}
		else if (dead[2] >= 1)
		{
			myscore = 60;
		}
		else
		{
			myscore = 55;
		}
		oppScore = oppSocreGenerator(rows, cols, preColer);
	}
	else
	{
	myscore = -10001;
	oppScore = 0;
	}
	return myscore + oppScore;
}

void generateNextStep(void)
{
}

void topScore(int top)
{

	for (size_t numberInTop = 0; numberInTop < TOP; numberInTop++)
	{
		int maxScore = -10010;
		for (int siteInsocreNextStep0 = 0; siteInsocreNextStep0 < SIZE; siteInsocreNextStep0++)
		{
			for (size_t siteInsocreNextStep1 = 0; siteInsocreNextStep1 < SIZE; siteInsocreNextStep1++)
			{
				if (scoreNextStep[siteInsocreNextStep0][siteInsocreNextStep1] > maxScore && numberInTop == 0)
				{
					maxScore = scoreNextStep[siteInsocreNextStep0][siteInsocreNextStep1];
					topChoices[numberInTop][0] = siteInsocreNextStep0;
					topChoices[numberInTop][1] = siteInsocreNextStep1;
				}
				else if (scoreNextStep[siteInsocreNextStep0][siteInsocreNextStep1] > maxScore && scoreNextStep[siteInsocreNextStep0][siteInsocreNextStep1] <= topGetScore[numberInTop - 1] && (topChoices[numberInTop][0] != siteInsocreNextStep0 || topChoices[numberInTop][1] != siteInsocreNextStep1))
				{
					maxScore = scoreNextStep[siteInsocreNextStep0][siteInsocreNextStep1];
					topChoices[numberInTop][0] = siteInsocreNextStep0;
					topChoices[numberInTop][1] = siteInsocreNextStep1;
				}
			}
		}
		topGetScore[numberInTop] = maxScore;
	}
}

int oppSocreGenerator(int rows, int cols, int preColer1)
{
	int myscore, oppScore;
	int preColer = 3 - preColer1;
	struct SingleInfo aroundInfo[4];
	if (aRecordBoard[rows][cols] == 0)
	{
		//首先计算左右连了几个
		int leftrightlength = 0;
		//算左边练了几个
		for (size_t site = 1; cols - site >= 0 && aRecordBoard[rows][cols - site] == preColer; site++)
		{
			leftrightlength++;
		}
		//判断左边头上是死是活
		aroundInfo[0].oppNum = 0;
		if (cols - leftrightlength - 1 < 0 || aRecordBoard[rows][cols - leftrightlength - 1] == 3 - preColer)
		{
			aroundInfo[0].oppNum++;
		}
		int rightLength = 0;
		for (size_t site = 1; cols + site < SIZE && aRecordBoard[rows][cols + site] == preColer; site++)
		{
			leftrightlength++;
			rightLength++;
		}
		//判断右边是死是活
		if (cols + rightLength + 1 >= SIZE || aRecordBoard[rows][cols + rightLength + 1] == 3 - preColer)
		{
			aroundInfo[0].oppNum++;
		}
		leftrightlength++;              //算上这个点本身
		aroundInfo[0].linkNum = leftrightlength;
		//然后计算上下连了几个
		int upDownLength = 0;
		//算上边连了几个
		for (size_t site = 1; rows - site >= 0 && aRecordBoard[rows - site][cols] == preColer; site++)
		{
			upDownLength++;
		}
		//判断上边头上是死是活
		aroundInfo[1].oppNum = 0;
		if (rows - upDownLength - 1 < 0 || aRecordBoard[rows - upDownLength - 1][cols] == 3 - preColer)
		{
			aroundInfo[1].oppNum++;
		}
		int downLength = 0;
		//算下边连了几个
		for (size_t site = 1; rows + site < SIZE && aRecordBoard[rows + site][cols] == preColer; site++)
		{
			upDownLength++;
			downLength++;
		}
		//判断下边是死是活
		if (rows + downLength + 1 >= SIZE || aRecordBoard[rows + downLength + 1][cols] == 3 - preColer)
		{
			aroundInfo[1].oppNum++;
		}
		upDownLength++;              //算上这个点本身
		aroundInfo[1].linkNum = upDownLength;
		//然后计算左下右上连了几个
		int cornerLeftDownLength = 0;
		//算左下连了几个
		for (size_t site = 1; cols - site >= 0 && rows + site < SIZE && aRecordBoard[rows + site][cols - site] == preColer; site++)
		{
			cornerLeftDownLength++;
		}
		//判断左边头上是死是活
		aroundInfo[2].oppNum = 0;
		if (cols - cornerLeftDownLength - 1 < 0 || rows + cornerLeftDownLength + 1 >= SIZE || aRecordBoard[rows][cols - cornerLeftDownLength - 1] == 3 - preColer)
		{
			aroundInfo[2].oppNum++;
		}
		int cornerRightUpLength = 0;
		//算右上连了几个
		for (size_t site = 1; cols + site < SIZE && rows - site >= 0 && aRecordBoard[rows - site][cols + site] == preColer; site++)
		{
			cornerLeftDownLength++;
			cornerRightUpLength++;
		}
		//判断右边是死是活
		if (cols + cornerRightUpLength + 1 >= SIZE || rows - cornerRightUpLength - 1 < 0 || aRecordBoard[rows][cols + cornerLeftDownLength + 1] == 3 - preColer)
		{
			aroundInfo[2].oppNum++;
		}
		cornerLeftDownLength++;              //算上这个点本身
		aroundInfo[2].linkNum = cornerLeftDownLength;
		//然后计算左上右下连了几个
		int cornerLeftUpLength = 0;
		//算左上连了几个
		for (size_t site = 1; cols - site >= 0 && rows - site >= 0 && aRecordBoard[rows - site][cols - site] == preColer; site++)
		{
			cornerLeftUpLength++;
		}
		//判断左上头上是死是活
		aroundInfo[3].oppNum = 0;
		if (cols - cornerLeftUpLength - 1 < 0 || rows - cornerLeftUpLength - 1 < 0 || aRecordBoard[rows - cornerLeftUpLength - 1][cols - cornerLeftUpLength - 1] == 3 - preColer)
		{
			aroundInfo[3].oppNum++;
		}
		int cornerRightDownLength = 0;
		//算右下连了几个
		for (size_t site = 1; cols + site < SIZE && rows + site < SIZE && aRecordBoard[rows + site][cols + site] == preColer; site++)
		{
			cornerLeftUpLength++;
			cornerRightDownLength++;
		}
		//判断右下头上是死是活
		if (cols + cornerRightDownLength + 1 >= SIZE || rows + cornerRightDownLength + 1 >= SIZE || aRecordBoard[rows + cornerRightDownLength + 1][cols + cornerRightDownLength + 1] == 3 - preColer)
		{
			aroundInfo[3].oppNum++;
		}
		cornerLeftUpLength++;              //算上这个点本身
		aroundInfo[3].linkNum = cornerLeftUpLength;
	}
	else
	{
		myscore = -10001;
	}
	int live[6] = { 0, 0, 0, 0, 0, 0}, half[6] = { 0, 0, 0, 0, 0, 0}, dead[6]= { 0, 0, 0, 0, 0, 0};          //计算死、半、活的各个长度都有多少个
	//先计算有多少活
	for (size_t liveLinkLength = 0; liveLinkLength < 6; liveLinkLength++)
	{
		for (size_t directionOfGo = 0; directionOfGo < 4; directionOfGo++)
		{
			if (aroundInfo[directionOfGo].oppNum == 0 && aroundInfo[directionOfGo].linkNum == liveLinkLength)
			{
				live[liveLinkLength]++;
			}
		}
	}
	for (size_t halfLinkLength = 0; halfLinkLength < 6; halfLinkLength++)
	{
		for (size_t directionOfGo = 0; directionOfGo < 4; directionOfGo++)
		{
			if (aroundInfo[directionOfGo].oppNum == 1 && aroundInfo[directionOfGo].linkNum == halfLinkLength)
			{
				half[halfLinkLength]++;
			}
		}
	}
	for (size_t deadLinkLength = 0; deadLinkLength < 6; deadLinkLength++)
	{
		for (size_t directionOfGo = 0; directionOfGo < 4; directionOfGo++)
		{
			if (aroundInfo[directionOfGo].oppNum == 2 && aroundInfo[directionOfGo].linkNum == deadLinkLength)
			{
				dead[deadLinkLength]++;
			}
		}
	}
	//有活五直接绝杀比赛
	if (live[5] >= 1 || half[5] >= 1 || dead[5] >= 1)
	{
		myscore = 2000;
	}
	else if (live[4] >= 1)
	{
		myscore = 900;
	}
	else if (half[4] >= 1 && live[3] >= 1)
	{
		myscore = 700;
	}
	else if ((half[4] >= 1 || live[3] >= 1) && (half[3] >= 1 || live[2] >= 1))
	{
		myscore = 675;
	}
	else if ((half[4] >= 1 || live[3] >= 1) && half[2] >= 1)
	{
		myscore = 650;
	}
	else if (half[4] >= 1 || live[3] >= 1)
	{
		myscore = 625;
	}
	else if ((half[3] >= 1 || live[2] >= 1) && dead[2] >= 1)
	{
		myscore = 73;
	}
	else if (half[3] >= 1 || live[2] >= 1)
	{
		myscore = 68;
	}
	else if (dead[2] >= 1)
	{
		myscore = 63;
	}
	else
	{
		myscore = 58;
	}
	return myscore;
}

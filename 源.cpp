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
void nextStepGenerator(void);                                  //������һ����Ҫʲô�ģ��㷨����
int winnerJudger(void);                                        //�ж�ʤ����
void changeBoard(struct NextStep,int);                         //��nextstep�Ķ���д��������
int numberInARowRight(int color, int rows, int cols);          //�жϱ��������м�����ס
int numberInARowDown(int color, int rows, int cols);           //�жϱ��������м�����ס
int numberInARowCornerLeft(int color, int rows, int cols);     //�жϱ����������м�����ס
int numberInARowCornerRight(int color, int rows, int cols);    //�жϱ����������м�����ס
int scoreGenerator(int rows, int cols, int preColer);          //����һ����ķ���
void generateNextStep(void);                                   //������
void topScore(int top);                                        //��ǰtop������浽һ����������
int oppSocreGenerator(int rows, int cols, int preColer);                                   //����Է�������


//����ʹ�õ���GBK���룬ÿһ�������ַ�ռ��2���ֽڡ�

//���̻���ģ�� 
char aInitDisplayBoardArray[SIZE][SIZE * CHARSIZE + 1] =
{
		"���өөөөөөөөөөөөө�",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"���۩۩۩۩۩۩۩۩۩۩۩۩۩�"
};
//������������ʾ���� 
char aDisplayBoardArray[SIZE][SIZE * CHARSIZE + 1];

char play1Pic[] = "��";//������;
char play1CurrentPic[] = "��";

char play2Pic[] = "��";//������;
char play2CurrentPic[] = "��";


//���������ڼ�¼���̸�� 
int aRecordBoard[SIZE][SIZE];

//��¼���������Ȼ��ǻ���
int AIfirstHandMode;

//��¼��һ�����ĸ���ɫ����
int goCloro = 1;
//��������λ�õĽṹ�����ְ�
struct NextStep
{
	char col;
	int row;
};

struct NextStep AINextStep; //��¼AI����λ�õĽṹ
struct NextStep HumNextStep;  //��¼������λ�õĽṹ

//���ܵ����̸��
int aPossibleBoard[SIZE][SIZE];

//��¼������Ϣ�Ľṹ
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
	initRecordBorard();  //��ʼ�����̸��
	printf("����������0������������1��");
	scanf_s
	
	
	
	("%d", &AIfirstHandMode);
	recordtoDisplayArray();
	displayBoard();
	while (getchar() != '\n')
	{
		;
	}
	if (AIfirstHandMode == AIFIRST)                                        //����������ߣ���ֱ�ӿ�ʼһ������
	{
		nextStepGenerator();
		recordtoDisplayArray();
		goCloro = 3 - goCloro;
		displayBoard();
		printf("AI������%c %d\n", AINextStep.col, AINextStep.row);
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
		printf("AI������%c %d\n", AINextStep.col, AINextStep.row);
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

//��ʼ�����̸�� 
void initRecordBorard(void) {
	//ͨ��˫��ѭ������aRecordBoard��0
	for (size_t i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			aRecordBoard[i][j] = 0;
		}
	}
}

//��aRecordBoard�м�¼������λ�ã�ת����aDisplayBoardArray��
void recordtoDisplayArray(void) {
	//��һ������aInitDisplayBoardArray�м�¼�Ŀ����̣����Ƶ�aDisplayBoardArray��
	for (size_t i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE * CHARSIZE + 1; j++)
		{
			aDisplayBoardArray[i][j] = aInitDisplayBoardArray[i][j];
		}
	}
	//�ڶ�����ɨ��aRecordBoard����������0��Ԫ�أ�������ߡ��Ƶ�aDisplayBoardArray����Ӧλ����
	//ע�⣺aDisplayBoardArray����¼���ַ��������ַ���ÿ���ַ�ռ2���ֽڡ���͡�Ҳ�������ַ���ÿ��Ҳռ2���ֽڡ�
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


//��ʾ���̸�� 
void displayBoard(void) {
	int i;
	//��һ��������
	system("cls");   //����  
	//�ڶ�������aDisplayBoardArray�������Ļ��
	for (size_t i = 0; i < SIZE; i++)
	{
		printf("%s\n", aDisplayBoardArray[i]);
	}
	//������������������һ����ĸA B .... 
	for (size_t i = 0; i < SIZE; i++)
	{
		printf("%c ", 'A' + i);
	}
	printf("\n");
}

void nextStepGenerator(void)
{
	int preColer = goCloro;
	//��ʼ���÷�����
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
	//�������ѡ����ģ��һ��
	/*for (size_t top = 0; top < TOP; top++)
	{
		for (size_t j = 0; j < SIZE; j++)
		{
			for (size_t k = 0; k < SIZE; k++)
			{
				aPossibleBoard[j][k] = aRecordBoard[j][k];
			}
		}
		//����ѡ����һ���½�ȥ

		aPossibleBoard[topChoices[top][0]][topChoices[top][1]] = goCloro;
		int preColer = 3 - goCloro;
		int scoreNextStep[SIZE][SIZE];
		//����ͬ�ķ�ʽ�����ִ��
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
		int topChoices[TOP][2];   //��¼��õ�top��ѡ��
		int topGetScore[TOP];     //�����topѡ����µ÷�
		topScore(TOP);            //���������λ�ô�scorenextstep��ѡ�������浽topchoice��
		int bestChoice[2];
		int maxScore = -10000;
		for (size_t i = 0; i < SIZE; i++)               //ѡ����ߵ÷�
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
	//�����������ѡ����͵ģ���Ϊ�������������ڼ�¼���ǶԷ��ĵ÷�
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
	//�õ����յĽ��
	aRecordBoard[topChoices[0][0]][topChoices[0][1]] = goCloro;
	AINextStep.row = topChoices[0][0] + 1;
	AINextStep.col = 'a' + topChoices[0][1];
}

int winnerJudger(void)
{
	int winner = 0;
	//�ĸ�ѭ��,��һ�������ŵ�
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
	//�ڶ��������ŵ�
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
	//�����������µ�
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
		//���ȼ����������˼���
		int leftrightlength = 0;
		//��������˼���
		for (size_t site = 1; cols - site >= 0 && aRecordBoard[rows][cols - site] == preColer; site++)
		{
			leftrightlength++;
		}
		//�ж����ͷ�������ǻ�
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
		//�ж��ұ������ǻ�
		if (cols + rightLength + 1 >= SIZE || aRecordBoard[rows][cols + rightLength + 1] == 3 - preColer)
		{
			aroundInfo[0].oppNum++;
		}
		leftrightlength++;              //��������㱾��
		aroundInfo[0].linkNum = leftrightlength;
		//Ȼ������������˼���
		int upDownLength = 0;
		//���ϱ����˼���
		for (size_t site = 1; rows - site >= 0 && aRecordBoard[rows - site][cols] == preColer; site++)
		{
			upDownLength++;
		}
		//�ж��ϱ�ͷ�������ǻ�
		aroundInfo[1].oppNum = 0;
		if (rows - upDownLength - 1 < 0 || aRecordBoard[rows - upDownLength - 1][cols] == 3 - preColer)
		{
			aroundInfo[1].oppNum++;
		}
		int downLength = 0;
		//���±����˼���
		for (size_t site = 1; rows + site < SIZE && aRecordBoard[rows + site][cols] == preColer; site++)
		{
			upDownLength++;
			downLength++;
		}
		//�ж��±������ǻ�
		if (rows + downLength + 1 >= SIZE || aRecordBoard[rows + downLength + 1][cols] == 3 - preColer)
		{
			aroundInfo[1].oppNum++;
		}
		upDownLength++;              //��������㱾��
		aroundInfo[1].linkNum = upDownLength;
		//Ȼ����������������˼���
		int cornerLeftDownLength = 0;
		//���������˼���
		for (size_t site = 1; cols - site >= 0 && rows + site < SIZE && aRecordBoard[rows + site][cols - site] == preColer; site++)
		{
			cornerLeftDownLength++;
		}
		//�ж����ͷ�������ǻ�
		aroundInfo[2].oppNum = 0;
		if (cols - cornerLeftDownLength - 1 < 0 || rows + cornerLeftDownLength + 1 >= SIZE || aRecordBoard[rows][cols - cornerLeftDownLength - 1] == 3 - preColer)
		{
			aroundInfo[2].oppNum++;
		}
		int cornerRightUpLength = 0;
		//���������˼���
		for (size_t site = 1; cols + site < SIZE && rows - site >= 0 && aRecordBoard[rows - site][cols + site] == preColer; site++)
		{
			cornerLeftDownLength++;
			cornerRightUpLength++;
		}
		//�ж��ұ������ǻ�
		if (cols + cornerRightUpLength + 1 >= SIZE || rows - cornerRightUpLength - 1 < 0 || aRecordBoard[rows][cols + cornerLeftDownLength + 1] == 3 - preColer)
		{
			aroundInfo[2].oppNum++;
		}
		cornerLeftDownLength++;              //��������㱾��
		aroundInfo[2].linkNum = cornerLeftDownLength;
		//Ȼ����������������˼���
		int cornerLeftUpLength = 0;
		//���������˼���
		for (size_t site = 1; cols - site >= 0 && rows - site >= 0 && aRecordBoard[rows - site][cols - site] == preColer; site++)
		{
			cornerLeftUpLength++;
		}
		//�ж�����ͷ�������ǻ�
		aroundInfo[3].oppNum = 0;
		if (cols - cornerLeftUpLength - 1 < 0 || rows - cornerLeftUpLength - 1 < 0 || aRecordBoard[rows - cornerLeftUpLength - 1][cols - cornerLeftUpLength - 1] == 3 - preColer)
		{
			aroundInfo[3].oppNum++;
		}
		int cornerRightDownLength = 0;
		//���������˼���
		for (size_t site = 1; cols + site < SIZE && rows + site < SIZE && aRecordBoard[rows + site][cols + site] == preColer; site++)
		{
			cornerLeftUpLength++;
			cornerRightDownLength++;
		}
		//�ж�����ͷ�������ǻ�
		if (cols + cornerRightDownLength + 1 >= SIZE || rows + cornerRightDownLength + 1 >= SIZE || aRecordBoard[rows + cornerRightDownLength + 1][cols + cornerRightDownLength + 1] == 3 - preColer)
		{
			aroundInfo[3].oppNum++;
		}
		cornerLeftUpLength++;              //��������㱾��
		aroundInfo[3].linkNum = cornerLeftDownLength;
		int live[6] = { 0, 0, 0, 0, 0, 0 }, half[6] = { 0, 0, 0, 0, 0, 0 }, dead[6] = { 0, 0, 0, 0, 0, 0 };          //���������롢��ĸ������ȶ��ж��ٸ���Ϊ���ú�����жϺÿ����Ը��˳���Ϊ6
		//�ȼ����ж��ٻ�
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
		//�����ж��ٰ�
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
		//�����ж�����
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
		//�л���ֱ�Ӿ�ɱ����
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
		//���ȼ����������˼���
		int leftrightlength = 0;
		//��������˼���
		for (size_t site = 1; cols - site >= 0 && aRecordBoard[rows][cols - site] == preColer; site++)
		{
			leftrightlength++;
		}
		//�ж����ͷ�������ǻ�
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
		//�ж��ұ������ǻ�
		if (cols + rightLength + 1 >= SIZE || aRecordBoard[rows][cols + rightLength + 1] == 3 - preColer)
		{
			aroundInfo[0].oppNum++;
		}
		leftrightlength++;              //��������㱾��
		aroundInfo[0].linkNum = leftrightlength;
		//Ȼ������������˼���
		int upDownLength = 0;
		//���ϱ����˼���
		for (size_t site = 1; rows - site >= 0 && aRecordBoard[rows - site][cols] == preColer; site++)
		{
			upDownLength++;
		}
		//�ж��ϱ�ͷ�������ǻ�
		aroundInfo[1].oppNum = 0;
		if (rows - upDownLength - 1 < 0 || aRecordBoard[rows - upDownLength - 1][cols] == 3 - preColer)
		{
			aroundInfo[1].oppNum++;
		}
		int downLength = 0;
		//���±����˼���
		for (size_t site = 1; rows + site < SIZE && aRecordBoard[rows + site][cols] == preColer; site++)
		{
			upDownLength++;
			downLength++;
		}
		//�ж��±������ǻ�
		if (rows + downLength + 1 >= SIZE || aRecordBoard[rows + downLength + 1][cols] == 3 - preColer)
		{
			aroundInfo[1].oppNum++;
		}
		upDownLength++;              //��������㱾��
		aroundInfo[1].linkNum = upDownLength;
		//Ȼ����������������˼���
		int cornerLeftDownLength = 0;
		//���������˼���
		for (size_t site = 1; cols - site >= 0 && rows + site < SIZE && aRecordBoard[rows + site][cols - site] == preColer; site++)
		{
			cornerLeftDownLength++;
		}
		//�ж����ͷ�������ǻ�
		aroundInfo[2].oppNum = 0;
		if (cols - cornerLeftDownLength - 1 < 0 || rows + cornerLeftDownLength + 1 >= SIZE || aRecordBoard[rows][cols - cornerLeftDownLength - 1] == 3 - preColer)
		{
			aroundInfo[2].oppNum++;
		}
		int cornerRightUpLength = 0;
		//���������˼���
		for (size_t site = 1; cols + site < SIZE && rows - site >= 0 && aRecordBoard[rows - site][cols + site] == preColer; site++)
		{
			cornerLeftDownLength++;
			cornerRightUpLength++;
		}
		//�ж��ұ������ǻ�
		if (cols + cornerRightUpLength + 1 >= SIZE || rows - cornerRightUpLength - 1 < 0 || aRecordBoard[rows][cols + cornerLeftDownLength + 1] == 3 - preColer)
		{
			aroundInfo[2].oppNum++;
		}
		cornerLeftDownLength++;              //��������㱾��
		aroundInfo[2].linkNum = cornerLeftDownLength;
		//Ȼ����������������˼���
		int cornerLeftUpLength = 0;
		//���������˼���
		for (size_t site = 1; cols - site >= 0 && rows - site >= 0 && aRecordBoard[rows - site][cols - site] == preColer; site++)
		{
			cornerLeftUpLength++;
		}
		//�ж�����ͷ�������ǻ�
		aroundInfo[3].oppNum = 0;
		if (cols - cornerLeftUpLength - 1 < 0 || rows - cornerLeftUpLength - 1 < 0 || aRecordBoard[rows - cornerLeftUpLength - 1][cols - cornerLeftUpLength - 1] == 3 - preColer)
		{
			aroundInfo[3].oppNum++;
		}
		int cornerRightDownLength = 0;
		//���������˼���
		for (size_t site = 1; cols + site < SIZE && rows + site < SIZE && aRecordBoard[rows + site][cols + site] == preColer; site++)
		{
			cornerLeftUpLength++;
			cornerRightDownLength++;
		}
		//�ж�����ͷ�������ǻ�
		if (cols + cornerRightDownLength + 1 >= SIZE || rows + cornerRightDownLength + 1 >= SIZE || aRecordBoard[rows + cornerRightDownLength + 1][cols + cornerRightDownLength + 1] == 3 - preColer)
		{
			aroundInfo[3].oppNum++;
		}
		cornerLeftUpLength++;              //��������㱾��
		aroundInfo[3].linkNum = cornerLeftUpLength;
	}
	else
	{
		myscore = -10001;
	}
	int live[6] = { 0, 0, 0, 0, 0, 0}, half[6] = { 0, 0, 0, 0, 0, 0}, dead[6]= { 0, 0, 0, 0, 0, 0};          //���������롢��ĸ������ȶ��ж��ٸ�
	//�ȼ����ж��ٻ�
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
	//�л���ֱ�Ӿ�ɱ����
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

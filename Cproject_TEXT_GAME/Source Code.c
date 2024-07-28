#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>//Ű���� �Է½� �Ѿ�� ����
#include <time.h>//����, ���� ������ ���� ����

#define RESET "\033[0m"//ü�¹�, ����ȿ���� �� �����
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

#define nameLen 30
#define save "SavedGame.txt"
#define enemydata "EnemyData.txt"
#define weapondata "WeaponData.txt"
#define armordata "ArmorData.txt"
#define bossdata "BossData.txt"
#define weaponcount 9
#define armorcount 9

typedef struct {
	char weaponName[30];
	int weaponATK, weaponDEF, weaponHP, weaponMP;
	char weaponskill[300];
	char weaponInfo[300];
}_Weapon;

typedef struct {
	char armorName[30];
	int armorATK, armorDEF, armorHP, armorMP;
	char armorskill[300];
	char armorInfo[300];
}_Armor;

typedef struct {
	char itemName[30];
	int quantity;
}_Item;//�κ��丮�� �� ������ ����ü

typedef  struct {//�÷��̾� ���� ����ü
	char charname[nameLen];
	int HP;
	int MP;
	int currentHP;
	int currentMP;
	int ATK;
	int DEF;
	_Weapon* weapon;//����ü �����͸� �Ἥ ���⳪ ������ �����ϴ� �ɷ�ġ�� ���ؼ��� ������
	_Armor* armor;
	_Item* Inventory[8];//8ĭ¥�� �κ��丮 ����
	int invenCount;
	int stage;
	char room[10];
	int gold;
	int battleCount;

	int storeItem1;
	int storeItem2;
	int storeItem3;
	int storeItem4;
	//���� ������-��״� �������� ������ �ʱ�ȭ�Ǿ ���������� ������

}Charstats;
typedef struct {//�� ���� ����ü
	char enemyname[nameLen];
	int HP;
	int currentHP;
	int ATK;
	int DEF;
	char enemyinfo[100];

}Enemystats;

void saveGame(Charstats* player);//���̺�
void loadGame(Charstats* player, Enemystats* enemy);//�̾��ϱ�
void startingRoom(Charstats* player, Enemystats* enemy);//���� ��
void battleRoom(Charstats* player, Enemystats* enemy);//�Ϲ� ���� ��
void preparingRoom(Charstats* player, Enemystats* enemy);//�غ� ��
void store(Charstats* player);//�غ��-����
void crafting(Charstats* player);//�غ��-��� ����
void bossRoom(Charstats* player, Enemystats* enemy);//���� ���� ��
void End(Charstats* player);//����
void newGame(Charstats* player, Enemystats* enemy);//�� ���� ����
void HowTo();//����ȭ��-Ʃ�丮��
void startMenu(Charstats* player, Enemystats* enemy);//����ȭ��
void judge(Charstats* player, Enemystats* enemy);//���� �� ���� ����
void bossjudge(Charstats* player, Enemystats* enemy);//���� ���� �� ���� ����
void healthBar(int CHP, int MHP, int shield);//ü�¹� ���
void ManaBar(int CMP, int MMP);//������ ���
void GameOver(Charstats* player);//���ӿ���
void addItem(Charstats* player, char name[30], int count);//�κ��丮�� ������ �߰�
void useItem(Charstats* player, char name[30], int count);//�κ��丮 ���� ������ ���
void readEquipments(Charstats* player);//��� ������ ���Ͽ��� �о����
int showItemCount(Charstats* player, char name[30]);//�κ��丮�� ������ �����۰� ���� �������� �ִ��� ã�� �Լ�


int battle = 0;//���� ������ �ƴ��� ���θ� ��Ÿ���� ��������
//��� ������ �����߿��� �Ұ���, ��� ���� ���� ����� �����߿��� ����
int GAMEOVER = 0;//��� Ƚ���� ��Ÿ���� ���� ����

int main(void) {
	Charstats player;
	Enemystats enemy;
	player.weapon = (_Weapon*)malloc(sizeof(_Weapon));
	player.armor = (_Armor*)malloc(sizeof(_Armor));
	while (1) {
		startMenu(&player, &enemy);
	}
	free(player.weapon);
	free(player.armor);
	return 0;
}

void saveGame(Charstats* player) {//����
	FILE* file = fopen(save, "w");
	if (file == NULL) {
		printf("������ �� �� ������ �߻��߽��ϴ�.\n");
		return;
	}

	fprintf(file, "%s\n", player->charname);//�̸�
	fprintf(file, "%d\n", player->HP);//�ִ� ü��
	fprintf(file, "%d\n", player->MP);//�ִ� ����
	fprintf(file, "%d\n", player->currentHP);//���� ü��
	fprintf(file, "%d\n", player->currentMP);//���� ����
	fprintf(file, "%d\n", player->ATK);//���ݷ�
	fprintf(file, "%d\n", player->DEF);//����

	fprintf(file, "%s\n", player->weapon->weaponName);//���� �̸�(��� ������ ���� �̸��� ���� �ҷ��� ���̱� ����)
	fprintf(file, "%s\n", player->armor->armorName);//�� �̸�

	fprintf(file, "%d\n", player->stage);//��������
	fprintf(file, "%s\n", player->room);//�� ����
	fprintf(file, "%d\n", player->gold);//���
	fprintf(file, "%d\n", player->battleCount);//������������ ������ ������ ��
	fprintf(file, "%d\n", player->storeItem1);//���� ���ǵ� ����
	fprintf(file, "%d\n", player->storeItem2);//���� ���ǵ� ����
	fprintf(file, "%d\n", player->storeItem3);//���� ���ǵ� ����
	fprintf(file, "%d\n", player->storeItem4);//���� ���ǵ� ����
	fprintf(file, "%d\n\n", player->invenCount);//������ �������� ��
	for (int i = 0;i < player->invenCount; i++) {
		fprintf(file, "%d %s\n", player->Inventory[i]->quantity, player->Inventory[i]->itemName);//�κ��丮�� �ִ� �����۵�
	}
	printf("���� �����.\n");
	fclose(file);

}

void loadGame(Charstats* player, Enemystats* enemy) {//�ε�
	FILE* file = fopen(save, "r");
	if (file == NULL) {
		printf("����� ������ �����ϴ�.\n");
		return;
	}
	fgets(player->charname, nameLen, file);//���� ���ڵ��� ã�� ���ּ� �����͸� �о���� �� �����Ϳ� ���๮�ڰ� ���ԵǴ� ��� ����
	player->charname[strcspn(player->charname, "\n")] = '\0';//�̸��� ���Ⱑ ���Ե� ���� �־� ĳ���ͳ� ����̸� �о�� ���� fgets�� �����

	fscanf(file, "%d\n", &player->HP);
	fscanf(file, "%d\n", &player->MP);
	fscanf(file, "%d\n", &player->currentHP);
	fscanf(file, "%d\n", &player->currentMP);
	fscanf(file, "%d\n", &player->ATK);
	fscanf(file, "%d\n", &player->DEF);

	fgets(player->weapon->weaponName, nameLen, file);//���� ���ڵ��� ã�� ���ּ� �����͸� �о���� �� �����Ϳ� ���๮�ڰ� ���ԵǴ� ��� ����
	player->weapon->weaponName[strcspn(player->weapon->weaponName, "\n")] = '\0';
	fgets(player->armor->armorName, nameLen, file);//���� ���ڵ��� ã�� ���ּ� �����͸� �о���� �� �����Ϳ� ���๮�ڰ� ���ԵǴ� ��� ����
	player->armor->armorName[strcspn(player->armor->armorName, "\n")] = '\0';


	fscanf(file, "%d\n", &player->stage);
	fscanf(file, "%s\n", player->room);
	fscanf(file, "%d\n", &player->gold);
	fscanf(file, "%d\n", &player->battleCount);
	fscanf(file, "%d\n", &player->storeItem1);
	fscanf(file, "%d\n", &player->storeItem2);
	fscanf(file, "%d\n", &player->storeItem3);
	fscanf(file, "%d\n", &player->storeItem4);
	fscanf(file, "%d\n\n", &player->invenCount);

	for (int i = 0;i < player->invenCount; i++) {
		player->Inventory[i] = (_Item*)malloc(sizeof(_Item) * 30);
		fscanf(file, "%d\n", &player->Inventory[i]->quantity);
		fgets(player->Inventory[i]->itemName, nameLen, file);//���� ���ڵ��� ã�� ���ּ� �����͸� �о���� �� �����Ϳ� ���๮�ڰ� ���ԵǴ� ��� ����
		player->Inventory[i]->itemName[strcspn(player->Inventory[i]->itemName, "\n")] = '\0';
	}
	printf("���������� �ҷ��Խ��ϴ�.\n");
	if (GAMEOVER == 1) {
		printf("...!\n");
		_getch();
		printf("����� �ٽ� ���� ����.\n");
		_getch();
		printf("������ �Ǿ��� ���� ��ü�� ã�Ҵ�.\n");
		_getch();
		printf("����� �κ��丮�� ����ô�. �κ��丮�� �� ���� �װ� �״�ο���.\n");
		_getch();
		printf("��������, ����� ����� �ױ� �� �� ������ ���ƿԴٴ� ���� ���޾Ҵ�.\n");
		_getch();
		printf("����� ������ ���Ǹ� ������.\n");
		printf("%s����� HP�� MP�� �ִ�� ȸ���Ǿ���!%s", GREEN, RESET);
		player->currentHP = player->HP+player->weapon->weaponHP+player->armor->armorHP;
		player->currentMP = player->MP+player->weapon->weaponMP+player->armor->armorMP;
	}
	fclose(file);
	if (!strcmp(player->room, "starting")) {
		startingRoom(player, enemy);
	}
	else if (!strcmp(player->room, "fighting")) {
		battleRoom(player, enemy);
	}
	else if (!strcmp(player->room, "preparing")) {
		preparingRoom(player, enemy);
	}
	else if (!strcmp(player->room, "boss")) {
		bossRoom(player, enemy);
	}else {
		return;
	}

}

void HowTo() {//�÷��� ���
	printf("---------------------------------\n");
	printf("������: ������б� �κ��к� 24�й� ������\n");
	printf("\n");
	printf("Ű���带 ������ ���� �ؽ�Ʈ�� �Ѿ �� �ֽ��ϴ�.\n");
	_getch();
	printf("����� �������� � ������ ���� �˴ϴ�.\n");
	_getch();
	printf("����� ������ ���� �ƹ��͵� �����ϴ�.\n");
	_getch();
	printf("������ ���� ���� ����ǰ���� �� ���� ��� �����ϼ���.\n");
	_getch();
	printf("��� ���� ���� �ɷ�ġ�� ��ų�� �ٸ��ϴ�.\n");
	_getch();
	printf("���������� �⺻������ 2���� AP(�ൿ ��ȸ)�� �־����ϴ�.\n");
	_getch();
	printf("���ݰ� ���� AP�� �Ҹ�������,\n");
	_getch();
	printf("������ ����̳� ���� ����� AP�� �Ҹ����� �ʽ��ϴ�.\n");
	_getch();
	printf("���� ������� ��뿡�Դ� 3���� �ൿ ��ȸ�� �־����ϴ�.\n");
	_getch();
	printf("����� ��ų�� ���������� ����� �¸��� �����ϼ���.\n");
	_getch();
	printf("---------------------------------\n");
	return;
}

void readEquipments(Charstats* player) {
	FILE* weapon = fopen(weapondata, "r");//��� ������ �о����
	if (weapon == NULL) {
		printf("������ �� �� ������ �߻��߽��ϴ�.\n");
		return;
	}

	char line1[weaponcount * 7];
	int found = 0;      // ���ڿ��� ã�Ҵ��� ���θ� ��Ÿ���� �÷���

	// ������ ���� �ϳ��� �����鼭 ó��
	while (fgets(line1, sizeof(line1), weapon)) {
		line1[strcspn(line1, "\n")] = '\0'; // ���� ���� ����
		//printf("%s", player->weapon->weaponName);
		// ���ڿ��� ã���� Ż��
		if (!found && strcmp(player->weapon->weaponName, line1) == 0) {
			found = 1; // ���ڿ��� ã������ ǥ��
			break;
		}
	}
	if (!found) {
		printf("���Ͽ��� '%s'�� ��ġ�ϴ� ���� ã�� �� �����ϴ�.\n", player->weapon->weaponName);
	}
	fscanf(weapon, "%d\n", &player->weapon->weaponHP);
	fscanf(weapon, "%d\n", &player->weapon->weaponATK);
	fscanf(weapon, "%d\n", &player->weapon->weaponDEF);
	fscanf(weapon, "%d\n", &player->weapon->weaponMP);
	fgets(player->weapon->weaponskill, 150, weapon);//���� ���ڵ��� ã�� ���ּ� �����͸� �о���� �� �����Ϳ� ���๮�ڰ� ���ԵǴ� ��� ����
	player->weapon->weaponskill[strcspn(player->weapon->weaponskill, "\n")] = '\0';
	fgets(player->weapon->weaponInfo, 100, weapon);//���� ���ڵ��� ã�� ���ּ� �����͸� �о���� �� �����Ϳ� ���๮�ڰ� ���ԵǴ� ��� ����
	player->weapon->weaponInfo[strcspn(player->weapon->weaponInfo, "\n")] = '\0';
	fclose(weapon);

	found = 0;
	FILE* armor = fopen(armordata, "r");//��� ������ �о����
	if (armor == NULL) {
		printf("������ �� �� ������ �߻��߽��ϴ�.\n");
		return;
	}

	while (fgets(line1, sizeof(line1), armor)) {
		line1[strcspn(line1, "\n")] = '\0'; // ���� ���� ����
		//printf("%s", player->armor->armorName);
		// ���ڿ��� ã���� Ż��
		if (!found && strcmp(player->armor->armorName, line1) == 0) {
			found = 1; // ���ڿ��� ã������ ǥ��
			break;
		}
	}
	if (!found) {
		printf("���Ͽ��� '%s'�� ��ġ�ϴ� ���� ã�� �� �����ϴ�.\n", player->armor->armorName);
	}
	fscanf(armor, "%d\n", &player->armor->armorHP);
	fscanf(armor, "%d\n", &player->armor->armorATK);
	fscanf(armor, "%d\n", &player->armor->armorDEF);
	fscanf(armor, "%d\n", &player->armor->armorMP);
	fgets(player->armor->armorskill, 150, armor);//���� ���ڵ��� ã�� ���ּ� �����͸� �о���� �� �����Ϳ� ���๮�ڰ� ���ԵǴ� ��� ����
	player->armor->armorskill[strcspn(player->armor->armorskill, "\n")] = '\0';
	fgets(player->armor->armorInfo, 100, armor);//���� ���ڵ��� ã�� ���ּ� �����͸� �о���� �� �����Ϳ� ���๮�ڰ� ���ԵǴ� ��� ����
	player->armor->armorInfo[strcspn(player->armor->armorInfo, "\n")] = '\0';
	fclose(armor);
	
}

void battleRoom(Charstats* player, Enemystats* enemy) {//������
	readEquipments(player);
	strcpy(player->room, "fighting");
	saveGame(player);
	battle = 1;
	int turn = 1;//���� ���� ���ο� �ϼ� ���
	int AP_player, AP_enemy, startline = 0;//���� ���� ü��, �ڽŰ� ����� ���� �ൿ�� ��Ÿ����
	char input[10];//�Է°� ����
	int ATKBonus[2] = { 0,0 }, DEFBonus[2] = { 0,0 }, Poison[2] = { 0,0 }, Burn[2] = { 0,0 }, Regen[2] = { 0,0 }, Will = 0, Fear = 0;//���� ȿ��: ���� ����, ��� ����, ��, ȭ��, ���, ����, ��ǥ
	//��: �� �� ����� ���� ���� �� ��ġ ����, ��ġ ��� ���� ���� | ȭ��: �� �ϸ��� ���� ���� �� ��ġ ����, ��ġ ��� ���ݷ� ����
	//���: �� �ϸ��� ��ġ ��� ȸ�� �� ��ġ ���� | ����: �ǰ� 1ȸ �������� ��ȿȭ�ϰ� ��ġ 1 ���� | ����: �� ���۽� �÷��̾��� �ൿ�� 1 ����
	//����, ������ �÷��̾� ���� Ư��ȿ���� �迭�� �������� ����
	//��� ������ ���� �÷��̾� �ִ� ü��, ���� ����
	int maxHP = player->HP + player->weapon->weaponHP + player->armor->armorHP;
	int maxMP = player->MP + player->weapon->weaponMP + player->armor->armorMP;
	int damage, Pshield, Eshield, Pcharge = 0, Pstack = 0, Echarge = 0, Estack = 0;//���� �� ����ϴ� ����
	int randomatk, atkarr[10] = { 0,0,0,0,0,0,1,1,2,2 };//���� �������� ������ �� �ְ� �ϴ� ����
	srand((unsigned int)time(NULL));//������ ���� ���
	//���������� ���� �ٸ� ���Ͱ� ����
	
	int encounter[] = { 0, 5, 10, 15 };

	if (player->stage == 1) {//���� ������ ������ Ư�� �ٺ��� ����ϴ� ������ ����
		startline = encounter[rand() % 4];//1���� ������ ���� 4��(���� �ϳ��� ������ 5���� ����)
	}
	else if (player->stage == 2) {
		startline = encounter[(rand() % 4)]+20;//2���� ������ ���� 4��
	}
	else if (player->stage == 3) {
		startline = encounter[(rand() % 4)]+40;//3���� ������ ���� 4��
	}
	
	/*
	if (player->stage == 1) {//���� ������ ������ Ư�� �ٺ��� ����ϴ� ������ ����
		startline = (rand() % 4)*5+1;//1��
	}
	else if (player->stage == 2) {
		startline = (rand() % 4)*5 + 20;//2��
	}
	else if (player->stage == 3) {
		startline = (rand() % 4)*5 + 40;//3��
	}
	*/
	FILE* monster = fopen(enemydata, "r");
	if (monster == NULL) {
		printf("���� �����Ͱ� �����ϴ�.\n");
		return;
	}
	
	char line[100];
	int currentline = 1;
	if (startline == 0) {

	}
	else {
		while (fgets(line, sizeof(line), monster)) {//������ ���س��� �ٺ��� ���� �����͸� �о�´�
			if (currentline == startline) {
				break;
			}
			else {
				line[0] = '\0';
				currentline++;
			}
		}
	}
	printf("%d %d\n", startline, currentline);
	fgets(enemy->enemyname, nameLen, monster);//���� ���ڵ��� ã�� ���ּ� �����͸� �о���� �� �����Ϳ� ���๮�ڰ� ���ԵǴ� ��� ����
	enemy->enemyname[strcspn(enemy->enemyname, "\n")] = '\0';
	fscanf(monster, "%d\n", &enemy->HP);
	fscanf(monster, "%d\n", &enemy->ATK);
	fscanf(monster, "%d\n", &enemy->DEF);
	fgets(enemy->enemyinfo, sizeof(enemy->enemyinfo), monster);//���� ���ڵ��� ã�� ���ּ� �����͸� �о���� �� �����Ϳ� ���๮�ڰ� ���ԵǴ� ��� ����
	enemy->enemyinfo[strcspn(enemy->enemyinfo, "\n")] = '\0';
	enemy->currentHP = enemy->HP;

	fclose(monster);

	_getch();
	printf("\n����� �ɻ�ġ ���� ����� ���� �濡 ����.\n");
	_getch();
	printf("�����߻�!\n");
	_getch();
	printf("---------------------------------\n");
	printf("%s�� ��Ÿ����!\n\n", enemy->enemyname);
	Eshield = 0;

	while (battle == 1) {
		input[0] = '\0';
		Pshield = 0;
		printf("\n----------------TURN %d--------------\n\n", turn);
		AP_enemy = 2;
		AP_player = 2;

		if (!strcmp(player->weapon->weaponName, "���� ���� �� ����")) {
			if (player->currentHP <= maxHP / 2) {
				printf("%s���� ���� �� ���� �нú�� %s�� ���� 1�� �����!%s\n", YELLOW, player->charname, RESET);
				Will++;
			}
		}
		printf("%s ", player->charname);
		healthBar(player->currentHP, maxHP, Pshield);
		printf("%s ", enemy->enemyname);
		healthBar(enemy->currentHP, enemy->HP, Eshield);

		while (AP_player > 0) {//�÷��̾� ��
			printf("\n%s�� ������ �ұ�?\n", player->charname);
			printf("AP : %s%d%s | MP ", YELLOW, AP_player, RESET);
			ManaBar(player->currentMP, maxMP);
			judge(player, enemy);
			printf("|����|���|������|����|");
			printf("---------------------------------\n�Է�: ");
			scanf("%s", input);
			//printf("\n%d %d %d %d\n", player->ATK, player->weapon->weaponATK, player->armor->armorATK, enemy->DEF);
			if (!strcmp(input, "����")) {
				damage = (player->ATK + player->weapon->weaponATK + player->armor->armorATK) * (1 + 0.1 * (ATKBonus[0] - Burn[0])) - (enemy->DEF * (1 + 0.1 * (DEFBonus[1] - Poison[1])));

				if (!strcmp(player->weapon->weaponName, "������ ������ �ҵ�")) {
					if (Pstack > 0) {
						printf("%s������ ������ �ҵ� �нú�� ���ط��� �����ߴ�!%s\n", YELLOW, RESET);
						damage *= 1.25 * Pstack;
						_getch();
						printf("%s�� ����! %d�� ���ظ� �־���!\n", player->charname, damage);
						printf("%s������ ������ �ҵ� �нú�� %s�� ���� %d�� ȸ���ߴ�!%s\n", YELLOW, player->charname, 5 * Pstack, RESET);
						player->currentMP += 5 * Pstack;
						if (player->currentMP > maxMP) {
							player->currentMP = maxMP;
						}
						Pstack = 0;
					}
					else {
						printf("%s�� ����! %d�� ���ظ� �־���!\n", player->charname, damage);
					}
				}
			else if (!strcmp(player->weapon->weaponName, "������ ������ �ҵ�")) {
					if (Pstack > 0) {
						printf("%s������ ������ �ҵ� �нú�� ���ط��� �����ߴ�!%s\n", YELLOW, RESET);
						damage *= 1.5 * Pstack;
						_getch();
						printf("%s�� ����! %d�� ���ظ� �־���!\n", player->charname, damage);
						Burn[1] += Pstack / 2 + 1;
						printf("%s������ ������ �ҵ� �нú�� %s�� ȭ��  %d�� �ɷȴ�!%s\n", RED, enemy->enemyname, (Pstack / 2)+1, RESET);
						printf("%s������ ������ �ҵ� �нú�� %s�� ���� %d�� ȸ���ߴ�!%s\n", YELLOW, player->charname, 7 * Pstack, RESET);
						player->currentMP += 7 * Pstack;
						if (player->currentMP > maxMP) {
							player->currentMP = maxMP;
						}
						Pstack = 0;
					}
					else {
						printf("%s�� ����! %d�� ���ظ� �־���!\n", player->charname, damage);
					}
				}
				if ((strcmp(player->weapon->weaponName, "������ ������ �ҵ�") || !strcmp(player->weapon->weaponName, "������ ������ �ҵ�")|| (strcmp(player->weapon->weaponName, "������ ������ �ҵ�"))==-1 || !strcmp(player->weapon->weaponName, "������ ������ �ҵ�")==-1))
				{

					printf("%s�� ����! %d�� ���ظ� �־���!\n", player->charname, damage);
				}

				Eshield -= damage;
				if (!strcmp(player->weapon->weaponName, "�� ���̽�")) {
					if ((player->currentHP < player->HP) && (player->currentMP >= 5)) {
						printf("%s�� ���̽� �нú�� %s�� �߰� ���� %d�� �޾Ҵ�!%s\n", RED, enemy->enemyname, (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0])), RESET);
						player->currentMP -= 5;
						Eshield -= (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0]));
					}
				}
				if (!strcmp(player->weapon->weaponName, "���� ���� �� ���̽�")) {
					if ((player->currentHP < player->HP) && (player->currentMP >= 5)) {
						printf("%s���� ���� �� ���̽� �нú�� %s�� �߰� ���� %d�� �޾Ҵ�!%s\n", RED, enemy->enemyname, (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0])), RESET);
						player->currentMP -= 5;
						Eshield -= (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0]));
						printf("%s���� ���� �� ���̽� �нú�� %s�� ȭ�� �ɷȴ�!%s\n", RED, enemy->enemyname, RESET);
						Burn[1]++;
					}
				}


				if (Eshield < 0) {
					enemy->currentHP += Eshield;
					Eshield = 0;
				}

				if (!strcmp(player->weapon->weaponName, "������ ��Ʈ")) {
					if (player->currentMP >= 5) {
						printf("%s������ ��Ʈ �нú�� %s�� ü���� ȸ���ߴ�!%s\n", GREEN, player->charname, RESET);
						player->currentMP -= 5;
						player->currentHP += 7;
						if (player->currentHP > maxHP) {
							player->currentHP = maxHP;
						}
					}
				}
				if (!strcmp(player->weapon->weaponName, "������ �ظ�")) {
					if (player->currentMP >= 5) {
						printf("%s������ �ظ� �нú�� %s�� ü���� ȸ���ߴ�!%s\n", GREEN, player->charname, RESET);
						player->currentMP -= 5;
						player->currentHP += 15;
						if (player->currentHP > maxHP) {
							player->currentHP = maxHP;
						}
						if (ATKBonus[0] < 2) {
							ATKBonus[0]++;
							printf("%s������ �ظ� �нú�� %s�� ���ݷ��� �����ߴ�!%s\n", RED, player->charname, RESET);
						}
					}
				}
				if (!strcmp(player->weapon->weaponName, "�ǽĿ� ��Į")) {
					if ((player->currentHP < maxHP) && (Poison[1] > 0)) {
						printf("%s�ǽĿ� ��Į �нú�� %s�� ü���� ȸ���ߴ�!%s\n", GREEN, player->charname, RESET);
						player->currentHP += 5;
						if (player->currentHP > maxHP) {
							player->currentHP = maxHP;
						}
					}
					if (player->currentMP >= 10) {
						printf("%s�ǽĿ� ��Į �нú�� %s�� ���� �ߵ��ƴ�!%s\n", RED, enemy->enemyname, RESET);
						player->currentMP -= 10;
						Poison[1] += 2;
					}
				}
				if (!strcmp(player->weapon->weaponName, "ô����")) {
					if (Poison[1] > 0) {
						if (player->currentHP < maxHP) {
							printf("%sô���� �нú�� %s�� ü���� ȸ���ߴ�!%s\n", GREEN, player->charname, RESET);
							player->currentHP += 15;
							if (player->currentHP > maxHP) {
								player->currentHP = maxHP;
							}
						}
						printf("%sô���� �нú�� %s�� ���� ���� 7�� �޾Ҵ�!%s\n", RED, enemy->enemyname, RESET);
						enemy->currentHP -= 7;
					}
					if (player->currentMP >= 10) {
						printf("%sô���� �нú�� %s�� ���� �ߵ��ƴ�!%s\n", RED, enemy->enemyname, RESET);
						player->currentMP -= 10;
						Poison[1] += 4;
					}
				}

				if (enemy->currentHP <= 0) {
					enemy->currentHP = 0;
				}
				printf("%s ", enemy->enemyname);
				healthBar(enemy->currentHP, enemy->HP, Eshield);
				judge(player, enemy);
				if (Estack > 0 && (!strcmp(enemy->enemyname, "���൹ ��") || !strcmp(enemy->enemyname, "���� ��") || !strcmp(enemy->enemyname, "���� ��"))) {
					printf("%s�� �ݻ� ���ظ� �޾Ҵ�!\n", player->charname);
					player->currentHP -= 5 * Estack;
					Estack -= player->stage;
				}
				//�߰�ȿ�� �ߵ�

				AP_player--;
			}
			else if (!strcmp(input, "���")) {
				Pshield += (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF);
				printf("%s�� ��� �ڼ��� ���ߴ�!\n", player->charname);
				printf("�̹� �� ���� %s%d%s�� ��ȣ���� �߰��� ��´�!\n", BLUE, (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF), RESET);

				if (!strcmp(player->weapon->weaponName, "������ ������ �ҵ�")) {
					if (player->currentMP >= 10) {
						printf("%s������ ������ �ҵ� �нú�� ���� 10�� �Ҹ��� ������ �����!%s\n", BLUE, RESET);
						player->currentMP -= 10;
						Pstack++;
					}
				}
				if (!strcmp(player->weapon->weaponName, "������ ������ �ҵ�")) {
					if (player->currentMP >= 10) {
						printf("%s������ ������ �ҵ� �нú�� ���� 10�� �Ҹ��� ������ �����!%s\n", BLUE, RESET);
						player->currentMP -= 10;
						Pstack++;
					}
				}
				if (!strcmp(player->weapon->weaponName, "��� ����")) {
					if (player->currentMP >= 5) {
						printf("%s��� ���� �нú�� ���� 5�� �Ҹ��� ��ȣ�� 10�� �߰��� �����!%s\n", BLUE, RESET);
						player->currentMP -= 5;
						Pshield += 10;
					}
				}
				if (!strcmp(player->weapon->weaponName, "��� ������ ����")) {
					if (player->currentMP >= 10) {
						printf("%s��� ������ ���� �нú�� ���� 10�� �Ҹ��� ��ȣ�� 20�� �߰��� �����!%s\n", BLUE, RESET);
						player->currentMP -= 10;
						Pshield += 20;
					}
				}

				//�߰�ȿ�� �ߵ�
				AP_player--;
			}
			else if (!strcmp(input, "������")) {
				//������(�κ��丮 �����ϰ� ���� �ǵ��)
				int temp;
				printf("---------------------------------\n");
				if (player->invenCount > 0) {
					for (int i = 0; i < player->invenCount; i++) {
						printf("%d. %s %d��\n", i + 1, player->Inventory[i]->itemName, player->Inventory[i]->quantity);
					}
					while (1) {
						printf("\n��ȣ �Է�:");
						scanf("%d", &temp);
						temp--;
						while (1) {
							if (temp >= 0 && temp < player->invenCount) {
								if (!strcmp(player->Inventory[temp]->itemName, "ü��ȸ�� ����")) {
									useItem(player, "ü��ȸ�� ����", 1);
									player->currentHP += 50;
									if (player->currentHP >= maxHP) {
										player->currentHP = maxHP;
										printf("ü���� �ִ�ġ���� ȸ���ߴ�!\n");
									}
									else {
										printf("ü���� 50 ȸ���ߴ�!\n");
									}
									break;
								}
								else if (!strcmp(player->Inventory[temp]->itemName, "��������� ����")) {
									useItem(player, "��������� ����", 1);
									if (ATKBonus[0] < 0) {
										ATKBonus[0] = 0;
									}
									if (DEFBonus[0] < 0) {
										DEFBonus[0] = 0;
									}
									if (Poison[0] > 0) {
										Poison[0] = 0;
									}
									if (Burn[0] > 0) {
										Burn[0] = 0;
									}
									if (Fear > 0) {
										Fear = 0;
										AP_player++;
									}
									break;
								}
								else if (!strcmp(player->Inventory[temp]->itemName, "����ȸ�� ����")) {
									useItem(player, "����ȸ�� ����", 1);
									player->currentMP += 20;
									if (player->currentMP >= maxMP) {
										player->currentMP = maxMP;
										printf("������ �ִ�ġ���� ȸ���ߴ�!\n");
									}
									else {
										printf("������ 20 ȸ���ߴ�!\n");
									}
									break;
								}
								else if (!strcmp(player->Inventory[temp]->itemName, "��� ����")) {
									useItem(player, "��� ����", 1);
									player->currentHP += 25;
									if (player->currentHP >= player->HP) {
										player->currentHP = player->HP;
										printf("ü���� �ִ�ġ���� ȸ���ϰ� ��� 4�� �����!\n");
									}
									else {
										printf("ü���� 25 ȸ���ϰ� ��� 4�� �����!\n");
									}
									Regen[0] = 4;
									break;
								}
								else if (!strcmp(player->Inventory[temp]->itemName, "AP ����")) {
									useItem(player, "AP ����", 1);
									AP_player++;
									printf("%s�� �ൿ�� ������ �̹� �� �߰� AP�� �����!\n", player->charname);
									break;

								}
								else {
									printf("�� �������� ���� �߿� ����� �� �����ϴ�!\n");
									break;
								}
							}
							else {
								printf("�߸��� �Է��Դϴ�.");
								break;
							}
						}
						break;
					}
				}
				else {
					printf("�������� �����ϴ�.\n");
				}
			}
			else if (!strcmp(input, "����")) {
				printf("\n�÷��̾� %s | ATK %.0f | DEF %.0f", player->charname, (player->ATK + player->weapon->weaponATK + player->armor->armorATK) * (1 + 0.1 * (ATKBonus[0] - Burn[0])), (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0])));
				printf("\n\n���� : %s\n%s\n%s\n\n�� : %s\n%s\n%s\n\n", player->weapon->weaponName, player->weapon->weaponInfo, player->weapon->weaponskill, player->armor->armorName, player->armor->armorInfo, player->armor->armorskill);
				printf("\n�� %s\nHP %d | ATK %.0f | DEF %.0f\n%s\n\n", enemy->enemyname, enemy->HP, enemy->ATK * (1 + 0.1 * (ATKBonus[0] - Burn[0])), enemy->DEF * (1 + 0.1 * (DEFBonus[0] - Poison[0])), enemy->enemyinfo);
				}
			if (Fear > 0) {
				Fear = 0;
			}
		}
		Eshield = 0;
		printf("\n����� ��!\n");
		while (AP_enemy > 0) {//�� ��
			_getch();
			randomatk = atkarr[(rand() % 10)];//�������� ������ ������

			if (randomatk == 0 || randomatk == 2) {
				if (randomatk == 0) {
					damage = (enemy->ATK) * (1 + 0.1 * (ATKBonus[1] - Burn[1])) - (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF * (1 + 0.1 * (DEFBonus[0] - Poison[0])));
					printf("%s�� ����! %d�� ���ظ� �־���!\n", enemy->enemyname, damage);
					if (!strcmp(enemy->enemyname, "�𵥵� ����") || !strcmp(enemy->enemyname, "�𵥵� ����") || !strcmp(enemy->enemyname, "�𵥵� ������")) {
						if (Estack > 0) {
							printf("%s�� �߰� ȿ���� ���� �ߵ��ƴ�!\n", player->charname);
							Poison[0]++;
							Estack--;
						}
					}
				}
				else if (randomatk == 2) {
					damage = (enemy->ATK) * (1.5 + 0.1 * (ATKBonus[1] - Burn[1])) - (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF * (1 + 0.1 * (DEFBonus[0] - Poison[0])));
					printf("%s�� ���� ����! %d�� ���ظ� �־���!\n", enemy->enemyname, damage);
					if (!strcmp(enemy->enemyname, "������") || !strcmp(enemy->enemyname, "���� ������") || !strcmp(enemy->enemyname, "���� ������")) {
						printf("%s�� ������ �����ߴ�!\n", player->charname);
						DEFBonus[0]--;
					}
					else if (!strcmp(enemy->enemyname, "�𵥵� ����") || !strcmp(enemy->enemyname, "�𵥵� ����") || !strcmp(enemy->enemyname, "�𵥵� ������")) {
						printf("%s�� ���� �ߵ��ƴ�!\n", player->charname);
						Poison[0]++;
						if (Estack > 0) {
							printf("%s�� �߰� ȿ���� ���� �ߵ��ƴ�!\n", player->charname);
							Poison[0]++;
							Estack--;
						}
					}
					else if (!strcmp(enemy->enemyname, "���൹ ��") || !strcmp(enemy->enemyname, "���� ��") || !strcmp(enemy->enemyname, "���� ��")) {
						printf("%s�� %d��ŭ�� ���� ���ظ� �߰��� �Ծ���!\n", player->charname, player->stage * 5);
						Pshield -= player->stage * 5;
						if (Pshield < 0) {
							player->currentHP += Pshield;
							Pshield = 0;
						}
					}
					else if (!strcmp(enemy->enemyname, "��� ��� ������") || !strcmp(enemy->enemyname, "��� ��� ����") || !strcmp(enemy->enemyname, "��� ��� ��ȣ��")) {
						if (Echarge < 0) {
							Echarge--;
							printf("%s�� ������ �������� ���� ȭ���� �Ծ���!\n", player->charname);
							Burn[0]++;
						}
					}
				}
				if (Will > 0) {
					printf("%s�� ������ ������ ���߳� ���ظ� ���� �ʾҴ�!\n", player->charname);
					Will--;
					damage = 0;
				}
				else {
					Pshield -= damage;
					if (Pshield < 0) {
						player->currentHP += Pshield;
						Pshield = 0;
					}
					if (player->currentHP <= 0) {
						player->currentHP = 0;
					}
				}
				if (!strcmp(player->weapon->weaponName, "�� ����")) {
					if ((player->currentHP < player->HP) && (player->currentMP >= 5)) {
						printf("%s�� ���� �нú�� %s�� �ݻ� ���� %d�� �޾Ҵ�!%s\n", RED, enemy->enemyname, (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0])), RESET);
						Eshield -= (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0]));
						if (Eshield < 0) {
							enemy->currentHP += Eshield;
							Eshield = 0;
							printf("%s ", enemy->enemyname);
							healthBar(enemy->currentHP, enemy->HP, Eshield);
						}
					}
				}
				if (!strcmp(player->weapon->weaponName, "���� ���� �� ����")) {
					if ((player->currentHP < player->HP) && (player->currentMP >= 5)) {
						printf("%s�� ���� �нú�� %s�� �ݻ� ���� %d�� �޾Ҵ�!%s\n", RED, enemy->enemyname, (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0])), RESET);
						Eshield -= (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0]));
						if (Eshield < 0) {
							enemy->currentHP += Eshield;
							Eshield = 0;
							printf("%s ", enemy->enemyname);
							healthBar(enemy->currentHP, enemy->HP, Eshield);
						}
					}
				}
				if (!strcmp(player->weapon->weaponName, "������ �� ����")) {
					if (Poison[1] > 0) {
						printf("%s������ �� ���� �нú�� %s�� ���ݷ��� �����d��!%s\n", YELLOW, player->charname, RESET);
						if (ATKBonus[0] <= 3) {
							ATKBonus[0]++;
						}
						if (player->currentMP < maxMP) {
							printf("%s������ �� ���� �нú�� %s�� ������ ȸ���ߴ�!%s\n", GREEN, player->charname, RESET);
							player->currentMP += 8;
							if (player->currentMP > maxMP) {
								player->currentMP = maxMP;
							}
						}
					}
				}


				printf("%s ", player->charname);
				healthBar(player->currentHP, maxHP, Pshield);
			}
			else if (randomatk == 1) {
				if (!strcmp(enemy->enemyname, "������") || !strcmp(enemy->enemyname, "���� ������") || !strcmp(enemy->enemyname, "���� ������")) {
					printf("%s�� ���� ������ ��ó�� ġ���Ǳ� �����ߴ�!\n", enemy->enemyname);
					Regen[1] += player->stage;
				}
				else if (!strcmp(enemy->enemyname, "�𵥵� ����") || !strcmp(enemy->enemyname, "�𵥵� ����") || !strcmp(enemy->enemyname, "�𵥵� ������")) {
					printf("%s�� ���⿡ ���� ������!\n", enemy->enemyname);
					Estack++;
					printf("%s���� ������ ������ %d������ ���� �ߵ��ȴ�!\n", enemy->enemyname, Estack);
				}
				else if (!strcmp(enemy->enemyname, "���൹ ��") || !strcmp(enemy->enemyname, "���� ��") || !strcmp(enemy->enemyname, "���� ��")) {
					printf("%s�� �ڽſ��� ������ �ɾ���!\n", enemy->enemyname);
					Estack += player->stage;
					printf("%s���� ������ ���ϸ� %d������ ���� ���ظ� �����޴´�!\n", enemy->enemyname, Estack);
				}
				else if (!strcmp(enemy->enemyname, "��� ��� ������") || !strcmp(enemy->enemyname, "��� ��� ����") || !strcmp(enemy->enemyname, "��� ��� ��ȣ��")) {
					if (Echarge < player->stage) {
						printf("%s�� ������ �������� �̿��� �ڽ��� ��ȭ�� ���ݷ��� �������״�!\n", enemy->enemyname);
						ATKBonus[1]++;
						Echarge++;
					}
					else if (Echarge >= player->stage && Echarge < player->stage * 2) {
						printf("%s�� ������ �������� �̿��� �ڽ��� ��ȭ�� ���ݷ��� �������״�!\n", enemy->enemyname);
						printf("%s�� ���������� ���� ������ �����ߴ�!\n", enemy->enemyname);
						ATKBonus[1]++;
						DEFBonus[1]--;
					}
					else {
						printf("%s�� ���������� ���� ������ �����ߴ�!\n", enemy->enemyname);
						DEFBonus[1]--;
					}
				}
			}
			else { printf("���� �Ǻ���"); }
			AP_enemy--;
			printf("�� ���� AP : %s%d%s \n\n", YELLOW, AP_enemy, RESET);
			judge(player, enemy);

		}

		if (Poison[0] > 0) {
			printf("%s�� ���� ���� ���ظ� %d �Ծ���!\n", player->charname, 5 * Poison[0]);
			player->currentHP -= 5 * Poison[0];
			Poison[0] -= player->stage;
			if (Poison[0] < 0) { Poison[0] = 0; printf("�÷��̾��� �ߵ��� �������!\n"); }
		}
		if (Poison[1] > 0) {

			printf("%s�� ���� ���� ���ظ� %d �Ծ���!\n", enemy->enemyname, 5 * Poison[1]);
			enemy->currentHP -= 5 * Poison[1];
			Poison[1] -= player->stage;
			if (Poison[1] <= 0) {
				Poison[1] = 0; printf("���� �ߵ��� �������!\n");
			}
		}
		judge(player, enemy);
		if (Burn[0] > 0) {
			printf("%s�� ȭ�� ���� ���ظ� %d �Ծ���!\n", enemy->enemyname, 5 * Burn[0]);
			player->currentHP -= 5 * Burn[0];
			Burn[0] -= player->stage;
			if (Burn[0] <= 0) {
				Burn[0] = 0; printf("�÷��̾��� ȭ���� ���Ҵ�!\n");
			}
		}
		if (Burn[1] > 0) {
			printf("%s�� ȭ�� ���� ���ظ� %d �Ծ���!\n", enemy->enemyname, 5 * Burn[1]);
			enemy->currentHP -= 5 * Burn[1];
			Burn[1] -= player->stage;
			if (Burn[1] <= 0) {
				Burn[1] = 0; printf("���� ȭ���� ���Ҵ�!\n");
			}
		}
		judge(player, enemy);
		if (Regen[0] > 0) {
			printf("%s�� %d�� ü���� ����ߴ�!\n", player->charname, 5 * Regen[0]);
			player->currentHP -= 5 * Regen[0];
			Regen[0] -= player->stage;
			if (player->currentHP > maxHP) { player->currentHP = maxHP; }
			if (Regen[0] <= 0) {
				Regen[0] = 0; printf("��� ȿ���� �������!\n");
			}
		}
		if (Regen[1] > 0) {
			printf("%s�� %d�� ü���� ����ߴ�!\n", enemy->enemyname, 5 * Regen[1]);
			enemy->currentHP += 5 * Regen[1];
			Regen[1] -= player->stage;
			if (enemy->currentHP > enemy->HP) { enemy->currentHP = enemy->HP; }
			if (Regen[1] <= 0) {
				Regen[1] = 0; printf("��� ȿ���� �������!\n");
			}
		}
		if (!strcmp(player->weapon->weaponName, "���� ��ȣ��")) {
			if (player->currentHP < maxHP) {
				printf("%s���� ��ȣ�� �нú�� %s�� ü���� 5 ȸ���ߴ�!%s\n", GREEN, player->charname, RESET);
				player->currentHP += 5;
				if (player->currentHP > maxHP) {
					player->currentHP = maxHP;
				}
			}
		}
		if (!strcmp(player->weapon->weaponName, "������ ����")) {
			if (player->currentHP < maxHP) {
				printf("%s������ ���� �нú�� %s�� ü���� 10 ȸ���ߴ�!%s\n", GREEN, player->charname, RESET);
				player->currentHP += 5;
				if (player->currentHP > maxHP) {
					player->currentHP = maxHP;
				}
				printf("%s������ ���� �нú�� %s�� ������ �����ߴ�!%s\n", GREEN, player->charname, RESET);
				if (DEFBonus[0] <= 3) {
					DEFBonus[0]++;
				}
			}
		}
		if (!strcmp(player->weapon->weaponName, "�� ����")) {
			if ((player->currentMP >= 12) && ((ATKBonus[0] < 0) || (DEFBonus[0] < 0) || (Burn[0] > 0) || (Poison[0] > 0))) {
				printf("%s�� ���� �нú�� %s�� ���� �̻��� �������!%s\n", GREEN, player->charname, RESET);
				if (ATKBonus[0] < 0) {
					ATKBonus[0] = 0;
				}
				if (DEFBonus[0] < 0) {
					DEFBonus[0] = 0;
				}
				if (Poison[0] > 0) {
					Poison[0] = 0;
				}
				if (Burn[0] > 0) {
					Burn[0] = 0;

				}
				player->currentMP -= 12;
			}
		}
		if (!strcmp(player->weapon->weaponName, "������ �� ����")) {
			if ((player->currentMP >= 12) && ((ATKBonus[0] < 0) || (DEFBonus[0] < 0) || (Burn[0] > 0) || (Poison[0] > 0))) {
				printf("%s������ �� ���� �нú�� %s�� ���� �̻��� �������!%s\n", GREEN, player->charname, RESET);
				if (ATKBonus[0] < 0) {
					ATKBonus[0] = 0;
				}
				if (DEFBonus[0] < 0) {
					DEFBonus[0] = 0;
				}
				if (Poison[0] > 0) {
					Poison[0] = 0;
				}
				if (Burn[0] > 0) {
					Burn[0] = 0;

				}
				player->currentMP -= 12;
			}
		}
		if (!strcmp(player->weapon->weaponName, "��� ������ ����")) {
			if (Pshield > 0) {
				printf("%s��� ������ ���� �нú�� %s�� ���� %d�� ȸ���ߴ�! %s\n", GREEN, player->charname, Pshield / 2, RESET);
				player->currentMP += Pshield / 2;
			}
		}
		if (!strcmp(player->weapon->weaponName, "��� ����")) {
			if (Pshield > 0) {
				printf("%s��� ������ ���� �нú�� %s�� ���� %d�� ȸ���ߴ�! %s\n", GREEN, player->charname, Pshield / 2, RESET);
				player->currentMP += Pshield / 2;
				if (ATKBonus[0] <= 3) {
					ATKBonus[0]++;
				}
			}
		}

		turn++;
		_getch();
	}
}

void bossRoom(Charstats* player, Enemystats* enemy) {//������
	strcpy(player->room, "boss");
	readEquipments(player);
	saveGame(player);
	battle = 1;
	int turn = 1;//���� ���� ���ο� �ϼ� ���
	int AP_player, AP_enemy, startline = 0;//���� ���� ü��, �ڽŰ� ����� ���� �ൿ�� ��Ÿ����
	char input[10];//�Է°� ����
	int ATKBonus[2] = { 0,0 }, DEFBonus[2] = { 0,0 }, Poison[2] = { 0,0 }, Burn[2] = { 0,0 }, Regen[2] = { 0,0 }, Will = 0, Fear = 0;//���� ȿ��: ���� ����, ��� ����, ��, ȭ��, ���, ����, ����
	//��: �� �� ����� ���� ���� �� ��ġ ����, ��ġ ��� ���� ���� | ȭ��: �� �ϸ��� ���� ���� �� ��ġ ����, ��ġ ��� ���ݷ� ����
	//���: �� �ϸ��� ��ġ ��� ȸ�� �� ��ġ ���� | ����: �ǰ� 1ȸ �������� ��ȿȭ�ϰ� ��ġ 1 ���� | ����: �� ���۽� �÷��̾��� �ൿ�� 1 ����
	//����, ������ �÷��̾� ���� Ư��ȿ���� �迭�� �������� ����
	//��� ������ ���� �÷��̾� �ִ� ü��, ���� ����
	int maxHP = player->HP + player->weapon->weaponHP + player->armor->armorHP;
	int maxMP = player->MP + player->weapon->weaponMP + player->armor->armorMP;
	int damage, Pshield, Eshield, Pcharge = 0, Pstack = 0, Echarge = 0, Estack = 0;//���� �� ����ϴ� ����
	int randomatk, atkarr[10] = { 0,0,0,0,0,1,1,1,2,2 };//���� �������� ������ �� �ְ� �ϴ� ����(������ Ư���⸦ �� �� ���� ����)
	srand((unsigned int)time(NULL));//������ ���� ���
	/*
	int encounter[] = { 1, 5 };

	if (player->stage == 1) {//���� ������ ������ Ư�� �ٺ��� ����ϴ� ������ ����
		startline = encounter[rand() % 2];//1���� ������ ���� 2��
	}
	else if (player->stage == 2) {
		startline = encounter[(rand() % 2)]+10;//2���� ������ ���� 2��
	}
	else if (player->stage == 3) {
		startline = encounter[(rand() % 2)]+20;//3���� ������ ���� 2��
	}
	*/

	if (player->stage == 1) {//���� ������ ������ Ư�� �ٺ��� ����ϴ� ������ ����
		startline = 5 * (rand() % 2);//1���� ������ ���� 2��
	}
	else if (player->stage == 2) {
		startline = 5 * (rand() % 2) + 10;//2���� ������ ���� 2��
	}
	else if (player->stage == 3) {
		startline = 5 * (rand() % 2) + 20;//3���� ������ ���� 2��
	}

	FILE* monster = fopen(bossdata, "r");
	if (monster == NULL) {
		printf("���� �����Ͱ� �����ϴ�.\n");
		return;
	}

	char line[100];
	int currentline = 1;
	if (startline == 0) {

	}
	else {
		while (fgets(line, sizeof(line), monster)) {//������ ���س��� �ٺ��� ���� �����͸� �о�´�
			if (currentline == startline) {
				break;
			}
			else {
				line[0] = '\0';
				currentline++;
			}
		}
	}

	fgets(enemy->enemyname, nameLen, monster);//���� ���ڵ��� ã�� ���ּ� �����͸� �о���� �� �����Ϳ� ���๮�ڰ� ���ԵǴ� ��� ����
	enemy->enemyname[strcspn(enemy->enemyname, "\n")] = '\0';
	fscanf(monster, "%d\n", &enemy->HP);
	fscanf(monster, "%d\n", &enemy->ATK);
	fscanf(monster, "%d\n", &enemy->DEF);
	fgets(enemy->enemyinfo, sizeof(enemy->enemyinfo), monster);//���� ���ڵ��� ã�� ���ּ� �����͸� �о���� �� �����Ϳ� ���๮�ڰ� ���ԵǴ� ��� ����
	enemy->enemyinfo[strcspn(enemy->enemyinfo, "\n")] = '\0';
	enemy->currentHP = enemy->HP;

	fclose(monster);

	_getch();
	printf("\n����� ���������� ������ �濡 ����.\n");
	_getch();
	printf("�����߻�!\n");
	_getch();
	printf("---------------------------------\n");
	printf("���� %s�� ��Ÿ����!\n\n", enemy->enemyname);
	if (!strcmp(enemy->enemyname, "������ �������� �ְ� ����")) {
		Eshield = 100;
	}
	else if (!strcmp(enemy->enemyname, "������ ������")) {
		Eshield = 400;
	}
	else {
		Eshield = 0;
	}
	if ((!strcmp(enemy->enemyname, "�Ƿ� [�Ѹ�]") && Estack > 0)) {
		Estack = 10;
	}
	while (battle == 1) {
		input[0] = '\0';
		Pshield = 0;

		printf("\n----------------TURN %d--------------\n\n", turn);
		if ((!strcmp(enemy->enemyname, "������ ������")) && Eshield > 0) {
			printf("������ �������� ��ȣ���� ������Ű�� ������ ������ �������� ���ݷ��� ���� �����մϴ�!\n");
			if (ATKBonus[1] <= 8) {
				ATKBonus[1] = 8;
			}
			DEFBonus[1] = 0;
		}if ((!strcmp(enemy->enemyname, "������ ������")) && Eshield == 0) {
			printf("������ �������� ��ȣ���� �����Ǿ� ������ �������� ������ ���� �����մϴ�!\n");
			if (DEFBonus[1] >= 5) {
				DEFBonus[1] = -5;
			}
			ATKBonus[1] = 0;
		}
		if ((!strcmp(enemy->enemyname, "�Ƿ� [�Ѹ�]") && Estack > 0)) {
			printf("�Ѹ��� �������� ������Ű�� ������ �Ѹ��� ������ ������ ������ ȭ�� �ɸ��ϴ�!\n");
			printf("��� ��ų�� ������ �Ϻ��ϰ� ���� �Ѹ��� �������� ������Ű����!!\n");
			_getch();
		}
		AP_enemy = 3;
		if (Fear > 0) {
			printf("%s�� ��Ⱑ ������ �̹� �� �ൿ���� �����Ѵ�!\n", player->charname);
			AP_player = 1;
		}
		else {
			AP_player = 2;
		}
		if (!strcmp(player->weapon->weaponName, "���� ���� �� ����")) {
			if (player->currentHP <= maxHP / 2) {
				printf("%s���� ���� �� ���� �нú�� %s�� ���� 1�� �����!%s\n", YELLOW, player->charname, RESET);
				Will++;
			}
		}
		printf("%s ", player->charname);
		healthBar(player->currentHP, maxHP, Pshield);
		printf("%s ", enemy->enemyname);
		healthBar(enemy->currentHP, enemy->HP, Eshield);
		if ((!strcmp(enemy->enemyname, "�Ƿ� [�Ѹ�]") && Estack > 0)) {
			printf("���� ������ :%d\n", Estack);
		}

		while (AP_player > 0) {//�÷��̾� ��
			printf("\n%s�� ������ �ұ�?\n", player->charname);
			printf("AP : %s%d%s | MP ", YELLOW, AP_player, RESET);
			ManaBar(player->currentMP, maxMP);
			bossjudge(player, enemy);
			printf("|����|���|������|����|");
			printf("---------------------------------\n�Է�: ");
			scanf("%s", input);

			if (!strcmp(input, "����")) {
				damage = (player->ATK + player->weapon->weaponATK + player->armor->armorATK) * (1 + 0.1 * (ATKBonus[0] - Burn[0])) - (enemy->DEF * (1 + 0.1 * (DEFBonus[1] - Poison[1])));

				if (!strcmp(player->weapon->weaponName, "������ ������ �ҵ�")) {
					if (Pstack > 0) {
						printf("%s������ ������ �ҵ� �нú�� ���ط��� �����ߴ�!%s\n", YELLOW, RESET);
						damage *= 1.25 * Pstack;
						_getch();
						printf("%s�� ����! %d�� ���ظ� �־���!\n", player->charname, damage);
						printf("%s������ ������ �ҵ� �нú�� %s�� ���� %d�� ȸ���ߴ�!%s\n", YELLOW, player->charname, 5 * Pstack, RESET);
						player->currentMP += 5 * Pstack;
						if (player->currentMP > maxMP) {
							player->currentMP = maxMP;
						}
						Pstack = 0;
					}
					else {
						printf("%s�� ����! %d�� ���ظ� �־���!\n", player->charname, damage);
					}
				}
			else if (!strcmp(player->weapon->weaponName, "������ ������ �ҵ�")) {
					if (Pstack > 0) {
						printf("%s������ ������ �ҵ� �нú�� ���ط��� �����ߴ�!%s\n", YELLOW, RESET);
						damage *= 1.5 * Pstack;
						_getch();
						printf("%s�� ����! %d�� ���ظ� �־���!\n", player->charname, damage);
						Burn[1] += Pstack / 2 + 1;
						printf("%s������ ������ �ҵ� �нú�� %s�� ȭ��  %d�� �ɷȴ�!%s\n", RED, enemy->enemyname, Pstack / 2, RESET);
						printf("%s������ ������ �ҵ� �нú�� %s�� ���� %d�� ȸ���ߴ�!%s\n", YELLOW, player->charname, 7 * Pstack, RESET);
						player->currentMP += 7 * Pstack;
						if (player->currentMP > maxMP) {
							player->currentMP = maxMP;
						}
						Pstack = 0;
					}
					else {
						printf("%s�� ����! %d�� ���ظ� �־���!\n", player->charname, damage);
					}
				}
				if ((strcmp(player->weapon->weaponName, "������ ������ �ҵ�") || !strcmp(player->weapon->weaponName, "������ ������ �ҵ�") || (strcmp(player->weapon->weaponName, "������ ������ �ҵ�")) == -1 || !strcmp(player->weapon->weaponName, "������ ������ �ҵ�") == -1))
				{

					printf("%s�� ����! %d�� ���ظ� �־���!\n", player->charname, damage);
				}

				Eshield -= damage;

				if (!strcmp(player->weapon->weaponName, "�� ���̽�")) {
					if ((player->currentHP < player->HP) && (player->currentMP >= 5)) {
						printf("%s�� ���̽� �нú�� %s�� �߰� ���� %d�� �޾Ҵ�!%s\n", RED, enemy->enemyname, (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0])), RESET);
						player->currentMP -= 5;
						Eshield -= (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0]));
					}
				}
				if (!strcmp(player->weapon->weaponName, "���� ���� �� ���̽�")) {
					if ((player->currentHP < player->HP) && (player->currentMP >= 5)) {
						printf("%s���� ���� �� ���̽� �нú�� %s�� �߰� ���� %d�� �޾Ҵ�!%s\n", RED, enemy->enemyname, (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0])), RESET);
						player->currentMP -= 5;
						Eshield -= (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0]));
						printf("%s���� ���� �� ���̽� �нú�� %s�� ȭ�� �ɷȴ�!%s\n", RED, enemy->enemyname, RESET);
						Burn[1]++;
					}
				}


				if (Eshield < 0) {
					enemy->currentHP += Eshield;
					Eshield = 0;
				}

				if (!strcmp(player->weapon->weaponName, "������ ��Ʈ")) {
					if (player->currentMP >= 5) {
						printf("%s������ ��Ʈ �нú�� %s�� ü���� ȸ���ߴ�!%s\n", GREEN, player->charname, RESET);
						player->currentMP -= 5;
						player->currentHP += 7;
						if (player->currentHP > maxHP) {
							player->currentHP = maxHP;
						}
					}
				}
				if (!strcmp(player->weapon->weaponName, "������ �ظ�")) {
					if (player->currentMP >= 5) {
						printf("%s������ �ظ� �нú�� %s�� ü���� ȸ���ߴ�!%s\n", GREEN, player->charname, RESET);
						player->currentMP -= 5;
						player->currentHP += 15;
						if (player->currentHP > maxHP) {
							player->currentHP = maxHP;
						}
						if (ATKBonus[0] < 2) {
							ATKBonus[0]++;
							printf("%s������ �ظ� �нú�� %s�� ���ݷ��� �����ߴ�!%s\n", RED, player->charname, RESET);
						}
					}
				}
				if (!strcmp(player->weapon->weaponName, "�ǽĿ� ��Į")) {
					if ((player->currentHP < maxHP) && (Poison[1] > 0)) {
						printf("%s�ǽĿ� ��Į �нú�� %s�� ü���� ȸ���ߴ�!%s\n", GREEN, player->charname, RESET);
						player->currentHP += 5;
						if (player->currentHP > maxHP) {
							player->currentHP = maxHP;
						}
					}
					if (player->currentMP >= 10) {
						printf("%s�ǽĿ� ��Į �нú�� %s�� ���� �ߵ��ƴ�!%s\n", RED, enemy->enemyname, RESET);
						player->currentMP -= 10;
						Poison[1] += 2;
					}
				}
				if (!strcmp(player->weapon->weaponName, "ô����")) {
					if (Poison[1] > 0) {
						if (player->currentHP < maxHP) {
							printf("%sô���� �нú�� %s�� ü���� ȸ���ߴ�!%s\n", GREEN, player->charname, RESET);
							player->currentHP += 15;
							if (player->currentHP > maxHP) {
								player->currentHP = maxHP;
							}
						}
						printf("%sô���� �нú�� %s�� ���� ���� 7�� �޾Ҵ�!%s\n", RED, enemy->enemyname, RESET);
						enemy->currentHP -= 7;
					}
					if (player->currentMP >= 10) {
						printf("%sô���� �нú�� %s�� ���� �ߵ��ƴ�!%s\n", RED, enemy->enemyname, RESET);
						player->currentMP -= 10;
						Poison[1] += 4;
					}
				}

				if (enemy->currentHP <= 0) {
					enemy->currentHP = 0;
				}
				printf("%s ", enemy->enemyname);
				healthBar(enemy->currentHP, enemy->HP, Eshield);
				if ((!strcmp(enemy->enemyname, "�Ƿ� [�Ѹ�]") && Estack > 0)) {
					printf("���� ������ :%d\n", Estack);
				}
				bossjudge(player, enemy);

				//�߰�ȿ�� �ߵ�

				AP_player--;
			}
			else if (!strcmp(input, "���")) {
				Pshield += (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF);
				printf("%s�� ��� �ڼ��� ���ߴ�!\n", player->charname);
				printf("�̹� �� ���� %s%d%s�� ��ȣ���� �߰��� ��´�!\n", BLUE, (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF), RESET);

				if (!strcmp(player->weapon->weaponName, "������ ������ �ҵ�")) {
					if (player->currentMP >= 10) {
						printf("%s������ ������ �ҵ� �нú�� ���� 10�� �Ҹ��� ������ �����!%s\n", BLUE, RESET);
						player->currentMP -= 10;
						Pstack++;
					}
				}
				if (!strcmp(player->weapon->weaponName, "������ ������ �ҵ�")) {
					if (player->currentMP >= 10) {
						printf("%s������ ������ �ҵ� �нú�� ���� 10�� �Ҹ��� ������ �����!%s\n", BLUE, RESET);
						player->currentMP -= 10;
						Pstack++;
					}
				}
				if (!strcmp(player->weapon->weaponName, "��� ����")) {
					if (player->currentMP >= 5) {
						printf("%s��� ���� �нú�� ���� 5�� �Ҹ��� ��ȣ�� 10�� �߰��� �����!%s\n", BLUE, RESET);
						player->currentMP -= 5;
						Pshield += 10;
					}
				}
				if (!strcmp(player->weapon->weaponName, "��� ������ ����")) {
					if (player->currentMP >= 10) {
						printf("%s��� ������ ���� �нú�� ���� 10�� �Ҹ��� ��ȣ�� 20�� �߰��� �����!%s\n", BLUE, RESET);
						player->currentMP -= 10;
						Pshield += 20;
					}
				}

				//�߰�ȿ�� �ߵ�
				AP_player--;
			}
			else if (!strcmp(input, "������")) {
				//������(�κ��丮 �����ϰ� ���� �ǵ��)
				int temp;
				printf("---------------------------------\n");
				if (player->invenCount > 0) {
					for (int i = 0; i < player->invenCount; i++) {
						printf("%d. %s %d��\n", i + 1, player->Inventory[i]->itemName, player->Inventory[i]->quantity);
					}
					while (1) {
						printf("\n��ȣ �Է�:");
						scanf("%d", &temp);
						temp--;
						while (1) {
							if (temp >= 0 && temp < player->invenCount) {
								if (!strcmp(player->Inventory[temp]->itemName, "ü��ȸ�� ����")) {
									useItem(player, "ü��ȸ�� ����", 1);
									player->currentHP += 50;
									if (player->currentHP >= maxHP) {
										player->currentHP = maxHP;
										printf("ü���� �ִ�ġ���� ȸ���ߴ�!\n");
									}
									else {
										printf("ü���� 50 ȸ���ߴ�!\n");
									}
									break;
								}
								else if (!strcmp(player->Inventory[temp]->itemName, "��������� ����")) {
									useItem(player, "��������� ����", 1);
									if (ATKBonus[0] < 0) {
										ATKBonus[0] = 0;
									}
									if (DEFBonus[0] < 0) {
										DEFBonus[0] = 0;
									}
									if (Poison[0] > 0) {
										Poison[0] = 0;
									}
									if (Burn[0] > 0) {
										Burn[0] = 0;
									}
									if (Fear > 0) {
										Fear = 0;
										AP_player++;
									}
									break;
								}
								else if (!strcmp(player->Inventory[temp]->itemName, "����ȸ�� ����")) {
									useItem(player, "����ȸ�� ����", 1);
									player->currentMP += 20;
									if (player->currentMP >= maxMP) {
										player->currentMP = maxMP;
										printf("������ �ִ�ġ���� ȸ���ߴ�!\n");
									}
									else {
										printf("������ 20 ȸ���ߴ�!\n");
									}
									break;
								}
								else if (!strcmp(player->Inventory[temp]->itemName, "��� ����")) {
									useItem(player, "��� ����", 1);
									player->currentHP += 25;
									if (player->currentHP >= player->HP) {
										player->currentHP = player->HP;
										printf("ü���� �ִ�ġ���� ȸ���ϰ� ��� 4�� �����!\n");
									}
									else {
										printf("ü���� 25 ȸ���ϰ� ��� 4�� �����!\n");
									}
									Regen[0] = 4;
									break;
								}
								else if (!strcmp(player->Inventory[temp]->itemName, "AP ����")) {
									useItem(player, "AP ����", 1);
									AP_player++;
									printf("%s�� �ൿ�� ������ �̹� �� �߰� AP�� �����!\n", player->charname);
									break;

								}
								else {
									printf("�� �������� ���� �߿� ����� �� �����ϴ�!\n");
									break;
								}
							}
							else {
								printf("�߸��� �Է��Դϴ�.");
								break;
							}
						}
						break;
					}
				}
				else {
					printf("�������� �����ϴ�.\n");
				}
			}
			else if (!strcmp(input, "����")) {
				printf("\n�÷��̾� %s | ATK %.0f | DEF %.0f", player->charname, (player->ATK + player->weapon->weaponATK + player->armor->armorATK) * (1 + 0.1 * (ATKBonus[0] - Burn[0])), (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0])));
				printf("\n\n���� : %s\n%s\n%s\n\n�� : %s\n%s\n%s\n\n", player->weapon->weaponName, player->weapon->weaponInfo, player->weapon->weaponskill, player->armor->armorName, player->armor->armorInfo, player->armor->armorskill);
				printf("\n�� %s\nHP %d | ATK %.0f | DEF %.0f\n%s\n\n", enemy->enemyname, enemy->HP, enemy->ATK * (1 + 0.1 * (ATKBonus[1] - Burn[1])), enemy->DEF * (1 + 0.1 * (DEFBonus[1] - Poison[1])), enemy->enemyinfo);
			}
		}
		if (Fear > 0) {
			Fear = 0;
		}
		if (strcmp(enemy->enemyname, "������ �������� �ְ� ����") && strcmp(enemy->enemyname, "������ ������")) {
			Eshield = 0;
		}
		printf("\n����� ��!\n");
		while (AP_enemy > 0) {//�� ��
			_getch();
			randomatk = atkarr[(rand() % 10)];//�������� ������ ������

			if (randomatk == 0 || randomatk == 2) {
				if (randomatk == 0) {
					damage = (enemy->ATK) * (1 + 0.1 * (ATKBonus[1] - Burn[1])) - (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF * (1 + 0.1 * (DEFBonus[0] - Poison[0])));
					printf("%s�� ����! %d�� ���ظ� �־���!\n", enemy->enemyname, damage);

				}
				else if (randomatk == 2) {
					damage = (enemy->ATK) * (1.5 + 0.1 * (ATKBonus[1] - Burn[1])) - (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF * (1 + 0.1 * (DEFBonus[0] - Poison[0])));
					printf("%s�� ���� ����! %d�� ���ظ� �־���!\n", enemy->enemyname, damage);
					if (!strcmp(enemy->enemyname, "�Ƿ�[�Ѹ�]") || (!strcmp(enemy->enemyname, "���� ���"))) {
						if (Fear == 0) {
							printf("%s�� ���� �������� ��Ⱑ ��������!\n", player->charname);
							Fear++;
						}
					}
					else if (!strcmp(enemy->enemyname, "������ ������")) {
						printf("%s�� ��Ʋ�Ÿ��� ƴ�� ������ �����ڴ� ��ȣ���� 10 �����ߴ�!\n", player->charname);
						Eshield += 10;
					}
				}
				if (Will > 0) {
					printf("%s�� ������ ������ ���߳� ���ظ� ���� �ʾҴ�!\n", player->charname);
					Will--;
					damage = 0;
				}
				else {
					Pshield -= damage;
					if ((!strcmp(enemy->enemyname, "�Ƿ� [�Ѹ�]") && Estack > 0)) {
						if (Pshield > 0) {
							printf("�Ѹ��� ������ ����!\n");
							Estack--;
						}
					}

					if (Pshield < 0) {
						player->currentHP += Pshield;
						Pshield = 0;
					}
					if (player->currentHP <= 0) {
						player->currentHP = 0;
					}
				}
				if (!strcmp(player->weapon->weaponName, "�� ����")) {
					if ((player->currentHP < player->HP) && (player->currentMP >= 5)) {
						printf("%s�� ���� �нú�� %s�� �ݻ� ���� %d�� �޾Ҵ�!%s\n", RED, enemy->enemyname, (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0])), RESET);
						Eshield -= (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0]));
						if (Eshield < 0) {
							enemy->currentHP += Eshield;
							Eshield = 0;
							printf("%s ", enemy->enemyname);
							healthBar(enemy->currentHP, enemy->HP, Eshield);
						}
					}
				}
				if (!strcmp(player->weapon->weaponName, "���� ���� �� ����")) {
					if ((player->currentHP < player->HP) && (player->currentMP >= 5)) {
						printf("%s�� ���� �нú�� %s�� �ݻ� ���� %d�� �޾Ҵ�!%s\n", RED, enemy->enemyname, (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0])), RESET);
						Eshield -= (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0]));
						if (Eshield < 0) {
							enemy->currentHP += Eshield;
							Eshield = 0;
							printf("%s ", enemy->enemyname);
							healthBar(enemy->currentHP, enemy->HP, Eshield);
						}
					}
				}
				if (!strcmp(player->weapon->weaponName, "������ �� ����")) {
					if (Poison[1] > 0) {
						printf("%s������ �� ���� �нú�� %s�� ���ݷ��� �����d��!%s\n", YELLOW, player->charname, RESET);
						if (ATKBonus[0] <= 3) {
							ATKBonus[0]++;
						}
						if (player->currentMP < maxMP) {
							printf("%s������ �� ���� �нú�� %s�� ������ ȸ���ߴ�!%s\n", GREEN, player->charname, RESET);
							player->currentMP += 8;
							if (player->currentMP > maxMP) {
								player->currentMP = maxMP;
							}
						}
					}
				}

				printf("%s ", player->charname);
				healthBar(player->currentHP, maxHP, Pshield);
			}
			else if (randomatk == 1) {
				if (!strcmp(enemy->enemyname, "��� ����") || (!strcmp(enemy->enemyname, "���� ���"))) {
					printf("%s�� ��� �¼��� ���ϰ� ��ȣ���� �����!\n", enemy->enemyname);
					Eshield += enemy->DEF * (1 + 0.1 * (DEFBonus[0] - Poison[0]));
				}
				else if (!strcmp(enemy->enemyname, "��� ����")) {
					printf("%s�� ������ �����ߴ�!\n", enemy->enemyname);
					Estack++;
					printf("%dȸ �� ������ �� ������ ������ ����Ѵ�!\n", 3 - Estack);
					if (Estack == 3) {
						printf("%s�� �Ŵ��� ȭ���� ��ȯ�� %s�� ���¿���! 5�� ȭ���� �߰��� �Ծ���!\n", enemy->enemyname, player->charname);
						Burn[0] += 5;
						Estack = 0;
					}
				}
				else if (!strcmp(enemy->enemyname, "������ ������")) {
					if (Eshield > 0) {
						printf("%s�� %s�� ���ĳ��� ��ȣ���� �����ߴ�! \n", enemy->enemyname, player->charname);
						Eshield += 30;
					}
					else {
						printf("%s�� ������ �����ߴ�!\n", enemy->enemyname);
						Estack++;
						if (Estack == 6) {
							printf("%s�� ��ȣ���� 200 ȸ���ߴ�! �װ��� ����� %s�� ��Ⱑ ��������...\n", enemy->enemyname, player->charname);
							Eshield = 200;
							Fear = 1;
							Estack = 0;
						}
						else {
							printf("%dȸ �� ������ �� ��ȣ���� 200 ȸ���Ѵ�!\n", 6 - Estack);
						}
					}

				}
				else if (!strcmp(enemy->enemyname, "�Ƿ�[�Ѹ�]")) {
					if (Estack > 0) {
						printf("%s�� ���� �������� %s�� ���¿���! %d�� ȭ���� �߰��� �Ծ���!\n", enemy->enemyname, player->charname, Estack / 2);
						Burn[0] += Estack / 2;
					}
					else if (Estack == 0) {
						printf("%s�� ������ �ֵѷ� %s�� �ڼ��� ��Ʈ��Ʈ�ȴ�!\n%s�� ������ �����ߴ�!\n", enemy->enemyname, player->charname, player->charname);
						DEFBonus[0]--;
					}
				}
			}
			else { printf("���� �Ǻ���"); }
			AP_enemy--;
			printf("�� ���� AP : %s%d%s \n\n", YELLOW, AP_enemy, RESET);
			bossjudge(player, enemy);

		}

		if (Poison[0] > 0) {
			printf("%s�� ���� ���� ���ظ� %d �Ծ���!\n", player->charname, 5 * Poison[0]);
			player->currentHP -= 5 * Poison[0];
			Poison[0] -= player->stage;
			if (Poison[0] < 0) { Poison[0] = 0; printf("�÷��̾��� �ߵ��� �������!\n"); }
		}
		if (Poison[1] > 0) {

			printf("%s�� ���� ���� ���ظ� %d �Ծ���!\n", enemy->enemyname, 5 * Poison[1]);
			enemy->currentHP -= 5 * Poison[1];
			Poison[1] -= player->stage;
			if (Poison[1] <= 0) {
				Poison[1] = 0; printf("���� �ߵ��� �������!\n");
			}
		}
		bossjudge(player, enemy);
		if (Burn[0] > 0) {
			printf("%s�� ȭ�� ���� ���ظ� %d �Ծ���!\n", enemy->enemyname, 5 * Burn[0]);
			player->currentHP -= 5 * Burn[0];
			Burn[0] -= player->stage;
			if (Burn[0] <= 0) {
				Burn[0] = 0; printf("�÷��̾��� ȭ���� ���Ҵ�!\n");
			}
		}
		if (Burn[1] > 0) {
			printf("%s�� ȭ�� ���� ���ظ� %d �Ծ���!\n", enemy->enemyname, 5 * Burn[1]);
			enemy->currentHP -= 5 * Burn[1];
			Burn[1] -= player->stage;
			if (Burn[1] <= 0) {
				Burn[1] = 0; printf("���� ȭ���� ���Ҵ�!\n");
			}
		}
		bossjudge(player, enemy);
		if (Regen[0] > 0) {
			printf("%s�� %d�� ü���� ����ߴ�!\n", player->charname, 5 * Regen[0]);
			player->currentHP -= 5 * Regen[0];
			Regen[0] -= player->stage;
			if (player->currentHP > maxHP) { player->currentHP = maxHP; }
			if (Regen[0] <= 0) {
				Regen[0] = 0; printf("��� ȿ���� �������!\n");
			}
		}
		if (Regen[1] > 0) {
			printf("%s�� %d�� ü���� ����ߴ�!\n", enemy->enemyname, 5 * Regen[1]);
			enemy->currentHP += 5 * Regen[1];
			Regen[1] -= player->stage;
			if (enemy->currentHP > enemy->HP) { enemy->currentHP = enemy->HP; }
			if (Regen[1] <= 0) {
				Regen[1] = 0; printf("��� ȿ���� �������!\n");
			}
		}
		if (!strcmp(player->weapon->weaponName, "���� ��ȣ��")) {
			if (player->currentHP < maxHP) {
				printf("%s���� ��ȣ�� �нú�� %s�� ü���� 5 ȸ���ߴ�!%s\n", GREEN, player->charname, RESET);
				player->currentHP += 5;
				if (player->currentHP > maxHP) {
					player->currentHP = maxHP;
				}
			}
		}
		if (!strcmp(player->weapon->weaponName, "������ ����")) {
			if (player->currentHP < maxHP) {
				printf("%s������ ���� �нú�� %s�� ü���� 10 ȸ���ߴ�!%s\n", GREEN, player->charname, RESET);
				player->currentHP += 5;
				if (player->currentHP > maxHP) {
					player->currentHP = maxHP;
				}
				printf("%s������ ���� �нú�� %s�� ������ �����ߴ�!%s\n", GREEN, player->charname, RESET);
				if (DEFBonus[0] <= 3) {
					DEFBonus[0]++;
				}
			}
		}
		if (!strcmp(player->weapon->weaponName, "�� ����")) {
			if ((player->currentMP >= 12) && ((ATKBonus[0] < 0) || (DEFBonus[0] < 0) || (Burn[0] > 0) || (Poison[0] > 0))) {
				printf("%s�� ���� �нú�� %s�� ���� �̻��� �������!%s\n", GREEN, player->charname, RESET);
				if (ATKBonus[0] < 0) {
					ATKBonus[0] = 0;
				}
				if (DEFBonus[0] < 0) {
					DEFBonus[0] = 0;
				}
				if (Poison[0] > 0) {
					Poison[0] = 0;
				}
				if (Burn[0] > 0) {
					Burn[0] = 0;

				}
				player->currentMP -= 12;
			}
		}
		if (!strcmp(player->weapon->weaponName, "������ �� ����")) {
			if ((player->currentMP >= 12) && ((ATKBonus[0] < 0) || (DEFBonus[0] < 0) || (Burn[0] > 0) || (Poison[0] > 0))) {
				printf("%s������ �� ���� �нú�� %s�� ���� �̻��� �������!%s\n", GREEN, player->charname, RESET);
				if (ATKBonus[0] < 0) {
					ATKBonus[0] = 0;
				}
				if (DEFBonus[0] < 0) {
					DEFBonus[0] = 0;
				}
				if (Poison[0] > 0) {
					Poison[0] = 0;
				}
				if (Burn[0] > 0) {
					Burn[0] = 0;

				}
				player->currentMP -= 12;
			}
		}
		if (!strcmp(player->weapon->weaponName, "��� ������ ����")) {
			if (Pshield > 0) {
				printf("%s��� ������ ���� �нú�� %s�� ���� %d�� ȸ���ߴ�! %s\n", GREEN, player->charname, Pshield / 2, RESET);
				player->currentMP += Pshield / 2;
			}
		}
		if (!strcmp(player->weapon->weaponName, "��� ����")) {
			if (Pshield > 0) {
				printf("%s��� ������ ���� �нú�� %s�� ���� %d�� ȸ���ߴ�! %s\n", GREEN, player->charname, Pshield / 2, RESET);
				player->currentMP += Pshield / 2;
				if (ATKBonus[0] <= 3) {
					ATKBonus[0]++;
				}
			}
		}

		turn++;
		_getch();
	}
}


void ManaBar(int CMP, int MMP) {
	int width = 10;
	int bar = (CMP * width) / MMP;

	char* color;
	color = BLUE;

	printf("[");
	for (int i = 0; i < width;i++) {
		if (i < bar) {
			printf("%s|%s", color, RESET);
		}
		else {
			printf(" ");
		}
	}
	printf("] %d/%d\n", CMP, MMP);
}

void healthBar(int CHP, int MHP, int shield) {
	int width = 20;
	int bar = (CHP * width) / MHP;

	char* color;
	if (CHP > MHP / 2) {
		color = GREEN;
	}
	else if (CHP > MHP / 4) {
		color = YELLOW;
	}
	else {
		color = RED;
	}

	printf("[");
	for (int i = 0; i < width;i++) {
		if (i < bar) {
			printf("%s|%s", color, RESET);
		}
		else {
			printf(" ");
		}
	}
	if (shield > 0) {
		printf("] %d/%d %s(+%d)%s\n", CHP, MHP, BLUE, shield, RESET);
	}
	else {
		printf("] %d/%d\n", CHP, MHP);
	}
}

void judge(Charstats* player, Enemystats* enemy) {
	if (enemy->currentHP <= 0) {
		enemy->currentHP = 0;
		printf("\n�¸��ߴ�!\n");
		printf("%s�� %s%d ���%s�� �����.\n", player->charname, YELLOW, player->stage * 20, RESET);
		player->gold += player->stage * 20;
		if (!strcmp(enemy->enemyname, "������") || !strcmp(enemy->enemyname, "���� ������") || !strcmp(enemy->enemyname, "���� ������")) {
			if (player->stage == 1) {
				printf("%s������ ������%s�� 1�� �����.\n", BLUE, RESET);
				addItem(player, "������ ������", 1);
			}
			else if (player->stage == 2) {
				printf("%s������ ��%s�� 1�� �����.\n", BLUE, RESET);
				addItem(player, "������ ��", 1);
			}
			else if (player->stage == 3) {
				printf("%s������ ������%s�� %s������ ��%s�� 1���� �����.\n", BLUE, RESET, BLUE, RESET);
				addItem(player, "������ ������", 1);
				addItem(player, "������ ��", 1);
			}
		}
		else if (!strcmp(enemy->enemyname, "�𵥵� ����") || !strcmp(enemy->enemyname, "�𵥵� ����") || !strcmp(enemy->enemyname, "�𵥵� ������")) {
			if (player->stage == 1) {
				printf("%s�𵥵� ������%s�� 1�� �����.\n", BLUE, RESET);
				addItem(player, "�𵥵� ������", 1);
			}
			else if (player->stage == 2) {
				printf("%s���ְ� ��� ��%s�� 1�� �����.\n", BLUE, RESET);
				addItem(player, "���ְ� ��� ��", 1);
			}
			else if (player->stage == 3) {
				printf("%s�𵥵� ������%s�� %s���ְ� ��� ��%s�� 1���� �����.\n", BLUE, RESET, BLUE, RESET);
				addItem(player, "�𵥵� ������", 1);
				addItem(player, "���ְ� ��� ��", 1);
			}
		}
		else if (!strcmp(enemy->enemyname, "���൹ ��") || !strcmp(enemy->enemyname, "���� ��") || !strcmp(enemy->enemyname, "���� ��")) {
			if (player->stage == 1) {
				printf("%s������ ������ ��%s�� 1�� �����.\n", BLUE, RESET);
				addItem(player, "������ ������ ��", 1);
			}
			else if (player->stage == 2) {
				printf("%s������ �帣�� ��%s�� 1�� �����.\n", BLUE, RESET);
				addItem(player, "������ �帣�� ��", 1);
			}
			else if (player->stage == 3) {
				printf("%s������ ������ ��%s�� %s������ �帣�� ��%s�� 1���� �����.\n", BLUE, RESET, BLUE, RESET);
				addItem(player, "������ ������ ��", 1);
				addItem(player, "������ �帣�� ��", 1);
			}
		}
		else if (!strcmp(enemy->enemyname, "��� ��� ������") || !strcmp(enemy->enemyname, "��� ��� ����") || !strcmp(enemy->enemyname, "��� ��� ��ȣ��")) {
			if (player->stage == 1) {
				printf("%s��� ��Ϲ���%s�� 1�� �����.\n", BLUE, RESET);
				addItem(player, "��� ��Ϲ���", 1);
			}
			else if (player->stage == 2) {
				printf("%s��� ����%s�� 1�� �����.\n", BLUE, RESET);
				addItem(player, "��� ����", 1);
			}
			else if (player->stage == 3) {
				printf("%s��� ��Ϲ���%s�� %s��� ����%s�� 1���� �����.\n", BLUE, RESET, BLUE, RESET);
				addItem(player, "��� ��Ϲ���", 1);
				addItem(player, "��� ����", 1);
			}
		}
		player->battleCount++;
		//���� ���� �ٸ� ���, ������ ��� �غ� ������ �Ѿ
		preparingRoom(player, enemy);
	}
	else if (player->currentHP <= 0) {
		player->currentHP = 0;

		GameOver(player);
	}
}

void bossjudge(Charstats* player, Enemystats* enemy) {
	if (enemy->currentHP <= 0) {
		enemy->currentHP = 0;
		printf("\n�¸��ߴ�!\n");
		_getch();
		printf("%s�� %s%d ���%s�� �����.\n", player->charname, YELLOW, player->stage * 100, RESET);
		player->gold += player->stage * 20;
			if (player->stage == 1) {
				printf("%s%s�� ������ �ö� ���� �ɷ�ġ�� ����ߴ�!%s\n\n", BLUE, player->charname, RESET);
				_getch();
				printf("���� HP  | 100 -> 150\n");
				printf("���� ATK | 20 -> 30\n");
				printf("���� DEF | 10 -> 15\n");
				printf("���� MP  | 10 -> 15\n");
				player->HP = 150;
				player->ATK = 35;
				player->DEF = 17;
				player->MP = 20;
				_getch();
				player->stage++;
				startingRoom(player, enemy);
			}
			else if (player->stage == 2) {
				printf("%s%s�� ������ �ö� ���� �ɷ�ġ�� ����ߴ�!%s\n\n", BLUE, player->charname, RESET);
				_getch();
				printf("���� HP  | 150 -> 225\n");
				printf("���� ATK | 30 -> 45\n");
				printf("���� DEF | 15 -> 25\n");
				printf("���� MP  | 20 -> 25\n");
				player->HP = 225;
				player->ATK = 45;
				player->DEF = 25;
				player->MP = 25;
				player->stage++;
				startingRoom(player, enemy);
			}
			else if (player->stage == 3) {
				End(player);
			}
	}
	else if (player->currentHP <= 0) {
		player->currentHP = 0;

		GameOver(player);
	}
}

void GameOver(Charstats* player) {
	printf("---------------------------------\n");
	_getch();
	printf("...\n");
	_getch();
	printf("����� ������ �ݰ� ��������.\n");
	_getch();
	printf("����� ������ ���������� �����ߴ�.\n");
	_getch();
	printf("��������� �ǽ� �ӿ���,\n");
	_getch();
	printf("����� ���������� ����� ���� ���Ҵ�.\n");
	_getch();
	printf("%s�� ���� ������ ������ ���� ��������� �־���.\n", player->charname);
	_getch();
	printf("����� ������ ����� ������ ������ �Ǿ� ������ ��ѷ�����.\n\n");
	_getch();
	printf("...?\n");
	_getch();
	printf("����� �̻����� ������.\n");
	_getch();
	printf("����� ���� ���������, ����� �ǽ��� ������ �������� �Ƿ�������..\n");
	_getch();
	printf("%s��¼��, ����� ������ �̰����� �ٽ� �Ͼ ����� �������� �𸥴�.%s\n", YELLOW, RESET);
	GAMEOVER = 1;
	//���� ȭ������...
	main();
}

void End(Charstats* player) {
	printf("������ Ŭ������ ����� �ٱ����� ������.\n");
	_getch();
	printf("���������� �޺��� ����� �ݱ��.\n");
	_getch();
	printf("����� �ڿ��� �ٶ󺸸� ������ ����.\n\n");
	_getch();
	printf("%s���� Ŭ����!%s\n\n", YELLOW, RESET);
	_getch();
	FILE* file = fopen(save, "w");
	if (file == NULL) {
		printf("���� ���� ����\n");
		return 1;
	}
	fclose(file);
	for (int i = 0;i < player->invenCount;i++) {
		free(player->Inventory[i]);
	}
	free(player->weapon);
	free(player->armor);
	main();
}

void addItem(Charstats* player, char name[30], int count) {//�κ��丮�� ������ �߰�
	char input[10];
	int input1;
	if (player->invenCount == 8) {//������ �ִ� �����ۼ� 8��
		while (1) {
			printf("�������� ���� ������ �����ϴ�! �������� ������?\n��/�ƴϿ�");
			scanf("%s", input);
			if (!strcmp(input, "��")) {
				printf("---------------------------------\n");
				for (int i = 0; i < player->invenCount; i++) {
					printf("%d. %s %d��\n", i + 1, player->Inventory[i]->itemName, player->Inventory[i]->quantity);
				}
				while (1) {
					printf("---------------------------------\n��ȣ �Է�:");
					scanf("%d", &input1);
					if (input1 > 0 && input1 <= player->invenCount) {
						printf("%s�� ������ %s�� ���� �־���!\n", player->Inventory[input1-1]->itemName, name);
						strcpy(player->Inventory[input1-1]->itemName, name);
						player->Inventory[input1-1]->quantity = count;
						break;
					}
					else {

					}
					break;
				}
			}
			else if (!strcmp(input, "�ƴϿ�")) {
				break;
			}
		}
	}
	else if (player->invenCount < 8) {
		int i = 0;
		for (i = 0;i < player->invenCount;i++) {
			if (!strcmp(player->Inventory[i]->itemName, name)) {
				player->Inventory[i]->quantity += count;
				break;
			}
		}
		if (i == player->invenCount) {
			player->Inventory[player->invenCount] = (_Item*)malloc(sizeof(_Item) * 30);
			strcpy(player->Inventory[player->invenCount]->itemName, name);
			player->Inventory[player->invenCount]->quantity = count;
			player->invenCount++;
		}
	}
}


void useItem(Charstats* player, char name[30], int count) {//�κ��丮�� �ִ� ������ ���
	
	if (player->invenCount > 0) {
		int i = 0;
		while ((player->invenCount > i)&&strcmp(name, player->Inventory[i])) {
			i++;
		}
		if (!strcmp(name, "ȸ�� ����") || !strcmp(name, "��������� ����") || !strcmp(name, "����ȸ�� ����") || !strcmp(name, "��� ����") || !strcmp(name, "AP ����")) {//��ȸ�� ������ ����
			printf("%s�� ����ߴ�!", player->Inventory[i]->itemName);
		}
		else {//���ۿ��� ������ ����Ҷ�, �κ��丮�� �ִ� ��� ������<-��� �����Ҷ��� ��� ���Ƴ��� �Լ��� ���� �����߰ڴ�
			//��� ��� X
		}
		player->Inventory[i]->quantity -= count;
		if (player->Inventory[i]->quantity == 0) {//�⼮�� ��ó�� �� �ε����� �Ҵ������ϰ� ��ĭ�����
	
			free(player->Inventory[i]);
			for (int k = i; k < player->invenCount; k++) {
				player->Inventory[k] = player->Inventory[k + 1];
			}
			player->invenCount--;
		}
	}
	else {
		printf("�������� �����ϴ�.\n");
	}
}

void startingRoom(Charstats* player, Enemystats* enemy) {//�� ���� ���� ��
	battle = 0;
	strcpy(player->room, "starting");
	player->battleCount = 0;
	saveGame(player);
	printf("---------------------------------\n");
	if (player->stage == 1) {
		player->storeItem1 = 2;
		player->storeItem2 = 2;
		player->storeItem3 = 1;
		player->storeItem4 = 1;
		printf("����� �̰����� �� ū �÷��� �ް� �� �Ŷ�� ���� ������.\n");
		_getch();
		printf("������ �̰��� ���� �̻�, ���� ���߸� �Ѵ�.\n");
		_getch();
		printf("����� ������ ������.\n");
		_getch();
		battleRoom(player, enemy);//1���̸� ����������, 2���̳� 3���̸�(���� �� ����) �غ������
	}
	else if (player->stage == 2) {
		player->storeItem1 = 3;
		player->storeItem2 = 3;
		player->storeItem3 = 2;
		player->storeItem4 = 1;
		printf("���� �ο��� ���� ����� ������ ���� ���� �����ߴ�.\n");
		_getch();
		printf("���� ���⼭ ���ļ��� �ȵȴ�.\n");
		_getch();
		printf("...���ư���.\n");
		_getch();
		preparingRoom(player, enemy);
	}
	else if (player->stage == 3) {
		player->storeItem1 = 3;
		player->storeItem2 = 3;
		player->storeItem3 = 2;
		player->storeItem4 = 2;
		printf("�쿩���� ���� ����� ������ ���� �����ߴ�.\n");
		_getch();
		printf("���� �����ʾ�����, ����� ������ ���� ���� ���ƾ� �Ѵٴ� ���� �ǻ����.\n");
		_getch();
		printf("����� �߰����� �Ű��.\n");
		_getch();
		preparingRoom(player, enemy);
	}
}

void newGame(Charstats* player, Enemystats* enemy) {//�޴����� �� ���� ���۽�
	GAMEOVER = 0;
	printf("�÷��̾� �̸� �Է� : ");
	scanf("%s", player->charname);
	printf("%s\n", player->charname);
	player->HP = 100;
	player->MP = 10;
	player->currentHP = player->HP;
	player->currentMP = player->MP;
	player->ATK = 20;
	player->DEF = 10;
	strcpy(player->weapon->weaponName, "���ָ�");
	strcpy(player->armor->armorName, "�Ǹ�");
	strcpy(player->weapon->weaponInfo, "�� ���ӿ��� ������ �����μ��� �� �ָ��̴�.");
	strcpy(player->armor->armorInfo, "�ڼ��� ���� ������ ������. �Ƹ���?");
	strcpy(player->weapon->weaponskill, "�ƹ��� ȿ���� ����.");
	strcpy(player->armor->armorskill, "�ƹ��� ȿ���� ����.");
	player->weapon->weaponATK = 0;
	player->weapon->weaponDEF = 0;
	player->weapon->weaponHP = 0;
	player->weapon->weaponMP = 0;
	player->armor->armorATK = 0;
	player->armor->armorDEF = 0;
	player->armor->armorHP = 0;
	player->armor->armorMP = 0;
	player->stage = 1;
	player->gold = 100;
	player->invenCount = 0;
	printf("---------------------------------\n������ ���� ���� ȯ���մϴ�.\n");
	_getch();
	printf("�������� ����� ������ ���۵˴ϴ�.\n");
	_getch();
	printf("�׷� ����� ���ϴ�.\n");
	_getch();
	printf("...\n");
	_getch();
	printf("����� ���� ����.\n");
	_getch();
	printf("�̰��� ������ ������ ��ٸ��� ������.\n");
	_getch();
	printf("����� ȣ��ɿ� �̲��� �߰����� �Ű��.\n");
	_getch();
	startingRoom(player, enemy);
}


void preparingRoom(Charstats* player, Enemystats* enemy) {//�غ��(����)
	battle = 0;
	strcpy(player->room, "preparing");
	saveGame(player);
	char input[10];
	printf("---------------------------------\n");
	printf("����� ����� �������� ��ҿ� �����ߴ�.\n");
	_getch();
	printf("�̰����� ��� �޽��� ���ϴ� �͵� ���� �� ����.\n");
	_getch();
	printf("�̰����� �޽��� ���ϰų� ������ �غ��� �� �ֽ��ϴ�.\n\n");
	_getch();
	printf("---------------------------------\n");
	readEquipments(player);
	//��� ������ ���� �÷��̾� �ִ� ü��, ���� ����
	int maxHP = player->HP + player->weapon->weaponHP + player->armor->armorHP;
	int maxMP = player->MP + player->weapon->weaponMP + player->armor->armorMP;
	while (1) {
		printf("����\n\n");
		printf("ȸ��\n\n");
		printf("����\n\n");
		printf("����\n\n");
		if (player->battleCount < 5) {
			printf("���� %s(���� %dȸ �� �ر�)%s\n", RED, 5 - player->battleCount, RESET);
		}
		else {
			printf("���� %s(���尡��)%s\n", YELLOW, RESET);
		}
		printf("\n����â\n");
		printf("---------------------------------\n");
		printf("�Է� : ");
		scanf("%s", input);
		if (!strcmp(input, "����")) {
			battleRoom(player, enemy);
		}
		else if (!strcmp(input, "ȸ��")) {
			char temp[10];
			printf("\n%d��带 �����ϰ� ü�°� ������ ȸ����Ű�ڽ��ϱ�?\n", player->stage * 50);
			printf("���� ��� : %d | HP : %d/%d | MP : %d/%d \n", player->gold, player->currentHP, maxHP, player->currentMP, maxMP);
			printf("�� / �ƴϿ�\n");
			scanf("%s", temp);
			if (!strcmp(temp, "��")) {
				if (player->gold >= player->stage * 50) {
					if (player->currentHP < maxHP || player->currentMP < maxMP) {
						printf("����� ������ ���Ⱑ ���ƿԴ�.\n");
						player->currentHP = maxHP;
						player->currentMP = maxMP;
						player->gold -= player->stage * 50;

					}
					else {
						printf("����� �̹� �ǰ��մϴ�.\n");
					}
				}
				else {
					printf("��尡 �����մϴ�.\n");
				}
			}
			else if (!strcmp(temp, "�ƴϿ�")) {
			}
			else {
				printf("�߸��� �Է��Դϴ�.\n");

			}
		}
		else if (!strcmp(input, "����")) {
			store(player);
		}
		else if (!strcmp(input, "����")) {
			crafting(player);
		}
		else if (!strcmp(input, "����")) {
			if (player->battleCount < 5) {
				printf("���� %dȸ�� �� �¸��ϸ� ������ �� �ֽ��ϴ�.\n", 5 - player->battleCount);
			}
			else {
				char temp[10];
				while (1) {
					printf("%s�������� �����Ͻðڽ��ϱ�? (��/�ƴϿ�)%s\n", YELLOW, RESET);
					scanf("%s", temp);
					if (!strcmp(temp, "��")) {
						bossRoom(player, enemy);
					}
					else if (strcmp(temp, "�ƴϿ�")) {
						break;
					}
				}
			}
		}
		else if (!strcmp(input, "����â")) {
			int equip;
			printf("---------------------------------\n");
			printf("�÷��̾� %s | �������� %d | %dG\n", player->charname, player->stage, player->gold);
			healthBar(player->currentHP, maxHP, 0);
			ManaBar(player->currentMP, maxMP);
			printf("\n\n���� : %s\n%s\n%s\n\n�� : %s\n%s\n%s\n\n", player->weapon->weaponName, player->weapon->weaponInfo, player->weapon->weaponskill, player->armor->armorName, player->armor->armorInfo, player->armor->armorskill);
			printf("�κ��丮 :\n");
			for (int i = 0; i < player->invenCount; i++) {
				printf("%d. %s %d��\n", i + 1, player->Inventory[i]->itemName, player->Inventory[i]->quantity);
			}
			printf("---------------------------------\n");
			equip = _getch() - '1';
			char temp[30];
			if (equip >= 0 && equip < player->invenCount) {
				if (!strcmp(player->Inventory[equip], "���ָ�") || !strcmp(player->Inventory[equip], "������ ��Ʈ") || !strcmp(player->Inventory[equip], "�� ���̽�") || !strcmp(player->Inventory[equip], "�ǽĿ� ��Į") || !strcmp(player->Inventory[equip], "������ ������ �ҵ�")) {
					strcpy(temp, player->weapon->weaponName);
					strcpy(player->weapon->weaponName, player->Inventory[equip]);
					strcpy(player->Inventory[equip], temp);
					printf("%s�� ����߽��ϴ�.\n", player->weapon->weaponName);
				}
				else if(!strcmp(player->Inventory[equip], "������ �ظ�") || !strcmp(player->Inventory[equip], "���� ���� �� ���̽�") || !strcmp(player->Inventory[equip], "ô����") || !strcmp(player->Inventory[equip], "������ ������ �ҵ�")){
					strcpy(temp, player->weapon->weaponName);
					strcpy(player->weapon->weaponName, player->Inventory[equip]);
					strcpy(player->Inventory[equip], temp);
					printf("%s�� ����߽��ϴ�.\n", player->weapon->weaponName);
				}
				else if(!strcmp(player->Inventory[equip], "�Ǹ�") || !strcmp(player->Inventory[equip], "���� ��ȣ��") || !strcmp(player->Inventory[equip], "������ ����") || !strcmp(player->Inventory[equip], "�� ����") || !strcmp(player->Inventory[equip], "���� ���� �� ����")){
					strcpy(temp, player->armor->armorName);
					strcpy(player->armor->armorName, player->Inventory[equip]);
					strcpy(player->Inventory[equip], temp);
					printf("%s�� ����߽��ϴ�.", player->armor->armorName);
				}
				else if (!strcmp(player->Inventory[equip], "�� ����") || !strcmp(player->Inventory[equip], "������ �� ����") || !strcmp(player->Inventory[equip], "��� ����") || !strcmp(player->Inventory[equip], "��� ������ ����")) {
					strcpy(temp, player->armor->armorName);
					strcpy(player->armor->armorName, player->Inventory[equip]);
					strcpy(player->Inventory[equip], temp);
					printf("%s�� ����߽��ϴ�.", player->armor->armorName);
				}
				strcpy(temp, "");
				readEquipments(player);
			}
		}
		else {
			printf("�߸��� �Է��Դϴ�.\n");
		}
	}
}

void store(Charstats* player) {

	printf("����� ���� �����̶��!\n");
	printf("��󺸽ðԳ�!\n");
	//�������� ���� ������ �ʱ�ȭ�Ǵ� �������� 4��
	int input;

	if (player->storeItem1 > 0) {
		printf("1. ü��ȸ�� ���� %d G �ܰ� : %d\nü���� 40 ȸ���Ѵ�.\n\n", 50 + (player->stage-1) *20 , player->storeItem1);
	}
	else {
		printf("1. ü��ȸ�� ����(ǰ��)\n\n");
	}
	if (player->storeItem2 > 0) {
		printf("2. ����ȸ�� ���� %d G �ܰ� : %d\n������ 20 ȸ���Ѵ�.\n\n", 30 + (player->stage - 1) * 10, player->storeItem2);
	}
	else {
		printf("2. ����ȸ�� ����(ǰ��)\n\n");
	}
	if (player->storeItem3 > 0) {
		printf("3. ��� ���� %d G �ܰ� : %d\n4�ϰ� ���������� ü���� ȸ���Ѵ�.\n\n", 75 + (player->stage - 1) * 30, player->storeItem3);
	}
	else {
		printf("3. ��� ����(ǰ��)\n\n");
	}
	if (player->storeItem4 > 0) {
		printf("4. AP ���� %d G �ܰ� : %d\n�̹� �� ���� �ൿ�� 1�� ��´�.\n\n", 120 + (player->stage - 1) * 40, player->storeItem4);
	}
	else {
		printf("4. AP ����(ǰ��)\n\n");
	}
	printf("���� ���: %d G", player->gold);
	while (1) {
		printf("��ȣ �Է� | ������=0:");
		scanf("%d", &input);

		if (input == 1) {
			if (player->gold >= 50 * 50 + (player->stage - 1) * 20) {
				player->gold -= 50 + (player->stage - 1) * 20;
				addItem(player, "ü��ȸ�� ����", 1);
				player->storeItem1--;
				printf("������ �ּż� ����!\n");
			}
			else if (player->storeItem1 == 0) {
				printf("�ƽ����� �� �ȷȴٳ�!\n");
			}
			else {
				printf("���� �����ϴٳ�!\n");
			}
		}
		else if (input==2) {
			if (player->gold >= 30 + (player->stage - 1) * 10) {
				player->gold -= 30 + (player->stage - 1) * 10;
				addItem(player, "����ȸ�� ����", 1);
				player->storeItem1--;
				printf("������ �ּż� ����!\n");
			}
			else if (player->storeItem1 == 0) {
				printf("�ƽ����� �� �ȷȴٳ�!\n");
			}
			else {
				printf("���� �����ϴٳ�!\n");
			}
		}
		else if (input==3) {
			if (player->gold >= 75 + (player->stage - 1) * 30) {
				player->gold -= 75 + (player->stage - 1) * 30;
				addItem(player, "��� ����", 1);
				player->storeItem1--;
				printf("������ �ּż� ����!\n");
			}
			else if (player->storeItem1 == 0) {
				printf("�ƽ����� �� �ȷȴٳ�!\n");
			}
			else {
				printf("���� �����ϴٳ�!\n");
			}
		}
		else if (input==4) {
			if (player->gold >= 120 + (player->stage - 1) * 40)
			{
				player->gold -= 120 + (player->stage - 1) * 40;
				addItem(player, "AP ����", 1);
				player->storeItem1--;
				printf("������ �ּż� ����!\n");
			}
			else if (player->storeItem1 == 0) {
				printf("�ƽ����� �� �ȷȴٳ�!\n");
			}
			else {
				printf("���� �����ϴٳ�!\n");
			}
		}
		else if (input==0) {
			printf("���� �Ϸ� �ǽñ�!\n\n");
			break;
		}
	}
}

void crafting(Charstats* player) {
	//���� �����س��� �Է¹ޱ�, ��ᰡ ����ϴٸ� ��� �Ҹ� �� ���� ��� �κ��丮�� �����ٵΰ� �� ��� �����͸� ���Ϸκ��� �Է¹���
	printf("���� ���� ������ ������ �� �͸� ���� ���̺��� ���� �ִ�.\n");
	printf("�̰����� ����ǰ���� ������ ������ �� ���� �� ����.\n");
	printf("---------------------------------\n���۴�\n");
	char input[10];
	int equipment;
	char equipment_made[50] = "";
	char material1[30] = "";
	char material2[30] = "";
	while (1) {
		printf("���� | �� | �׸��α�\n�Է� : ");
		scanf("%s", input);
		if (!strcmp(input, "����")) {
				printf("---------------------------------\n");
				printf("1. ������ ��Ʈ | ������ ������ *3\n\n2. �� ���̽� | ������ ������ �� *3\n\n");
				printf("3. �ǽĿ� ��Į | �𵥵� ������ *3\n\n4. ������ ������ �ҵ� | ��� ��Ϲ��� *3\n\n");
				if (player->stage > 1) {
					printf("5. ������ �ظ� | ������ ��Ʈ + ������ �� *2\n�Ǵ� ������ �� *4\n\n");
					printf("6. ���� ���� �� ���̽� | �� ���̽� + ������ �帣�� �� *2\n�Ǵ� ������ �帣�� �� *4\n\n");
					printf("7. ô���� | �ǽĿ� ��Į + ���ְ� ��� �� *2\n�Ǵ� ���ְ� ��� �� *4\n\n");
					printf("8. ������ ������ �ҵ� | ������ ������ �ҵ� + ��� ���� *2\n�Ǵ� ��� ���� *4\n");
				}
				while (1){
				printf("---------------------------------\n���� ���\n\n");
				printf("������ ������ : %d | ", showItemCount(player, "������ ������"));
				printf("������ ������ ��: %d \n", showItemCount(player, "������ ������ ��"));
				printf("�𵥵� ������ : %d | ", showItemCount(player, "�𵥵� ������"));
				printf("��� ��Ϲ���: %d \n", showItemCount(player, "��� ��Ϲ���"));
				if (player->stage > 1) {
					printf("������ �� : %d | ", showItemCount(player, "������ ��"));
					printf("������ �帣�� ��: %d \n", showItemCount(player, "������ �帣�� ��"));
					printf("�𵥵� ������ : %d | ", showItemCount(player, "���ְ� ��� ��"));
					printf("��� ��Ϲ���: %d \n", showItemCount(player, "��� ����"));
				}
				printf("0. ���\n");
				printf("---------------------------------\n�Է� | :");
				scanf("%d", &equipment);

				if (equipment == 0) {
					break;
				}

				if (equipment == 1) {
					strcpy(equipment_made, "������ ��Ʈ");
					strcpy(material1, "������ ������");
				}
				else if (equipment == 2) {
					strcpy(equipment_made, "�� ���̽�");
					strcpy(material1, "������ ������ ��");
				}
				else if (equipment == 3) {
					strcpy(equipment_made, "�ǽĿ� ��Į");
					strcpy(material1, "�𵥵� ������");
				}
				else if (equipment == 4) {
					strcpy(equipment_made, "������ ������ �ҵ�");
					strcpy(material1, "��� ��Ϲ���");
				}
				else if (player->stage > 1) {  // 2�ܰ� �̻��� ���⸸ ó��
					if (equipment == 5) {
						strcpy(equipment_made, "������ �ظ�");
						strcpy(material1, "������ ��");
						strcpy(material2, "������ ��Ʈ");
					}
					else if (equipment == 6) {
						strcpy(equipment_made, "���� ���� �� ���̽�");
						strcpy(material1, "������ �帣�� ��");
						strcpy(material2, "�� ���̽�");
					}
					else if (equipment == 7) {
						strcpy(equipment_made, "ô����");
						strcpy(material1, "���ְ� ��� ��");
						strcpy(material2, "�ǽĿ� ��Į");
					}
					else if (equipment == 8) {
						strcpy(equipment_made, "������ ������ �ҵ�");
						strcpy(material1, "��� ����");
						strcpy(material2, "������ ������ �ҵ�");
					}
				}

				if (player->stage > 1 && material2[0] != '\0') {
					if (showItemCount(player, material1) >= 4) {
						useItem(player, material1, 4);
						addItem(player, equipment_made, 1);
						printf("%s�� �����ߴ�!\n", equipment_made);
					}
					else if (showItemCount(player, material2) >= 1 && showItemCount(player, material1) >= 2) {
						if (!strcmp(player->weapon->weaponName, material2)) {
							strcpy(player->weapon->weaponName, "���ָ�");
						}
						else {
							useItem(player, material2, 1);
						}
						useItem(player, material1, 2);
						addItem(player, equipment_made, 1);
						printf("%s�� �����ߴ�!\n", equipment_made);
					}
					else {
						printf("��ᰡ �����մϴ�!\n\n");
					}
				}
				else {
					if (showItemCount(player, material1) >= 3) {
						useItem(player, material1, 3);
						addItem(player, equipment_made, 1);
						printf("%s�� �����ߴ�!\n", equipment_made);
						break;
					}
					else {
						printf("%s", material1);
						printf("%s", equipment_made);
						printf("%d", showItemCount(player, material1));
						printf("��ᰡ �����մϴ�!\n\n");
					}
				}
			}
		}
		else if (!strcmp(input, "��")) {
				printf("---------------------------------\n");
				printf("1. ���� ��ȣ�� | ������ ������ *3\n\n2. �� ���� | ������ ������ �� *3\n\n");
				printf("3. �� ���� | �𵥵� ������ *3\n\n4. ��� ���� | ��� ��Ϲ��� *3\n\n");
				if (player->stage > 1) {
					printf("5. ������ ���� | ���� ��ȣ�� + ������ �� *2\n�Ǵ� ������ �� *4\n\n");
					printf("6. ���� ���� �� ���� | �� ���� + ������ �帣�� �� *2\n�Ǵ� ������ �帣�� �� *4\n\n");
					printf("7. ������ �� ���� | �� ���� + ���ְ� ��� �� *2\n�Ǵ� ���ְ� ��� �� *4\n\n");
					printf("8. ��� ������ ���� | ��� ���� + ��� ���� *2\n�Ǵ� ��� ���� *4\n");
				}
				while (1) {
				printf("---------------------------------\n���� ���\n\n");
				printf("������ ������ : %d | ", showItemCount(player, "������ ������"));
				printf("������ ������ ��: %d \n", showItemCount(player, "������ ������ ��"));
				printf("�𵥵� ������ : %d | ", showItemCount(player, "�𵥵� ������"));
				printf("��� ��Ϲ���: %d \n", showItemCount(player, "��� ��Ϲ���"));
				if (player->stage > 1) {
					printf("������ �� : %d | ", showItemCount(player, "������ ��"));
					printf("������ ������ ��: %d \n", showItemCount(player, "������ �帣�� ��"));
					printf("�𵥵� ������ : %d | ", showItemCount(player, "���ְ� ��� ��"));
					printf("��� ��Ϲ���: %d \n", showItemCount(player, "��� ����"));
				}
				printf("0. ���\n");
				printf("---------------------------------\n�Է� | :");
				scanf("%d", &equipment);

				if (equipment == 0) {
					break;
				}

				if (equipment == 1) {
					strcpy(equipment_made, "���� ��ȣ��");
					strcpy(material1, "������ ������");
				}
				else if (equipment == 2) {
					strcpy(equipment_made, "�� ����");
					strcpy(material1, "������ ������ ��");
				}
				else if (equipment == 3) {
					strcpy(equipment_made, "�� ����");
					strcpy(material1, "�𵥵� ������");
				}
				else if (equipment == 4) {
					strcpy(equipment_made, "��� ����");
					strcpy(material1, "��� ��Ϲ���");
				}
				else if (player->stage > 1) {  // 2�ܰ� �̻��� ���⸸ ó��
					if (equipment == 5) {
						strcpy(equipment_made, "������ ����");
						strcpy(material1, "������ ��");
						strcpy(material2, "���� ��ȣ��");
					}
					else if (equipment == 6) {
						strcpy(equipment_made, "���� ���� �� ����");
						strcpy(material1, "������ �帣�� ��");
						strcpy(material2, "�� ����");
					}
					else if (equipment == 7) {
						strcpy(equipment_made, "������ �� ����");
						strcpy(material1, "���ְ� ��� ��");
						strcpy(material2, "�� ����");
					}
					else if (equipment == 8) {
						strcpy(equipment_made, "��� ������ ����");
						strcpy(material1, "��� ����");
						strcpy(material2, "��� ����");
					}
				}

				if (player->stage > 1 && material2[0] != '\0') {
					if (showItemCount(player, material1) >= 4) {
						useItem(player, material1, 4);
						addItem(player, equipment_made, 1);
						printf("%s�� �����ߴ�!\n", equipment_made);
					}
					else if (showItemCount(player, material2) >= 1 && showItemCount(player, material1) >= 2) {
						if (!strcmp(player->armor->armorName, material2)) {
							strcpy(player->armor->armorName, "�Ǹ�");
						}
						else {
							useItem(player, material2, 1);
						}
						useItem(player, material1, 2);
						addItem(player, equipment_made, 1);
						printf("%s�� �����ߴ�!\n", equipment_made);
					}
					else {
						printf("��ᰡ �����մϴ�!\n\n");
					}
				}
				else {
					if (showItemCount(player, material1) >= 3) {
						useItem(player, material1, 3);
						addItem(player, equipment_made, 1);
						printf("%s�� �����ߴ�!\n", equipment_made);
						break;
					}
					else {
						printf("%s", material1);
						printf("%s", equipment_made);
						printf("%d", showItemCount(player, material1));
						printf("��ᰡ �����մϴ�!\n\n");
					}
				}
			}
		}
		else if (!strcmp(input, "�׸��α�")) {
			printf("�׸��ξ���.\n\n\n");
			break;
		}
	}


}

int showItemCount(Charstats* player, char name[30]) {
	int i;
	// �÷��̾��� �κ��丮�� ��ȸ�ϸ� �̸��� ��ġ�ϴ� �������� ã���ϴ�.
	for (i = 0; i < player->invenCount; i++) {
		if (player->Inventory[i] != NULL && !strcmp(player->Inventory[i]->itemName, name)) {
			// �������� �κ��丮�� ������ ���� ���� ���� ��� +1 ��ȯ
			if (!strcmp(player->weapon->weaponName, name) || !strcmp(player->armor->armorName, name)) {
				return player->Inventory[i]->quantity + 1;
			}
			// �κ��丮���� �ִ� ��� ���� ��ȯ
			else {
				return player->Inventory[i]->quantity;
			}
		}
	}
	// ���� ��� ���� ���⳪ ������ �̸��� ��ġ�� ��� 1 ��ȯ
	if (!strcmp(player->weapon->weaponName, name) || !strcmp(player->armor->armorName, name)) {
		return 1;
	}
	// �κ��丮���� ���� ���� ��񿡵� ���� ��� 0 ��ȯ
	return 0;
}


void startMenu(Charstats* player, Enemystats* enemy) {
	char input[10];
	printf("Text RPG GAME\n\n---------------------------------\n");
		if (GAMEOVER == 1) {
			printf("�����ϱ�\n\n%s�̾��ϱ�%s\n\nƩ�丮��\n", YELLOW, RESET);
		}
		else {
			printf("�����ϱ�\n\n�̾��ϱ�\n\nƩ�丮��\n");
		}
	printf("---------------------------------\n");
	while (1) {
		printf("�Է� : ");
		scanf("%s", input);
		if (!strcmp(input, "�����ϱ�")) {
			newGame(player, enemy);
		}
		else if (!strcmp(input, "�̾��ϱ�")) {
			loadGame(player, enemy);
		}
		else if (!strcmp(input, "Ʃ�丮��")) {
			HowTo(player);
		}
		else {
			printf("�߸� �Է��߽��ϴ�.\n");
		}
	}
}
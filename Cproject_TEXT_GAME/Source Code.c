#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>//키보드 입력시 넘어가기 구현
#include <time.h>//몬스터, 상점 무작위 생성 구현

#define RESET "\033[0m"//체력바, 상태효과에 쓸 색상들
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
}_Item;//인벤토리에 들어갈 아이템 구조체

typedef  struct {//플레이어 정보 구조체
	char charname[nameLen];
	int HP;
	int MP;
	int currentHP;
	int currentMP;
	int ATK;
	int DEF;
	_Weapon* weapon;//구조체 포인터를 써서 무기나 갑옷이 지원하는 능력치에 대해서도 쓸거임
	_Armor* armor;
	_Item* Inventory[8];//8칸짜리 인벤토리 구현
	int invenCount;
	int stage;
	char room[10];
	int gold;
	int battleCount;

	int storeItem1;
	int storeItem2;
	int storeItem3;
	int storeItem4;
	//상점 아이템-얘네는 스테이지 단위로 초기화되어서 전역변수로 선언함

}Charstats;
typedef struct {//적 정보 구조체
	char enemyname[nameLen];
	int HP;
	int currentHP;
	int ATK;
	int DEF;
	char enemyinfo[100];

}Enemystats;

void saveGame(Charstats* player);//세이브
void loadGame(Charstats* player, Enemystats* enemy);//이어하기
void startingRoom(Charstats* player, Enemystats* enemy);//시작 방
void battleRoom(Charstats* player, Enemystats* enemy);//일반 전투 방
void preparingRoom(Charstats* player, Enemystats* enemy);//준비 방
void store(Charstats* player);//준비방-상점
void crafting(Charstats* player);//준비방-장비 제작
void bossRoom(Charstats* player, Enemystats* enemy);//보스 전투 방
void End(Charstats* player);//엔딩
void newGame(Charstats* player, Enemystats* enemy);//새 게임 시작
void HowTo();//메인화면-튜토리얼
void startMenu(Charstats* player, Enemystats* enemy);//메인화면
void judge(Charstats* player, Enemystats* enemy);//전투 시 승패 판정
void bossjudge(Charstats* player, Enemystats* enemy);//보스 전투 시 승패 판정
void healthBar(int CHP, int MHP, int shield);//체력바 출력
void ManaBar(int CMP, int MMP);//마나바 출력
void GameOver(Charstats* player);//게임오버
void addItem(Charstats* player, char name[30], int count);//인벤토리에 아이템 추가
void useItem(Charstats* player, char name[30], int count);//인벤토리 내의 아이템 사용
void readEquipments(Charstats* player);//장비 데이터 파일에서 읽어오기
int showItemCount(Charstats* player, char name[30]);//인벤토리에 제시한 아이템과 같은 아이템이 있는지 찾는 함수


int battle = 0;//전투 중인지 아닌지 여부를 나타내는 전역변수
//장비 변경은 전투중에는 불가능, 재생 물약 등의 사용은 전투중에만 가능
int GAMEOVER = 0;//사망 횟수를 나타내는 전역 변수

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

void saveGame(Charstats* player) {//저장
	FILE* file = fopen(save, "w");
	if (file == NULL) {
		printf("파일을 열 때 에러가 발생했습니다.\n");
		return;
	}

	fprintf(file, "%s\n", player->charname);//이름
	fprintf(file, "%d\n", player->HP);//최대 체력
	fprintf(file, "%d\n", player->MP);//최대 마나
	fprintf(file, "%d\n", player->currentHP);//현재 체력
	fprintf(file, "%d\n", player->currentMP);//현재 마나
	fprintf(file, "%d\n", player->ATK);//공격력
	fprintf(file, "%d\n", player->DEF);//방어력

	fprintf(file, "%s\n", player->weapon->weaponName);//무기 이름(장비 정보는 무기 이름을 통해 불러올 것이기 때문)
	fprintf(file, "%s\n", player->armor->armorName);//방어구 이름

	fprintf(file, "%d\n", player->stage);//스테이지
	fprintf(file, "%s\n", player->room);//방 종류
	fprintf(file, "%d\n", player->gold);//골드
	fprintf(file, "%d\n", player->battleCount);//스테이지에서 전투를 진행한 수
	fprintf(file, "%d\n", player->storeItem1);//상점 물건들 수량
	fprintf(file, "%d\n", player->storeItem2);//상점 물건들 수량
	fprintf(file, "%d\n", player->storeItem3);//상점 물건들 수량
	fprintf(file, "%d\n", player->storeItem4);//상점 물건들 수량
	fprintf(file, "%d\n\n", player->invenCount);//보유한 아이템의 수
	for (int i = 0;i < player->invenCount; i++) {
		fprintf(file, "%d %s\n", player->Inventory[i]->quantity, player->Inventory[i]->itemName);//인벤토리에 있는 아이템들
	}
	printf("파일 저장됨.\n");
	fclose(file);

}

void loadGame(Charstats* player, Enemystats* enemy) {//로드
	FILE* file = fopen(save, "r");
	if (file == NULL) {
		printf("저장된 게임이 없습니다.\n");
		return;
	}
	fgets(player->charname, nameLen, file);//개행 문자들을 찾고 없애서 데이터를 읽어왔을 때 데이터에 개행문자가 포함되는 경우 막기
	player->charname[strcspn(player->charname, "\n")] = '\0';//이름에 띄어쓰기가 포함될 수도 있어 캐릭터나 장비이름 읽어올 때는 fgets를 사용함

	fscanf(file, "%d\n", &player->HP);
	fscanf(file, "%d\n", &player->MP);
	fscanf(file, "%d\n", &player->currentHP);
	fscanf(file, "%d\n", &player->currentMP);
	fscanf(file, "%d\n", &player->ATK);
	fscanf(file, "%d\n", &player->DEF);

	fgets(player->weapon->weaponName, nameLen, file);//개행 문자들을 찾고 없애서 데이터를 읽어왔을 때 데이터에 개행문자가 포함되는 경우 막기
	player->weapon->weaponName[strcspn(player->weapon->weaponName, "\n")] = '\0';
	fgets(player->armor->armorName, nameLen, file);//개행 문자들을 찾고 없애서 데이터를 읽어왔을 때 데이터에 개행문자가 포함되는 경우 막기
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
		fgets(player->Inventory[i]->itemName, nameLen, file);//개행 문자들을 찾고 없애서 데이터를 읽어왔을 때 데이터에 개행문자가 포함되는 경우 막기
		player->Inventory[i]->itemName[strcspn(player->Inventory[i]->itemName, "\n")] = '\0';
	}
	printf("성공적으로 불러왔습니다.\n");
	if (GAMEOVER == 1) {
		printf("...!\n");
		_getch();
		printf("당신은 다시 눈을 떴다.\n");
		_getch();
		printf("먼지가 되었던 몸은 형체를 찾았다.\n");
		_getch();
		printf("당신은 인벤토리를 열어봤다. 인벤토리는 얼마 전의 그것 그대로였다.\n");
		_getch();
		printf("그제서야, 당신은 당신이 죽기 몇 분 전으로 돌아왔다는 것을 깨달았다.\n");
		_getch();
		printf("당신은 내면의 결의를 느꼈다.\n");
		printf("%s당신의 HP와 MP가 최대로 회복되었다!%s", GREEN, RESET);
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

void HowTo() {//플레이 방법
	printf("---------------------------------\n");
	printf("만든이: 광운대학교 로봇학부 24학번 서현규\n");
	printf("\n");
	printf("키보드를 눌러서 다음 텍스트로 넘어갈 수 있습니다.\n");
	_getch();
	printf("당신은 이제부터 어떤 던전에 들어가게 됩니다.\n");
	_getch();
	printf("당신의 몸에는 현재 아무것도 없습니다.\n");
	_getch();
	printf("전투를 통해 얻은 전리품으로 더 좋은 장비를 제작하세요.\n");
	_getch();
	printf("장비에 따라 고유 능력치와 스킬이 다릅니다.\n");
	_getch();
	printf("전투에서는 기본적으로 2개의 AP(행동 기회)가 주어집니다.\n");
	_getch();
	printf("공격과 방어는 AP를 소모하지만,\n");
	_getch();
	printf("아이템 사용이나 정보 보기는 AP를 소모하지 않습니다.\n");
	_getch();
	printf("적이 보스라면 상대에게는 3번의 행동 기회가 주어집니다.\n");
	_getch();
	printf("무기와 스킬을 전략적으로 사용해 승리를 쟁취하세요.\n");
	_getch();
	printf("---------------------------------\n");
	return;
}

void readEquipments(Charstats* player) {
	FILE* weapon = fopen(weapondata, "r");//장비 데이터 읽어오기
	if (weapon == NULL) {
		printf("파일을 열 때 에러가 발생했습니다.\n");
		return;
	}

	char line1[weaponcount * 7];
	int found = 0;      // 문자열을 찾았는지 여부를 나타내는 플래그

	// 파일의 줄을 하나씩 읽으면서 처리
	while (fgets(line1, sizeof(line1), weapon)) {
		line1[strcspn(line1, "\n")] = '\0'; // 개행 문자 제거
		//printf("%s", player->weapon->weaponName);
		// 문자열을 찾으면 탈출
		if (!found && strcmp(player->weapon->weaponName, line1) == 0) {
			found = 1; // 문자열을 찾았음을 표시
			break;
		}
	}
	if (!found) {
		printf("파일에서 '%s'와 일치하는 줄을 찾을 수 없습니다.\n", player->weapon->weaponName);
	}
	fscanf(weapon, "%d\n", &player->weapon->weaponHP);
	fscanf(weapon, "%d\n", &player->weapon->weaponATK);
	fscanf(weapon, "%d\n", &player->weapon->weaponDEF);
	fscanf(weapon, "%d\n", &player->weapon->weaponMP);
	fgets(player->weapon->weaponskill, 150, weapon);//개행 문자들을 찾고 없애서 데이터를 읽어왔을 때 데이터에 개행문자가 포함되는 경우 막기
	player->weapon->weaponskill[strcspn(player->weapon->weaponskill, "\n")] = '\0';
	fgets(player->weapon->weaponInfo, 100, weapon);//개행 문자들을 찾고 없애서 데이터를 읽어왔을 때 데이터에 개행문자가 포함되는 경우 막기
	player->weapon->weaponInfo[strcspn(player->weapon->weaponInfo, "\n")] = '\0';
	fclose(weapon);

	found = 0;
	FILE* armor = fopen(armordata, "r");//장비 데이터 읽어오기
	if (armor == NULL) {
		printf("파일을 열 때 에러가 발생했습니다.\n");
		return;
	}

	while (fgets(line1, sizeof(line1), armor)) {
		line1[strcspn(line1, "\n")] = '\0'; // 개행 문자 제거
		//printf("%s", player->armor->armorName);
		// 문자열을 찾으면 탈출
		if (!found && strcmp(player->armor->armorName, line1) == 0) {
			found = 1; // 문자열을 찾았음을 표시
			break;
		}
	}
	if (!found) {
		printf("파일에서 '%s'와 일치하는 줄을 찾을 수 없습니다.\n", player->armor->armorName);
	}
	fscanf(armor, "%d\n", &player->armor->armorHP);
	fscanf(armor, "%d\n", &player->armor->armorATK);
	fscanf(armor, "%d\n", &player->armor->armorDEF);
	fscanf(armor, "%d\n", &player->armor->armorMP);
	fgets(player->armor->armorskill, 150, armor);//개행 문자들을 찾고 없애서 데이터를 읽어왔을 때 데이터에 개행문자가 포함되는 경우 막기
	player->armor->armorskill[strcspn(player->armor->armorskill, "\n")] = '\0';
	fgets(player->armor->armorInfo, 100, armor);//개행 문자들을 찾고 없애서 데이터를 읽어왔을 때 데이터에 개행문자가 포함되는 경우 막기
	player->armor->armorInfo[strcspn(player->armor->armorInfo, "\n")] = '\0';
	fclose(armor);
	
}

void battleRoom(Charstats* player, Enemystats* enemy) {//전투방
	readEquipments(player);
	strcpy(player->room, "fighting");
	saveGame(player);
	battle = 1;
	int turn = 1;//전투 진행 여부와 턴수 기록
	int AP_player, AP_enemy, startline = 0;//적의 현재 체력, 자신과 상대의 남은 행동력 나타내기
	char input[10];//입력값 저장
	int ATKBonus[2] = { 0,0 }, DEFBonus[2] = { 0,0 }, Poison[2] = { 0,0 }, Burn[2] = { 0,0 }, Regen[2] = { 0,0 }, Will = 0, Fear = 0;//상태 효과: 공격 보정, 방어 보정, 독, 화상, 재생, 의지, 공표
	//독: 매 턴 종료시 고정 피해 후 수치 감소, 수치 비례 방어력 감소 | 화상: 매 턴마다 고정 피해 후 수치 감소, 수치 비례 공격력 감소
	//재생: 매 턴마다 수치 비례 회복 후 수치 감소 | 의지: 피격 1회 데미지를 무효화하고 수치 1 감소 | 공포: 턴 시작시 플레이어의 행동력 1 감소
	//의지, 공포는 플레이어 전용 특수효과라 배열로 설정하지 않음
	//장비 영향을 받은 플레이어 최대 체력, 마나 설정
	int maxHP = player->HP + player->weapon->weaponHP + player->armor->armorHP;
	int maxMP = player->MP + player->weapon->weaponMP + player->armor->armorMP;
	int damage, Pshield, Eshield, Pcharge = 0, Pstack = 0, Echarge = 0, Estack = 0;//전투 시 사용하는 변수
	int randomatk, atkarr[10] = { 0,0,0,0,0,0,1,1,2,2 };//적이 랜덤으로 공격할 수 있게 하는 변수
	srand((unsigned int)time(NULL));//무작위 랜덤 사용
	//스테이지에 따라 다른 몬스터가 출현
	
	int encounter[] = { 0, 5, 10, 15 };

	if (player->stage == 1) {//몬스터 데이터 파일의 특정 줄부터 출력하는 것으로 구현
		startline = encounter[rand() % 4];//1층에 나오는 몬스터 4종(몬스터 하나당 파일의 5줄을 차지)
	}
	else if (player->stage == 2) {
		startline = encounter[(rand() % 4)]+20;//2층에 나오는 몬스터 4종
	}
	else if (player->stage == 3) {
		startline = encounter[(rand() % 4)]+40;//3층에 나오는 몬스터 4종
	}
	
	/*
	if (player->stage == 1) {//몬스터 데이터 파일의 특정 줄부터 출력하는 것으로 구현
		startline = (rand() % 4)*5+1;//1층
	}
	else if (player->stage == 2) {
		startline = (rand() % 4)*5 + 20;//2층
	}
	else if (player->stage == 3) {
		startline = (rand() % 4)*5 + 40;//3층
	}
	*/
	FILE* monster = fopen(enemydata, "r");
	if (monster == NULL) {
		printf("몬스터 데이터가 없습니다.\n");
		return;
	}
	
	char line[100];
	int currentline = 1;
	if (startline == 0) {

	}
	else {
		while (fgets(line, sizeof(line), monster)) {//위에서 정해놓은 줄부터 몬스터 데이터를 읽어온다
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
	fgets(enemy->enemyname, nameLen, monster);//개행 문자들을 찾고 없애서 데이터를 읽어왔을 때 데이터에 개행문자가 포함되는 경우 막기
	enemy->enemyname[strcspn(enemy->enemyname, "\n")] = '\0';
	fscanf(monster, "%d\n", &enemy->HP);
	fscanf(monster, "%d\n", &enemy->ATK);
	fscanf(monster, "%d\n", &enemy->DEF);
	fgets(enemy->enemyinfo, sizeof(enemy->enemyinfo), monster);//개행 문자들을 찾고 없애서 데이터를 읽어왔을 때 데이터에 개행문자가 포함되는 경우 막기
	enemy->enemyinfo[strcspn(enemy->enemyinfo, "\n")] = '\0';
	enemy->currentHP = enemy->HP;

	fclose(monster);

	_getch();
	printf("\n당신은 심상치 않은 기운이 도는 방에 들어섰다.\n");
	_getch();
	printf("전투발생!\n");
	_getch();
	printf("---------------------------------\n");
	printf("%s가 나타났다!\n\n", enemy->enemyname);
	Eshield = 0;

	while (battle == 1) {
		input[0] = '\0';
		Pshield = 0;
		printf("\n----------------TURN %d--------------\n\n", turn);
		AP_enemy = 2;
		AP_player = 2;

		if (!strcmp(player->weapon->weaponName, "마법 문양 돌 갑옷")) {
			if (player->currentHP <= maxHP / 2) {
				printf("%s마법 문양 돌 갑옷 패시브로 %s는 의지 1을 얻었다!%s\n", YELLOW, player->charname, RESET);
				Will++;
			}
		}
		printf("%s ", player->charname);
		healthBar(player->currentHP, maxHP, Pshield);
		printf("%s ", enemy->enemyname);
		healthBar(enemy->currentHP, enemy->HP, Eshield);

		while (AP_player > 0) {//플레이어 턴
			printf("\n%s는 무엇을 할까?\n", player->charname);
			printf("AP : %s%d%s | MP ", YELLOW, AP_player, RESET);
			ManaBar(player->currentMP, maxMP);
			judge(player, enemy);
			printf("|공격|방어|아이템|정보|");
			printf("---------------------------------\n입력: ");
			scanf("%s", input);
			//printf("\n%d %d %d %d\n", player->ATK, player->weapon->weaponATK, player->armor->armorATK, enemy->DEF);
			if (!strcmp(input, "공격")) {
				damage = (player->ATK + player->weapon->weaponATK + player->armor->armorATK) * (1 + 0.1 * (ATKBonus[0] - Burn[0])) - (enemy->DEF * (1 + 0.1 * (DEFBonus[1] - Poison[1])));

				if (!strcmp(player->weapon->weaponName, "조잡한 에너지 소드")) {
					if (Pstack > 0) {
						printf("%s조잡한 에너지 소드 패시브로 피해량이 증가했다!%s\n", YELLOW, RESET);
						damage *= 1.25 * Pstack;
						_getch();
						printf("%s의 공격! %d의 피해를 주었다!\n", player->charname, damage);
						printf("%s조잡한 에너지 소드 패시브로 %s는 마나 %d를 회복했다!%s\n", YELLOW, player->charname, 5 * Pstack, RESET);
						player->currentMP += 5 * Pstack;
						if (player->currentMP > maxMP) {
							player->currentMP = maxMP;
						}
						Pstack = 0;
					}
					else {
						printf("%s의 공격! %d의 피해를 주었다!\n", player->charname, damage);
					}
				}
			else if (!strcmp(player->weapon->weaponName, "빛나는 에너지 소드")) {
					if (Pstack > 0) {
						printf("%s빛나는 에너지 소드 패시브로 피해량이 증가했다!%s\n", YELLOW, RESET);
						damage *= 1.5 * Pstack;
						_getch();
						printf("%s의 공격! %d의 피해를 주었다!\n", player->charname, damage);
						Burn[1] += Pstack / 2 + 1;
						printf("%s빛나는 에너지 소드 패시브로 %s는 화상  %d에 걸렸다!%s\n", RED, enemy->enemyname, (Pstack / 2)+1, RESET);
						printf("%s빛나는 에너지 소드 패시브로 %s는 마나 %d를 회복했다!%s\n", YELLOW, player->charname, 7 * Pstack, RESET);
						player->currentMP += 7 * Pstack;
						if (player->currentMP > maxMP) {
							player->currentMP = maxMP;
						}
						Pstack = 0;
					}
					else {
						printf("%s의 공격! %d의 피해를 주었다!\n", player->charname, damage);
					}
				}
				if ((strcmp(player->weapon->weaponName, "빛나는 에너지 소드") || !strcmp(player->weapon->weaponName, "조잡한 에너지 소드")|| (strcmp(player->weapon->weaponName, "빛나는 에너지 소드"))==-1 || !strcmp(player->weapon->weaponName, "조잡한 에너지 소드")==-1))
				{

					printf("%s의 공격! %d의 피해를 주었다!\n", player->charname, damage);
				}

				Eshield -= damage;
				if (!strcmp(player->weapon->weaponName, "돌 메이스")) {
					if ((player->currentHP < player->HP) && (player->currentMP >= 5)) {
						printf("%s돌 메이스 패시브로 %s는 추가 피해 %d를 받았다!%s\n", RED, enemy->enemyname, (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0])), RESET);
						player->currentMP -= 5;
						Eshield -= (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0]));
					}
				}
				if (!strcmp(player->weapon->weaponName, "마법 문양 돌 메이스")) {
					if ((player->currentHP < player->HP) && (player->currentMP >= 5)) {
						printf("%s마법 문양 돌 메이스 패시브로 %s는 추가 피해 %d를 받았다!%s\n", RED, enemy->enemyname, (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0])), RESET);
						player->currentMP -= 5;
						Eshield -= (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0]));
						printf("%s마법 문양 돌 메이스 패시브로 %s는 화상에 걸렸다!%s\n", RED, enemy->enemyname, RESET);
						Burn[1]++;
					}
				}


				if (Eshield < 0) {
					enemy->currentHP += Eshield;
					Eshield = 0;
				}

				if (!strcmp(player->weapon->weaponName, "슬라임 배트")) {
					if (player->currentMP >= 5) {
						printf("%s슬라임 배트 패시브로 %s는 체력을 회복했다!%s\n", GREEN, player->charname, RESET);
						player->currentMP -= 5;
						player->currentHP += 7;
						if (player->currentHP > maxHP) {
							player->currentHP = maxHP;
						}
					}
				}
				if (!strcmp(player->weapon->weaponName, "슬라임 해머")) {
					if (player->currentMP >= 5) {
						printf("%s슬라임 해머 패시브로 %s는 체력을 회복했다!%s\n", GREEN, player->charname, RESET);
						player->currentMP -= 5;
						player->currentHP += 15;
						if (player->currentHP > maxHP) {
							player->currentHP = maxHP;
						}
						if (ATKBonus[0] < 2) {
							ATKBonus[0]++;
							printf("%s슬라임 해머 패시브로 %s의 공격력이 증가했다!%s\n", RED, player->charname, RESET);
						}
					}
				}
				if (!strcmp(player->weapon->weaponName, "의식용 뼈칼")) {
					if ((player->currentHP < maxHP) && (Poison[1] > 0)) {
						printf("%s의식용 뼈칼 패시브로 %s는 체력을 회복했다!%s\n", GREEN, player->charname, RESET);
						player->currentHP += 5;
						if (player->currentHP > maxHP) {
							player->currentHP = maxHP;
						}
					}
					if (player->currentMP >= 10) {
						printf("%s의식용 뼈칼 패시브로 %s는 독에 중독됐다!%s\n", RED, enemy->enemyname, RESET);
						player->currentMP -= 10;
						Poison[1] += 2;
					}
				}
				if (!strcmp(player->weapon->weaponName, "척수검")) {
					if (Poison[1] > 0) {
						if (player->currentHP < maxHP) {
							printf("%s척수검 패시브로 %s는 체력을 회복했다!%s\n", GREEN, player->charname, RESET);
							player->currentHP += 15;
							if (player->currentHP > maxHP) {
								player->currentHP = maxHP;
							}
						}
						printf("%s척수검 패시브로 %s는 고정 피해 7을 받았다!%s\n", RED, enemy->enemyname, RESET);
						enemy->currentHP -= 7;
					}
					if (player->currentMP >= 10) {
						printf("%s척수검 패시브로 %s는 독에 중독됐다!%s\n", RED, enemy->enemyname, RESET);
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
				if (Estack > 0 && (!strcmp(enemy->enemyname, "조약돌 골렘") || !strcmp(enemy->enemyname, "바위 골렘") || !strcmp(enemy->enemyname, "원소 골렘"))) {
					printf("%s는 반사 피해를 받았다!\n", player->charname);
					player->currentHP -= 5 * Estack;
					Estack -= player->stage;
				}
				//추가효과 발동

				AP_player--;
			}
			else if (!strcmp(input, "방어")) {
				Pshield += (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF);
				printf("%s는 방어 자세를 취했다!\n", player->charname);
				printf("이번 턴 동안 %s%d%s의 보호막을 추가로 얻는다!\n", BLUE, (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF), RESET);

				if (!strcmp(player->weapon->weaponName, "조잡한 에너지 소드")) {
					if (player->currentMP >= 10) {
						printf("%s조잡한 에너지 소드 패시브로 마나 10을 소모해 충전을 얻었다!%s\n", BLUE, RESET);
						player->currentMP -= 10;
						Pstack++;
					}
				}
				if (!strcmp(player->weapon->weaponName, "빛나는 에너지 소드")) {
					if (player->currentMP >= 10) {
						printf("%s빛나는 에너지 소드 패시브로 마나 10을 소모해 충전을 얻었다!%s\n", BLUE, RESET);
						player->currentMP -= 10;
						Pstack++;
					}
				}
				if (!strcmp(player->weapon->weaponName, "기계 방패")) {
					if (player->currentMP >= 5) {
						printf("%s기계 방패 패시브로 마나 5를 소모해 보호막 10을 추가로 얻었다!%s\n", BLUE, RESET);
						player->currentMP -= 5;
						Pshield += 10;
					}
				}
				if (!strcmp(player->weapon->weaponName, "고대 에너지 방패")) {
					if (player->currentMP >= 10) {
						printf("%s고대 에너지 방패 패시브로 마나 10을 소모해 보호막 20을 추가로 얻었다!%s\n", BLUE, RESET);
						player->currentMP -= 10;
						Pshield += 20;
					}
				}

				//추가효과 발동
				AP_player--;
			}
			else if (!strcmp(input, "아이템")) {
				//아이템(인벤토리 구현하고 나서 건들기)
				int temp;
				printf("---------------------------------\n");
				if (player->invenCount > 0) {
					for (int i = 0; i < player->invenCount; i++) {
						printf("%d. %s %d개\n", i + 1, player->Inventory[i]->itemName, player->Inventory[i]->quantity);
					}
					while (1) {
						printf("\n번호 입력:");
						scanf("%d", &temp);
						temp--;
						while (1) {
							if (temp >= 0 && temp < player->invenCount) {
								if (!strcmp(player->Inventory[temp]->itemName, "체력회복 물약")) {
									useItem(player, "체력회복 물약", 1);
									player->currentHP += 50;
									if (player->currentHP >= maxHP) {
										player->currentHP = maxHP;
										printf("체력을 최대치까지 회복했다!\n");
									}
									else {
										printf("체력을 50 회복했다!\n");
									}
									break;
								}
								else if (!strcmp(player->Inventory[temp]->itemName, "디버프해제 물약")) {
									useItem(player, "디버프해제 물약", 1);
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
								else if (!strcmp(player->Inventory[temp]->itemName, "마나회복 물약")) {
									useItem(player, "마나회복 물약", 1);
									player->currentMP += 20;
									if (player->currentMP >= maxMP) {
										player->currentMP = maxMP;
										printf("마나를 최대치까지 회복했다!\n");
									}
									else {
										printf("마나를 20 회복했다!\n");
									}
									break;
								}
								else if (!strcmp(player->Inventory[temp]->itemName, "재생 물약")) {
									useItem(player, "재생 물약", 1);
									player->currentHP += 25;
									if (player->currentHP >= player->HP) {
										player->currentHP = player->HP;
										printf("체력을 최대치까지 회복하고 재생 4를 얻었다!\n");
									}
									else {
										printf("체력을 25 회복하고 재생 4를 얻었다!\n");
									}
									Regen[0] = 4;
									break;
								}
								else if (!strcmp(player->Inventory[temp]->itemName, "AP 물약")) {
									useItem(player, "AP 물약", 1);
									AP_player++;
									printf("%s의 행동이 빨라져 이번 턴 추가 AP를 얻었다!\n", player->charname);
									break;

								}
								else {
									printf("이 아이템은 전투 중에 사용할 수 없습니다!\n");
									break;
								}
							}
							else {
								printf("잘못된 입력입니다.");
								break;
							}
						}
						break;
					}
				}
				else {
					printf("아이템이 없습니다.\n");
				}
			}
			else if (!strcmp(input, "정보")) {
				printf("\n플레이어 %s | ATK %.0f | DEF %.0f", player->charname, (player->ATK + player->weapon->weaponATK + player->armor->armorATK) * (1 + 0.1 * (ATKBonus[0] - Burn[0])), (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0])));
				printf("\n\n무기 : %s\n%s\n%s\n\n방어구 : %s\n%s\n%s\n\n", player->weapon->weaponName, player->weapon->weaponInfo, player->weapon->weaponskill, player->armor->armorName, player->armor->armorInfo, player->armor->armorskill);
				printf("\n적 %s\nHP %d | ATK %.0f | DEF %.0f\n%s\n\n", enemy->enemyname, enemy->HP, enemy->ATK * (1 + 0.1 * (ATKBonus[0] - Burn[0])), enemy->DEF * (1 + 0.1 * (DEFBonus[0] - Poison[0])), enemy->enemyinfo);
				}
			if (Fear > 0) {
				Fear = 0;
			}
		}
		Eshield = 0;
		printf("\n상대의 턴!\n");
		while (AP_enemy > 0) {//적 턴
			_getch();
			randomatk = atkarr[(rand() % 10)];//랜덤으로 공격을 진행함

			if (randomatk == 0 || randomatk == 2) {
				if (randomatk == 0) {
					damage = (enemy->ATK) * (1 + 0.1 * (ATKBonus[1] - Burn[1])) - (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF * (1 + 0.1 * (DEFBonus[0] - Poison[0])));
					printf("%s의 공격! %d의 피해를 주었다!\n", enemy->enemyname, damage);
					if (!strcmp(enemy->enemyname, "언데드 병사") || !strcmp(enemy->enemyname, "언데드 법사") || !strcmp(enemy->enemyname, "언데드 정예병")) {
						if (Estack > 0) {
							printf("%s는 추가 효과로 독에 중독됐다!\n", player->charname);
							Poison[0]++;
							Estack--;
						}
					}
				}
				else if (randomatk == 2) {
					damage = (enemy->ATK) * (1.5 + 0.1 * (ATKBonus[1] - Burn[1])) - (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF * (1 + 0.1 * (DEFBonus[0] - Poison[0])));
					printf("%s의 강한 공격! %d의 피해를 주었다!\n", enemy->enemyname, damage);
					if (!strcmp(enemy->enemyname, "슬라임") || !strcmp(enemy->enemyname, "중형 슬라임") || !strcmp(enemy->enemyname, "대형 슬라임")) {
						printf("%s의 방어력이 감소했다!\n", player->charname);
						DEFBonus[0]--;
					}
					else if (!strcmp(enemy->enemyname, "언데드 병사") || !strcmp(enemy->enemyname, "언데드 법사") || !strcmp(enemy->enemyname, "언데드 정예병")) {
						printf("%s는 독에 중독됐다!\n", player->charname);
						Poison[0]++;
						if (Estack > 0) {
							printf("%s는 추가 효과로 독에 중독됐다!\n", player->charname);
							Poison[0]++;
							Estack--;
						}
					}
					else if (!strcmp(enemy->enemyname, "조약돌 골렘") || !strcmp(enemy->enemyname, "바위 골렘") || !strcmp(enemy->enemyname, "원소 골렘")) {
						printf("%s는 %d만큼의 고정 피해를 추가로 입었다!\n", player->charname, player->stage * 5);
						Pshield -= player->stage * 5;
						if (Pshield < 0) {
							player->currentHP += Pshield;
							Pshield = 0;
						}
					}
					else if (!strcmp(enemy->enemyname, "고대 기계 순찰자") || !strcmp(enemy->enemyname, "고대 기계 병사") || !strcmp(enemy->enemyname, "고대 기계 수호자")) {
						if (Echarge < 0) {
							Echarge--;
							printf("%s는 충전된 에너지로 인해 화상을 입었다!\n", player->charname);
							Burn[0]++;
						}
					}
				}
				if (Will > 0) {
					printf("%s는 의지로 공격을 버텨내 피해를 받지 않았다!\n", player->charname);
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
				if (!strcmp(player->weapon->weaponName, "돌 갑옷")) {
					if ((player->currentHP < player->HP) && (player->currentMP >= 5)) {
						printf("%s돌 갑옷 패시브로 %s는 반사 피해 %d를 받았다!%s\n", RED, enemy->enemyname, (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0])), RESET);
						Eshield -= (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0]));
						if (Eshield < 0) {
							enemy->currentHP += Eshield;
							Eshield = 0;
							printf("%s ", enemy->enemyname);
							healthBar(enemy->currentHP, enemy->HP, Eshield);
						}
					}
				}
				if (!strcmp(player->weapon->weaponName, "마법 문양 돌 갑옷")) {
					if ((player->currentHP < player->HP) && (player->currentMP >= 5)) {
						printf("%s돌 갑옷 패시브로 %s는 반사 피해 %d를 받았다!%s\n", RED, enemy->enemyname, (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0])), RESET);
						Eshield -= (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0]));
						if (Eshield < 0) {
							enemy->currentHP += Eshield;
							Eshield = 0;
							printf("%s ", enemy->enemyname);
							healthBar(enemy->currentHP, enemy->HP, Eshield);
						}
					}
				}
				if (!strcmp(player->weapon->weaponName, "정교한 뼈 갑옷")) {
					if (Poison[1] > 0) {
						printf("%s정교한 뼈 갑옷 패시브로 %s는 공격력이 증가햏다!%s\n", YELLOW, player->charname, RESET);
						if (ATKBonus[0] <= 3) {
							ATKBonus[0]++;
						}
						if (player->currentMP < maxMP) {
							printf("%s정교한 뼈 갑옷 패시브로 %s는 마나를 회복했다!%s\n", GREEN, player->charname, RESET);
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
				if (!strcmp(enemy->enemyname, "슬라임") || !strcmp(enemy->enemyname, "중형 슬라임") || !strcmp(enemy->enemyname, "대형 슬라임")) {
					printf("%s의 몸이 빛나며 상처가 치유되기 시작했다!\n", enemy->enemyname);
					Regen[1] += player->stage;
				}
				else if (!strcmp(enemy->enemyname, "언데드 병사") || !strcmp(enemy->enemyname, "언데드 법사") || !strcmp(enemy->enemyname, "언데드 정예병")) {
					printf("%s는 무기에 독을 묻혔다!\n", enemy->enemyname);
					Estack++;
					printf("%s에게 공격을 받으면 %d번까지 독에 중독된다!\n", enemy->enemyname, Estack);
				}
				else if (!strcmp(enemy->enemyname, "조약돌 골렘") || !strcmp(enemy->enemyname, "바위 골렘") || !strcmp(enemy->enemyname, "원소 골렘")) {
					printf("%s는 자신에게 마법을 걸었다!\n", enemy->enemyname);
					Estack += player->stage;
					printf("%s에게 공격을 가하면 %d번까지 일정 피해를 돌려받는다!\n", enemy->enemyname, Estack);
				}
				else if (!strcmp(enemy->enemyname, "고대 기계 순찰자") || !strcmp(enemy->enemyname, "고대 기계 병사") || !strcmp(enemy->enemyname, "고대 기계 수호자")) {
					if (Echarge < player->stage) {
						printf("%s는 충전된 에너지를 이용해 자신을 강화해 공격력을 증가시켰다!\n", enemy->enemyname);
						ATKBonus[1]++;
						Echarge++;
					}
					else if (Echarge >= player->stage && Echarge < player->stage * 2) {
						printf("%s는 충전된 에너지를 이용해 자신을 강화해 공격력을 증가시켰다!\n", enemy->enemyname);
						printf("%s는 과충전으로 인해 방어력이 감소했다!\n", enemy->enemyname);
						ATKBonus[1]++;
						DEFBonus[1]--;
					}
					else {
						printf("%s는 과충전으로 인해 방어력이 감소했다!\n", enemy->enemyname);
						DEFBonus[1]--;
					}
				}
			}
			else { printf("에러 판별용"); }
			AP_enemy--;
			printf("적 남은 AP : %s%d%s \n\n", YELLOW, AP_enemy, RESET);
			judge(player, enemy);

		}

		if (Poison[0] > 0) {
			printf("%s는 독에 의한 피해를 %d 입었다!\n", player->charname, 5 * Poison[0]);
			player->currentHP -= 5 * Poison[0];
			Poison[0] -= player->stage;
			if (Poison[0] < 0) { Poison[0] = 0; printf("플레이어의 중독이 사라졌다!\n"); }
		}
		if (Poison[1] > 0) {

			printf("%s는 독에 의한 피해를 %d 입었다!\n", enemy->enemyname, 5 * Poison[1]);
			enemy->currentHP -= 5 * Poison[1];
			Poison[1] -= player->stage;
			if (Poison[1] <= 0) {
				Poison[1] = 0; printf("적의 중독이 사라졌다!\n");
			}
		}
		judge(player, enemy);
		if (Burn[0] > 0) {
			printf("%s는 화상에 의한 피해를 %d 입었다!\n", enemy->enemyname, 5 * Burn[0]);
			player->currentHP -= 5 * Burn[0];
			Burn[0] -= player->stage;
			if (Burn[0] <= 0) {
				Burn[0] = 0; printf("플레이어의 화상이 나았다!\n");
			}
		}
		if (Burn[1] > 0) {
			printf("%s는 화상에 의한 피해를 %d 입었다!\n", enemy->enemyname, 5 * Burn[1]);
			enemy->currentHP -= 5 * Burn[1];
			Burn[1] -= player->stage;
			if (Burn[1] <= 0) {
				Burn[1] = 0; printf("적의 화상이 나았다!\n");
			}
		}
		judge(player, enemy);
		if (Regen[0] > 0) {
			printf("%s는 %d의 체력을 재생했다!\n", player->charname, 5 * Regen[0]);
			player->currentHP -= 5 * Regen[0];
			Regen[0] -= player->stage;
			if (player->currentHP > maxHP) { player->currentHP = maxHP; }
			if (Regen[0] <= 0) {
				Regen[0] = 0; printf("재생 효과가 사라졌다!\n");
			}
		}
		if (Regen[1] > 0) {
			printf("%s는 %d의 체력을 재생했다!\n", enemy->enemyname, 5 * Regen[1]);
			enemy->currentHP += 5 * Regen[1];
			Regen[1] -= player->stage;
			if (enemy->currentHP > enemy->HP) { enemy->currentHP = enemy->HP; }
			if (Regen[1] <= 0) {
				Regen[1] = 0; printf("재생 효과가 사라졌다!\n");
			}
		}
		if (!strcmp(player->weapon->weaponName, "점액 보호대")) {
			if (player->currentHP < maxHP) {
				printf("%s점액 보호대 패시브로 %s는 체력을 5 회복했다!%s\n", GREEN, player->charname, RESET);
				player->currentHP += 5;
				if (player->currentHP > maxHP) {
					player->currentHP = maxHP;
				}
			}
		}
		if (!strcmp(player->weapon->weaponName, "슬라임 갑옷")) {
			if (player->currentHP < maxHP) {
				printf("%s슬라임 갑옷 패시브로 %s는 체력을 10 회복했다!%s\n", GREEN, player->charname, RESET);
				player->currentHP += 5;
				if (player->currentHP > maxHP) {
					player->currentHP = maxHP;
				}
				printf("%s슬라임 갑옷 패시브로 %s는 방어력이 증가했다!%s\n", GREEN, player->charname, RESET);
				if (DEFBonus[0] <= 3) {
					DEFBonus[0]++;
				}
			}
		}
		if (!strcmp(player->weapon->weaponName, "뼈 투구")) {
			if ((player->currentMP >= 12) && ((ATKBonus[0] < 0) || (DEFBonus[0] < 0) || (Burn[0] > 0) || (Poison[0] > 0))) {
				printf("%s뼈 투구 패시브로 %s의 상태 이상이 사라졌다!%s\n", GREEN, player->charname, RESET);
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
		if (!strcmp(player->weapon->weaponName, "정교한 뼈 갑옷")) {
			if ((player->currentMP >= 12) && ((ATKBonus[0] < 0) || (DEFBonus[0] < 0) || (Burn[0] > 0) || (Poison[0] > 0))) {
				printf("%s정교한 뼈 갑옷 패시브로 %s의 상태 이상이 사라졌다!%s\n", GREEN, player->charname, RESET);
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
		if (!strcmp(player->weapon->weaponName, "고대 에너지 방패")) {
			if (Pshield > 0) {
				printf("%s고대 에너지 방패 패시브로 %s는 마나 %d를 회복했다! %s\n", GREEN, player->charname, Pshield / 2, RESET);
				player->currentMP += Pshield / 2;
			}
		}
		if (!strcmp(player->weapon->weaponName, "기계 방패")) {
			if (Pshield > 0) {
				printf("%s고대 에너지 방패 패시브로 %s는 마나 %d를 회복했다! %s\n", GREEN, player->charname, Pshield / 2, RESET);
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

void bossRoom(Charstats* player, Enemystats* enemy) {//보스방
	strcpy(player->room, "boss");
	readEquipments(player);
	saveGame(player);
	battle = 1;
	int turn = 1;//전투 진행 여부와 턴수 기록
	int AP_player, AP_enemy, startline = 0;//적의 현재 체력, 자신과 상대의 남은 행동력 나타내기
	char input[10];//입력값 저장
	int ATKBonus[2] = { 0,0 }, DEFBonus[2] = { 0,0 }, Poison[2] = { 0,0 }, Burn[2] = { 0,0 }, Regen[2] = { 0,0 }, Will = 0, Fear = 0;//상태 효과: 공격 보정, 방어 보정, 독, 화상, 재생, 의지, 감속
	//독: 매 턴 종료시 고정 피해 후 수치 감소, 수치 비례 방어력 감소 | 화상: 매 턴마다 고정 피해 후 수치 감소, 수치 비례 공격력 감소
	//재생: 매 턴마다 수치 비례 회복 후 수치 감소 | 의지: 피격 1회 데미지를 무효화하고 수치 1 감소 | 공포: 턴 시작시 플레이어의 행동력 1 감소
	//의지, 공포는 플레이어 전용 특수효과라 배열로 설정하지 않음
	//장비 영향을 받은 플레이어 최대 체력, 마나 설정
	int maxHP = player->HP + player->weapon->weaponHP + player->armor->armorHP;
	int maxMP = player->MP + player->weapon->weaponMP + player->armor->armorMP;
	int damage, Pshield, Eshield, Pcharge = 0, Pstack = 0, Echarge = 0, Estack = 0;//전투 시 사용하는 변수
	int randomatk, atkarr[10] = { 0,0,0,0,0,1,1,1,2,2 };//적이 랜덤으로 공격할 수 있게 하는 변수(보스는 특수기를 좀 더 자주 쓴다)
	srand((unsigned int)time(NULL));//무작위 랜덤 사용
	/*
	int encounter[] = { 1, 5 };

	if (player->stage == 1) {//몬스터 데이터 파일의 특정 줄부터 출력하는 것으로 구현
		startline = encounter[rand() % 2];//1층에 나오는 보스 2종
	}
	else if (player->stage == 2) {
		startline = encounter[(rand() % 2)]+10;//2층에 나오는 보스 2종
	}
	else if (player->stage == 3) {
		startline = encounter[(rand() % 2)]+20;//3층에 나오는 보스 2종
	}
	*/

	if (player->stage == 1) {//몬스터 데이터 파일의 특정 줄부터 출력하는 것으로 구현
		startline = 5 * (rand() % 2);//1층에 나오는 보스 2종
	}
	else if (player->stage == 2) {
		startline = 5 * (rand() % 2) + 10;//2층에 나오는 보스 2종
	}
	else if (player->stage == 3) {
		startline = 5 * (rand() % 2) + 20;//3층에 나오는 보스 2종
	}

	FILE* monster = fopen(bossdata, "r");
	if (monster == NULL) {
		printf("몬스터 데이터가 없습니다.\n");
		return;
	}

	char line[100];
	int currentline = 1;
	if (startline == 0) {

	}
	else {
		while (fgets(line, sizeof(line), monster)) {//위에서 정해놓은 줄부터 몬스터 데이터를 읽어온다
			if (currentline == startline) {
				break;
			}
			else {
				line[0] = '\0';
				currentline++;
			}
		}
	}

	fgets(enemy->enemyname, nameLen, monster);//개행 문자들을 찾고 없애서 데이터를 읽어왔을 때 데이터에 개행문자가 포함되는 경우 막기
	enemy->enemyname[strcspn(enemy->enemyname, "\n")] = '\0';
	fscanf(monster, "%d\n", &enemy->HP);
	fscanf(monster, "%d\n", &enemy->ATK);
	fscanf(monster, "%d\n", &enemy->DEF);
	fgets(enemy->enemyinfo, sizeof(enemy->enemyinfo), monster);//개행 문자들을 찾고 없애서 데이터를 읽어왔을 때 데이터에 개행문자가 포함되는 경우 막기
	enemy->enemyinfo[strcspn(enemy->enemyinfo, "\n")] = '\0';
	enemy->currentHP = enemy->HP;

	fclose(monster);

	_getch();
	printf("\n당신은 스테이지의 마지막 방에 들어섰다.\n");
	_getch();
	printf("전투발생!\n");
	_getch();
	printf("---------------------------------\n");
	printf("보스 %s가 나타났다!\n\n", enemy->enemyname);
	if (!strcmp(enemy->enemyname, "던전의 지배자의 최강 부하")) {
		Eshield = 100;
	}
	else if (!strcmp(enemy->enemyname, "던전의 지배자")) {
		Eshield = 400;
	}
	else {
		Eshield = 0;
	}
	if ((!strcmp(enemy->enemyname, "악룡 [둘리]") && Estack > 0)) {
		Estack = 10;
	}
	while (battle == 1) {
		input[0] = '\0';
		Pshield = 0;

		printf("\n----------------TURN %d--------------\n\n", turn);
		if ((!strcmp(enemy->enemyname, "던전의 지배자")) && Eshield > 0) {
			printf("던전의 지배자의 보호막을 소진시키기 전까지 던전의 지배자의 공격력이 대폭 증가합니다!\n");
			if (ATKBonus[1] <= 8) {
				ATKBonus[1] = 8;
			}
			DEFBonus[1] = 0;
		}if ((!strcmp(enemy->enemyname, "던전의 지배자")) && Eshield == 0) {
			printf("던전의 지배자의 보호막이 소진되어 던전의 지배자의 방어력이 대폭 감소합니다!\n");
			if (DEFBonus[1] >= 5) {
				DEFBonus[1] = -5;
			}
			ATKBonus[1] = 0;
		}
		if ((!strcmp(enemy->enemyname, "악룡 [둘리]") && Estack > 0)) {
			printf("둘리의 에너지를 소진시키기 전까지 둘리의 공격을 맞으면 강력한 화상에 걸립니다!\n");
			printf("방어 스킬로 공격을 완벽하게 막아 둘리의 에너지를 소진시키세요!!\n");
			_getch();
		}
		AP_enemy = 3;
		if (Fear > 0) {
			printf("%s는 사기가 떨어져 이번 턴 행동력이 감소한다!\n", player->charname);
			AP_player = 1;
		}
		else {
			AP_player = 2;
		}
		if (!strcmp(player->weapon->weaponName, "마법 문양 돌 갑옷")) {
			if (player->currentHP <= maxHP / 2) {
				printf("%s마법 문양 돌 갑옷 패시브로 %s는 의지 1을 얻었다!%s\n", YELLOW, player->charname, RESET);
				Will++;
			}
		}
		printf("%s ", player->charname);
		healthBar(player->currentHP, maxHP, Pshield);
		printf("%s ", enemy->enemyname);
		healthBar(enemy->currentHP, enemy->HP, Eshield);
		if ((!strcmp(enemy->enemyname, "악룡 [둘리]") && Estack > 0)) {
			printf("남은 에너지 :%d\n", Estack);
		}

		while (AP_player > 0) {//플레이어 턴
			printf("\n%s는 무엇을 할까?\n", player->charname);
			printf("AP : %s%d%s | MP ", YELLOW, AP_player, RESET);
			ManaBar(player->currentMP, maxMP);
			bossjudge(player, enemy);
			printf("|공격|방어|아이템|정보|");
			printf("---------------------------------\n입력: ");
			scanf("%s", input);

			if (!strcmp(input, "공격")) {
				damage = (player->ATK + player->weapon->weaponATK + player->armor->armorATK) * (1 + 0.1 * (ATKBonus[0] - Burn[0])) - (enemy->DEF * (1 + 0.1 * (DEFBonus[1] - Poison[1])));

				if (!strcmp(player->weapon->weaponName, "조잡한 에너지 소드")) {
					if (Pstack > 0) {
						printf("%s조잡한 에너지 소드 패시브로 피해량이 증가했다!%s\n", YELLOW, RESET);
						damage *= 1.25 * Pstack;
						_getch();
						printf("%s의 공격! %d의 피해를 주었다!\n", player->charname, damage);
						printf("%s조잡한 에너지 소드 패시브로 %s는 마나 %d를 회복했다!%s\n", YELLOW, player->charname, 5 * Pstack, RESET);
						player->currentMP += 5 * Pstack;
						if (player->currentMP > maxMP) {
							player->currentMP = maxMP;
						}
						Pstack = 0;
					}
					else {
						printf("%s의 공격! %d의 피해를 주었다!\n", player->charname, damage);
					}
				}
			else if (!strcmp(player->weapon->weaponName, "빛나는 에너지 소드")) {
					if (Pstack > 0) {
						printf("%s빛나는 에너지 소드 패시브로 피해량이 증가했다!%s\n", YELLOW, RESET);
						damage *= 1.5 * Pstack;
						_getch();
						printf("%s의 공격! %d의 피해를 주었다!\n", player->charname, damage);
						Burn[1] += Pstack / 2 + 1;
						printf("%s빛나는 에너지 소드 패시브로 %s는 화상  %d에 걸렸다!%s\n", RED, enemy->enemyname, Pstack / 2, RESET);
						printf("%s빛나는 에너지 소드 패시브로 %s는 마나 %d를 회복했다!%s\n", YELLOW, player->charname, 7 * Pstack, RESET);
						player->currentMP += 7 * Pstack;
						if (player->currentMP > maxMP) {
							player->currentMP = maxMP;
						}
						Pstack = 0;
					}
					else {
						printf("%s의 공격! %d의 피해를 주었다!\n", player->charname, damage);
					}
				}
				if ((strcmp(player->weapon->weaponName, "빛나는 에너지 소드") || !strcmp(player->weapon->weaponName, "조잡한 에너지 소드") || (strcmp(player->weapon->weaponName, "빛나는 에너지 소드")) == -1 || !strcmp(player->weapon->weaponName, "조잡한 에너지 소드") == -1))
				{

					printf("%s의 공격! %d의 피해를 주었다!\n", player->charname, damage);
				}

				Eshield -= damage;

				if (!strcmp(player->weapon->weaponName, "돌 메이스")) {
					if ((player->currentHP < player->HP) && (player->currentMP >= 5)) {
						printf("%s돌 메이스 패시브로 %s는 추가 피해 %d를 받았다!%s\n", RED, enemy->enemyname, (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0])), RESET);
						player->currentMP -= 5;
						Eshield -= (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0]));
					}
				}
				if (!strcmp(player->weapon->weaponName, "마법 문양 돌 메이스")) {
					if ((player->currentHP < player->HP) && (player->currentMP >= 5)) {
						printf("%s마법 문양 돌 메이스 패시브로 %s는 추가 피해 %d를 받았다!%s\n", RED, enemy->enemyname, (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0])), RESET);
						player->currentMP -= 5;
						Eshield -= (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0]));
						printf("%s마법 문양 돌 메이스 패시브로 %s는 화상에 걸렸다!%s\n", RED, enemy->enemyname, RESET);
						Burn[1]++;
					}
				}


				if (Eshield < 0) {
					enemy->currentHP += Eshield;
					Eshield = 0;
				}

				if (!strcmp(player->weapon->weaponName, "슬라임 배트")) {
					if (player->currentMP >= 5) {
						printf("%s슬라임 배트 패시브로 %s는 체력을 회복했다!%s\n", GREEN, player->charname, RESET);
						player->currentMP -= 5;
						player->currentHP += 7;
						if (player->currentHP > maxHP) {
							player->currentHP = maxHP;
						}
					}
				}
				if (!strcmp(player->weapon->weaponName, "슬라임 해머")) {
					if (player->currentMP >= 5) {
						printf("%s슬라임 해머 패시브로 %s는 체력을 회복했다!%s\n", GREEN, player->charname, RESET);
						player->currentMP -= 5;
						player->currentHP += 15;
						if (player->currentHP > maxHP) {
							player->currentHP = maxHP;
						}
						if (ATKBonus[0] < 2) {
							ATKBonus[0]++;
							printf("%s슬라임 해머 패시브로 %s의 공격력이 증가했다!%s\n", RED, player->charname, RESET);
						}
					}
				}
				if (!strcmp(player->weapon->weaponName, "의식용 뼈칼")) {
					if ((player->currentHP < maxHP) && (Poison[1] > 0)) {
						printf("%s의식용 뼈칼 패시브로 %s는 체력을 회복했다!%s\n", GREEN, player->charname, RESET);
						player->currentHP += 5;
						if (player->currentHP > maxHP) {
							player->currentHP = maxHP;
						}
					}
					if (player->currentMP >= 10) {
						printf("%s의식용 뼈칼 패시브로 %s는 독에 중독됐다!%s\n", RED, enemy->enemyname, RESET);
						player->currentMP -= 10;
						Poison[1] += 2;
					}
				}
				if (!strcmp(player->weapon->weaponName, "척수검")) {
					if (Poison[1] > 0) {
						if (player->currentHP < maxHP) {
							printf("%s척수검 패시브로 %s는 체력을 회복했다!%s\n", GREEN, player->charname, RESET);
							player->currentHP += 15;
							if (player->currentHP > maxHP) {
								player->currentHP = maxHP;
							}
						}
						printf("%s척수검 패시브로 %s는 고정 피해 7을 받았다!%s\n", RED, enemy->enemyname, RESET);
						enemy->currentHP -= 7;
					}
					if (player->currentMP >= 10) {
						printf("%s척수검 패시브로 %s는 독에 중독됐다!%s\n", RED, enemy->enemyname, RESET);
						player->currentMP -= 10;
						Poison[1] += 4;
					}
				}

				if (enemy->currentHP <= 0) {
					enemy->currentHP = 0;
				}
				printf("%s ", enemy->enemyname);
				healthBar(enemy->currentHP, enemy->HP, Eshield);
				if ((!strcmp(enemy->enemyname, "악룡 [둘리]") && Estack > 0)) {
					printf("남은 에너지 :%d\n", Estack);
				}
				bossjudge(player, enemy);

				//추가효과 발동

				AP_player--;
			}
			else if (!strcmp(input, "방어")) {
				Pshield += (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF);
				printf("%s는 방어 자세를 취했다!\n", player->charname);
				printf("이번 턴 동안 %s%d%s의 보호막을 추가로 얻는다!\n", BLUE, (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF), RESET);

				if (!strcmp(player->weapon->weaponName, "조잡한 에너지 소드")) {
					if (player->currentMP >= 10) {
						printf("%s조잡한 에너지 소드 패시브로 마나 10을 소모해 충전을 얻었다!%s\n", BLUE, RESET);
						player->currentMP -= 10;
						Pstack++;
					}
				}
				if (!strcmp(player->weapon->weaponName, "빛나는 에너지 소드")) {
					if (player->currentMP >= 10) {
						printf("%s빛나는 에너지 소드 패시브로 마나 10을 소모해 충전을 얻었다!%s\n", BLUE, RESET);
						player->currentMP -= 10;
						Pstack++;
					}
				}
				if (!strcmp(player->weapon->weaponName, "기계 방패")) {
					if (player->currentMP >= 5) {
						printf("%s기계 방패 패시브로 마나 5를 소모해 보호막 10을 추가로 얻었다!%s\n", BLUE, RESET);
						player->currentMP -= 5;
						Pshield += 10;
					}
				}
				if (!strcmp(player->weapon->weaponName, "고대 에너지 방패")) {
					if (player->currentMP >= 10) {
						printf("%s고대 에너지 방패 패시브로 마나 10을 소모해 보호막 20을 추가로 얻었다!%s\n", BLUE, RESET);
						player->currentMP -= 10;
						Pshield += 20;
					}
				}

				//추가효과 발동
				AP_player--;
			}
			else if (!strcmp(input, "아이템")) {
				//아이템(인벤토리 구현하고 나서 건들기)
				int temp;
				printf("---------------------------------\n");
				if (player->invenCount > 0) {
					for (int i = 0; i < player->invenCount; i++) {
						printf("%d. %s %d개\n", i + 1, player->Inventory[i]->itemName, player->Inventory[i]->quantity);
					}
					while (1) {
						printf("\n번호 입력:");
						scanf("%d", &temp);
						temp--;
						while (1) {
							if (temp >= 0 && temp < player->invenCount) {
								if (!strcmp(player->Inventory[temp]->itemName, "체력회복 물약")) {
									useItem(player, "체력회복 물약", 1);
									player->currentHP += 50;
									if (player->currentHP >= maxHP) {
										player->currentHP = maxHP;
										printf("체력을 최대치까지 회복했다!\n");
									}
									else {
										printf("체력을 50 회복했다!\n");
									}
									break;
								}
								else if (!strcmp(player->Inventory[temp]->itemName, "디버프해제 물약")) {
									useItem(player, "디버프해제 물약", 1);
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
								else if (!strcmp(player->Inventory[temp]->itemName, "마나회복 물약")) {
									useItem(player, "마나회복 물약", 1);
									player->currentMP += 20;
									if (player->currentMP >= maxMP) {
										player->currentMP = maxMP;
										printf("마나를 최대치까지 회복했다!\n");
									}
									else {
										printf("마나를 20 회복했다!\n");
									}
									break;
								}
								else if (!strcmp(player->Inventory[temp]->itemName, "재생 물약")) {
									useItem(player, "재생 물약", 1);
									player->currentHP += 25;
									if (player->currentHP >= player->HP) {
										player->currentHP = player->HP;
										printf("체력을 최대치까지 회복하고 재생 4를 얻었다!\n");
									}
									else {
										printf("체력을 25 회복하고 재생 4를 얻었다!\n");
									}
									Regen[0] = 4;
									break;
								}
								else if (!strcmp(player->Inventory[temp]->itemName, "AP 물약")) {
									useItem(player, "AP 물약", 1);
									AP_player++;
									printf("%s의 행동이 빨라져 이번 턴 추가 AP를 얻었다!\n", player->charname);
									break;

								}
								else {
									printf("이 아이템은 전투 중에 사용할 수 없습니다!\n");
									break;
								}
							}
							else {
								printf("잘못된 입력입니다.");
								break;
							}
						}
						break;
					}
				}
				else {
					printf("아이템이 없습니다.\n");
				}
			}
			else if (!strcmp(input, "정보")) {
				printf("\n플레이어 %s | ATK %.0f | DEF %.0f", player->charname, (player->ATK + player->weapon->weaponATK + player->armor->armorATK) * (1 + 0.1 * (ATKBonus[0] - Burn[0])), (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0])));
				printf("\n\n무기 : %s\n%s\n%s\n\n방어구 : %s\n%s\n%s\n\n", player->weapon->weaponName, player->weapon->weaponInfo, player->weapon->weaponskill, player->armor->armorName, player->armor->armorInfo, player->armor->armorskill);
				printf("\n적 %s\nHP %d | ATK %.0f | DEF %.0f\n%s\n\n", enemy->enemyname, enemy->HP, enemy->ATK * (1 + 0.1 * (ATKBonus[1] - Burn[1])), enemy->DEF * (1 + 0.1 * (DEFBonus[1] - Poison[1])), enemy->enemyinfo);
			}
		}
		if (Fear > 0) {
			Fear = 0;
		}
		if (strcmp(enemy->enemyname, "던전의 지배자의 최강 부하") && strcmp(enemy->enemyname, "던전의 지배자")) {
			Eshield = 0;
		}
		printf("\n상대의 턴!\n");
		while (AP_enemy > 0) {//적 턴
			_getch();
			randomatk = atkarr[(rand() % 10)];//랜덤으로 공격을 진행함

			if (randomatk == 0 || randomatk == 2) {
				if (randomatk == 0) {
					damage = (enemy->ATK) * (1 + 0.1 * (ATKBonus[1] - Burn[1])) - (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF * (1 + 0.1 * (DEFBonus[0] - Poison[0])));
					printf("%s의 공격! %d의 피해를 주었다!\n", enemy->enemyname, damage);

				}
				else if (randomatk == 2) {
					damage = (enemy->ATK) * (1.5 + 0.1 * (ATKBonus[1] - Burn[1])) - (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF * (1 + 0.1 * (DEFBonus[0] - Poison[0])));
					printf("%s의 강한 공격! %d의 피해를 주었다!\n", enemy->enemyname, damage);
					if (!strcmp(enemy->enemyname, "악룡[둘리]") || (!strcmp(enemy->enemyname, "유령 기사"))) {
						if (Fear == 0) {
							printf("%s는 강한 공격으로 사기가 떨어졌다!\n", player->charname);
							Fear++;
						}
					}
					else if (!strcmp(enemy->enemyname, "던전의 지배자")) {
						printf("%s가 비틀거리는 틈에 던전의 지배자는 보호막을 10 복구했다!\n", player->charname);
						Eshield += 10;
					}
				}
				if (Will > 0) {
					printf("%s는 의지로 공격을 버텨내 피해를 받지 않았다!\n", player->charname);
					Will--;
					damage = 0;
				}
				else {
					Pshield -= damage;
					if ((!strcmp(enemy->enemyname, "악룡 [둘리]") && Estack > 0)) {
						if (Pshield > 0) {
							printf("둘리의 에너지 감소!\n");
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
				if (!strcmp(player->weapon->weaponName, "돌 갑옷")) {
					if ((player->currentHP < player->HP) && (player->currentMP >= 5)) {
						printf("%s돌 갑옷 패시브로 %s는 반사 피해 %d를 받았다!%s\n", RED, enemy->enemyname, (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0])), RESET);
						Eshield -= (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0]));
						if (Eshield < 0) {
							enemy->currentHP += Eshield;
							Eshield = 0;
							printf("%s ", enemy->enemyname);
							healthBar(enemy->currentHP, enemy->HP, Eshield);
						}
					}
				}
				if (!strcmp(player->weapon->weaponName, "마법 문양 돌 갑옷")) {
					if ((player->currentHP < player->HP) && (player->currentMP >= 5)) {
						printf("%s돌 갑옷 패시브로 %s는 반사 피해 %d를 받았다!%s\n", RED, enemy->enemyname, (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0])), RESET);
						Eshield -= (player->DEF + player->weapon->weaponDEF + player->armor->armorDEF) * (1 + 0.1 * (DEFBonus[0] - Poison[0]));
						if (Eshield < 0) {
							enemy->currentHP += Eshield;
							Eshield = 0;
							printf("%s ", enemy->enemyname);
							healthBar(enemy->currentHP, enemy->HP, Eshield);
						}
					}
				}
				if (!strcmp(player->weapon->weaponName, "정교한 뼈 갑옷")) {
					if (Poison[1] > 0) {
						printf("%s정교한 뼈 갑옷 패시브로 %s는 공격력이 증가햏다!%s\n", YELLOW, player->charname, RESET);
						if (ATKBonus[0] <= 3) {
							ATKBonus[0]++;
						}
						if (player->currentMP < maxMP) {
							printf("%s정교한 뼈 갑옷 패시브로 %s는 마나를 회복했다!%s\n", GREEN, player->charname, RESET);
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
				if (!strcmp(enemy->enemyname, "고블린 전사") || (!strcmp(enemy->enemyname, "유령 기사"))) {
					printf("%s는 방어 태세를 취하고 보호막을 얻었다!\n", enemy->enemyname);
					Eshield += enemy->DEF * (1 + 0.1 * (DEFBonus[0] - Poison[0]));
				}
				else if (!strcmp(enemy->enemyname, "고블린 샤먼")) {
					printf("%s는 정신을 집중했다!\n", enemy->enemyname);
					Estack++;
					printf("%d회 더 집중한 후 강력한 공격을 사용한다!\n", 3 - Estack);
					if (Estack == 3) {
						printf("%s는 거대한 화염을 소환해 %s를 불태웠다! 5의 화상을 추가로 입었다!\n", enemy->enemyname, player->charname);
						Burn[0] += 5;
						Estack = 0;
					}
				}
				else if (!strcmp(enemy->enemyname, "던전의 지배자")) {
					if (Eshield > 0) {
						printf("%s는 %s를 밀쳐내고 보호막을 전개했다! \n", enemy->enemyname, player->charname);
						Eshield += 30;
					}
					else {
						printf("%s는 정신을 집중했다!\n", enemy->enemyname);
						Estack++;
						if (Estack == 6) {
							printf("%s는 보호막을 200 회복했다! 그것을 목격한 %s는 사기가 떨어졌다...\n", enemy->enemyname, player->charname);
							Eshield = 200;
							Fear = 1;
							Estack = 0;
						}
						else {
							printf("%d회 더 집중한 후 보호막을 200 회복한다!\n", 6 - Estack);
						}
					}

				}
				else if (!strcmp(enemy->enemyname, "악룡[둘리]")) {
					if (Estack > 0) {
						printf("%s는 고열의 에너지로 %s를 불태웠다! %d의 화상을 추가로 입었다!\n", enemy->enemyname, player->charname, Estack / 2);
						Burn[0] += Estack / 2;
					}
					else if (Estack == 0) {
						printf("%s는 꼬리를 휘둘러 %s의 자세를 흐트러트렸다!\n%s의 방어력이 감소했다!\n", enemy->enemyname, player->charname, player->charname);
						DEFBonus[0]--;
					}
				}
			}
			else { printf("에러 판별용"); }
			AP_enemy--;
			printf("적 남은 AP : %s%d%s \n\n", YELLOW, AP_enemy, RESET);
			bossjudge(player, enemy);

		}

		if (Poison[0] > 0) {
			printf("%s는 독에 의한 피해를 %d 입었다!\n", player->charname, 5 * Poison[0]);
			player->currentHP -= 5 * Poison[0];
			Poison[0] -= player->stage;
			if (Poison[0] < 0) { Poison[0] = 0; printf("플레이어의 중독이 사라졌다!\n"); }
		}
		if (Poison[1] > 0) {

			printf("%s는 독에 의한 피해를 %d 입었다!\n", enemy->enemyname, 5 * Poison[1]);
			enemy->currentHP -= 5 * Poison[1];
			Poison[1] -= player->stage;
			if (Poison[1] <= 0) {
				Poison[1] = 0; printf("적의 중독이 사라졌다!\n");
			}
		}
		bossjudge(player, enemy);
		if (Burn[0] > 0) {
			printf("%s는 화상에 의한 피해를 %d 입었다!\n", enemy->enemyname, 5 * Burn[0]);
			player->currentHP -= 5 * Burn[0];
			Burn[0] -= player->stage;
			if (Burn[0] <= 0) {
				Burn[0] = 0; printf("플레이어의 화상이 나았다!\n");
			}
		}
		if (Burn[1] > 0) {
			printf("%s는 화상에 의한 피해를 %d 입었다!\n", enemy->enemyname, 5 * Burn[1]);
			enemy->currentHP -= 5 * Burn[1];
			Burn[1] -= player->stage;
			if (Burn[1] <= 0) {
				Burn[1] = 0; printf("적의 화상이 나았다!\n");
			}
		}
		bossjudge(player, enemy);
		if (Regen[0] > 0) {
			printf("%s는 %d의 체력을 재생했다!\n", player->charname, 5 * Regen[0]);
			player->currentHP -= 5 * Regen[0];
			Regen[0] -= player->stage;
			if (player->currentHP > maxHP) { player->currentHP = maxHP; }
			if (Regen[0] <= 0) {
				Regen[0] = 0; printf("재생 효과가 사라졌다!\n");
			}
		}
		if (Regen[1] > 0) {
			printf("%s는 %d의 체력을 재생했다!\n", enemy->enemyname, 5 * Regen[1]);
			enemy->currentHP += 5 * Regen[1];
			Regen[1] -= player->stage;
			if (enemy->currentHP > enemy->HP) { enemy->currentHP = enemy->HP; }
			if (Regen[1] <= 0) {
				Regen[1] = 0; printf("재생 효과가 사라졌다!\n");
			}
		}
		if (!strcmp(player->weapon->weaponName, "점액 보호대")) {
			if (player->currentHP < maxHP) {
				printf("%s점액 보호대 패시브로 %s는 체력을 5 회복했다!%s\n", GREEN, player->charname, RESET);
				player->currentHP += 5;
				if (player->currentHP > maxHP) {
					player->currentHP = maxHP;
				}
			}
		}
		if (!strcmp(player->weapon->weaponName, "슬라임 갑옷")) {
			if (player->currentHP < maxHP) {
				printf("%s슬라임 갑옷 패시브로 %s는 체력을 10 회복했다!%s\n", GREEN, player->charname, RESET);
				player->currentHP += 5;
				if (player->currentHP > maxHP) {
					player->currentHP = maxHP;
				}
				printf("%s슬라임 갑옷 패시브로 %s는 방어력이 증가했다!%s\n", GREEN, player->charname, RESET);
				if (DEFBonus[0] <= 3) {
					DEFBonus[0]++;
				}
			}
		}
		if (!strcmp(player->weapon->weaponName, "뼈 투구")) {
			if ((player->currentMP >= 12) && ((ATKBonus[0] < 0) || (DEFBonus[0] < 0) || (Burn[0] > 0) || (Poison[0] > 0))) {
				printf("%s뼈 투구 패시브로 %s의 상태 이상이 사라졌다!%s\n", GREEN, player->charname, RESET);
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
		if (!strcmp(player->weapon->weaponName, "정교한 뼈 갑옷")) {
			if ((player->currentMP >= 12) && ((ATKBonus[0] < 0) || (DEFBonus[0] < 0) || (Burn[0] > 0) || (Poison[0] > 0))) {
				printf("%s정교한 뼈 갑옷 패시브로 %s의 상태 이상이 사라졌다!%s\n", GREEN, player->charname, RESET);
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
		if (!strcmp(player->weapon->weaponName, "고대 에너지 방패")) {
			if (Pshield > 0) {
				printf("%s고대 에너지 방패 패시브로 %s는 마나 %d를 회복했다! %s\n", GREEN, player->charname, Pshield / 2, RESET);
				player->currentMP += Pshield / 2;
			}
		}
		if (!strcmp(player->weapon->weaponName, "기계 방패")) {
			if (Pshield > 0) {
				printf("%s고대 에너지 방패 패시브로 %s는 마나 %d를 회복했다! %s\n", GREEN, player->charname, Pshield / 2, RESET);
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
		printf("\n승리했다!\n");
		printf("%s는 %s%d 골드%s를 얻었다.\n", player->charname, YELLOW, player->stage * 20, RESET);
		player->gold += player->stage * 20;
		if (!strcmp(enemy->enemyname, "슬라임") || !strcmp(enemy->enemyname, "중형 슬라임") || !strcmp(enemy->enemyname, "대형 슬라임")) {
			if (player->stage == 1) {
				printf("%s슬라임 점액질%s을 1개 얻었다.\n", BLUE, RESET);
				addItem(player, "슬라임 점액질", 1);
			}
			else if (player->stage == 2) {
				printf("%s슬라임 핵%s을 1개 얻었다.\n", BLUE, RESET);
				addItem(player, "슬라임 핵", 1);
			}
			else if (player->stage == 3) {
				printf("%s슬라임 점액질%s과 %s슬라임 핵%s을 1개씩 얻었다.\n", BLUE, RESET, BLUE, RESET);
				addItem(player, "슬라임 점액질", 1);
				addItem(player, "슬라임 핵", 1);
			}
		}
		else if (!strcmp(enemy->enemyname, "언데드 병사") || !strcmp(enemy->enemyname, "언데드 법사") || !strcmp(enemy->enemyname, "언데드 정예병")) {
			if (player->stage == 1) {
				printf("%s언데드 뼛조각%s을 1개 얻었다.\n", BLUE, RESET);
				addItem(player, "언데드 뼛조각", 1);
			}
			else if (player->stage == 2) {
				printf("%s저주가 깃든 뼈%s를 1개 얻었다.\n", BLUE, RESET);
				addItem(player, "저주가 깃든 뼈", 1);
			}
			else if (player->stage == 3) {
				printf("%s언데드 뼛조각%s과 %s저주가 깃든 뼈%s를 1개씩 얻었다.\n", BLUE, RESET, BLUE, RESET);
				addItem(player, "언데드 뼛조각", 1);
				addItem(player, "저주가 깃든 뼈", 1);
			}
		}
		else if (!strcmp(enemy->enemyname, "조약돌 골렘") || !strcmp(enemy->enemyname, "바위 골렘") || !strcmp(enemy->enemyname, "원소 골렘")) {
			if (player->stage == 1) {
				printf("%s문양이 새겨진 돌%s을 1개 얻었다.\n", BLUE, RESET);
				addItem(player, "문양이 새겨진 돌", 1);
			}
			else if (player->stage == 2) {
				printf("%s마법이 흐르는 돌%s을 1개 얻었다.\n", BLUE, RESET);
				addItem(player, "마법이 흐르는 돌", 1);
			}
			else if (player->stage == 3) {
				printf("%s문양이 새겨진 돌%s과 %s마법이 흐르는 돌%s을 1개씩 얻었다.\n", BLUE, RESET, BLUE, RESET);
				addItem(player, "문양이 새겨진 돌", 1);
				addItem(player, "마법이 흐르는 돌", 1);
			}
		}
		else if (!strcmp(enemy->enemyname, "고대 기계 순찰자") || !strcmp(enemy->enemyname, "고대 기계 병사") || !strcmp(enemy->enemyname, "고대 기계 수호자")) {
			if (player->stage == 1) {
				printf("%s고대 톱니바퀴%s을 1개 얻었다.\n", BLUE, RESET);
				addItem(player, "고대 톱니바퀴", 1);
			}
			else if (player->stage == 2) {
				printf("%s고대 엔진%s을 1개 얻었다.\n", BLUE, RESET);
				addItem(player, "고대 엔진", 1);
			}
			else if (player->stage == 3) {
				printf("%s고대 톱니바퀴%s과 %s고대 엔진%s을 1개씩 얻었다.\n", BLUE, RESET, BLUE, RESET);
				addItem(player, "고대 톱니바퀴", 1);
				addItem(player, "고대 엔진", 1);
			}
		}
		player->battleCount++;
		//층에 따라 다른 골드, 아이템 얻고 준비 방으로 넘어감
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
		printf("\n승리했다!\n");
		_getch();
		printf("%s는 %s%d 골드%s를 얻었다.\n", player->charname, YELLOW, player->stage * 100, RESET);
		player->gold += player->stage * 20;
			if (player->stage == 1) {
				printf("%s%s의 레벨이 올라 기초 능력치가 상승했다!%s\n\n", BLUE, player->charname, RESET);
				_getch();
				printf("기초 HP  | 100 -> 150\n");
				printf("기초 ATK | 20 -> 30\n");
				printf("기초 DEF | 10 -> 15\n");
				printf("기초 MP  | 10 -> 15\n");
				player->HP = 150;
				player->ATK = 35;
				player->DEF = 17;
				player->MP = 20;
				_getch();
				player->stage++;
				startingRoom(player, enemy);
			}
			else if (player->stage == 2) {
				printf("%s%s의 레벨이 올라 기초 능력치가 상승했다!%s\n\n", BLUE, player->charname, RESET);
				_getch();
				printf("기초 HP  | 150 -> 225\n");
				printf("기초 ATK | 30 -> 45\n");
				printf("기초 DEF | 15 -> 25\n");
				printf("기초 MP  | 20 -> 25\n");
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
	printf("당신은 무릎을 꿇고 쓰러졌다.\n");
	_getch();
	printf("당신의 눈앞이 깜깜해지기 시작했다.\n");
	_getch();
	printf("흐려져가는 의식 속에서,\n");
	_getch();
	printf("당신은 마지막으로 당신의 몸을 보았다.\n");
	_getch();
	printf("%s의 몸은 빛나는 먼지로 변해 사라져가고 있었다.\n", player->charname);
	_getch();
	printf("당신의 마지막 모습은 빛나는 먼지가 되어 던전에 흩뿌려졌다.\n\n");
	_getch();
	printf("...?\n");
	_getch();
	printf("당신은 이상함을 느꼈다.\n");
	_getch();
	printf("당신의 몸은 사라졌지만, 당신의 의식은 오히려 이전보다 또렷해졌다..\n");
	_getch();
	printf("%s어쩌면, 당신이 쓰러진 이곳에서 다시 일어날 방법이 있을지도 모른다.%s\n", YELLOW, RESET);
	GAMEOVER = 1;
	//시작 화면으로...
	main();
}

void End(Charstats* player) {
	printf("던전을 클리어한 당신은 바깥으로 나간다.\n");
	_getch();
	printf("오래간만의 햇빛이 당신을 반긴다.\n");
	_getch();
	printf("당신은 자연을 바라보며 생각에 잠긴다.\n\n");
	_getch();
	printf("%s게임 클리어!%s\n\n", YELLOW, RESET);
	_getch();
	FILE* file = fopen(save, "w");
	if (file == NULL) {
		printf("파일 열기 실패\n");
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

void addItem(Charstats* player, char name[30], int count) {//인벤토리에 아이템 추가
	char input[10];
	int input1;
	if (player->invenCount == 8) {//가지고 있는 아이템수 8개
		while (1) {
			printf("아이템을 넣을 공간이 부족하다! 아이템을 버릴까?\n예/아니요");
			scanf("%s", input);
			if (!strcmp(input, "예")) {
				printf("---------------------------------\n");
				for (int i = 0; i < player->invenCount; i++) {
					printf("%d. %s %d개\n", i + 1, player->Inventory[i]->itemName, player->Inventory[i]->quantity);
				}
				while (1) {
					printf("---------------------------------\n번호 입력:");
					scanf("%d", &input1);
					if (input1 > 0 && input1 <= player->invenCount) {
						printf("%s를 버리고 %s를 새로 넣었다!\n", player->Inventory[input1-1]->itemName, name);
						strcpy(player->Inventory[input1-1]->itemName, name);
						player->Inventory[input1-1]->quantity = count;
						break;
					}
					else {

					}
					break;
				}
			}
			else if (!strcmp(input, "아니요")) {
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


void useItem(Charstats* player, char name[30], int count) {//인벤토리에 있는 아이템 사용
	
	if (player->invenCount > 0) {
		int i = 0;
		while ((player->invenCount > i)&&strcmp(name, player->Inventory[i])) {
			i++;
		}
		if (!strcmp(name, "회복 물약") || !strcmp(name, "디버프해제 물약") || !strcmp(name, "마나회복 물약") || !strcmp(name, "재생 물약") || !strcmp(name, "AP 물약")) {//일회성 아이템 사용시
			printf("%s를 사용했다!", player->Inventory[i]->itemName);
		}
		else {//제작에서 아이템 사용할때, 인벤토리에 있는 장비 장착시<-장비 장착할때는 장비 갈아끼는 함수를 따로 만들어야겠다
			//대사 출력 X
		}
		player->Inventory[i]->quantity -= count;
		if (player->Inventory[i]->quantity == 0) {//출석부 때처럼 그 인덱스만 할당해제하고 한칸땡기기
	
			free(player->Inventory[i]);
			for (int k = i; k < player->invenCount; k++) {
				player->Inventory[k] = player->Inventory[k + 1];
			}
			player->invenCount--;
		}
	}
	else {
		printf("아이템이 없습니다.\n");
	}
}

void startingRoom(Charstats* player, Enemystats* enemy) {//각 층의 시작 방
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
		printf("당신은 이곳에서 곧 큰 시련을 겪게 될 거라는 것을 느꼈다.\n");
		_getch();
		printf("하지만 이곳에 들어온 이상, 끝을 봐야만 한다.\n");
		_getch();
		printf("당신은 의지를 다졌다.\n");
		_getch();
		battleRoom(player, enemy);//1층이면 전투방으로, 2층이나 3층이면(보스 깬 직후) 준비방으로
	}
	else if (player->stage == 2) {
		player->storeItem1 = 3;
		player->storeItem2 = 3;
		player->storeItem3 = 2;
		player->storeItem4 = 1;
		printf("힘든 싸움을 겪은 당신은 던전의 다음 층에 도착했다.\n");
		_getch();
		printf("아직 여기서 지쳐서는 안된다.\n");
		_getch();
		printf("...나아가자.\n");
		_getch();
		preparingRoom(player, enemy);
	}
	else if (player->stage == 3) {
		player->storeItem1 = 3;
		player->storeItem2 = 3;
		player->storeItem3 = 2;
		player->storeItem4 = 2;
		printf("우여곡절 끝에 당신은 마지막 층에 도착했다.\n");
		_getch();
		printf("끝이 머지않았지만, 당신은 긴장의 끈을 놓지 말아야 한다는 것을 되새겼다.\n");
		_getch();
		printf("당신은 발걸음을 옮겼다.\n");
		_getch();
		preparingRoom(player, enemy);
	}
}

void newGame(Charstats* player, Enemystats* enemy) {//메뉴에서 새 게임 시작시
	GAMEOVER = 0;
	printf("플레이어 이름 입력 : ");
	scanf("%s", player->charname);
	printf("%s\n", player->charname);
	player->HP = 100;
	player->MP = 10;
	player->currentHP = player->HP;
	player->currentMP = player->MP;
	player->ATK = 20;
	player->DEF = 10;
	strcpy(player->weapon->weaponName, "맨주먹");
	strcpy(player->armor->armorName, "맨몸");
	strcpy(player->weapon->weaponInfo, "모 게임에선 나무도 때려부수는 그 주먹이다.");
	strcpy(player->armor->armorInfo, "자세히 보고 있으면 멋지다. 아마도?");
	strcpy(player->weapon->weaponskill, "아무런 효과도 없다.");
	strcpy(player->armor->armorskill, "아무런 효과도 없다.");
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
	printf("---------------------------------\n던전에 오신 것을 환영합니다.\n");
	_getch();
	printf("이제부터 당신의 모험이 시작됩니다.\n");
	_getch();
	printf("그럼 행운을 빕니다.\n");
	_getch();
	printf("...\n");
	_getch();
	printf("당신은 눈을 떴다.\n");
	_getch();
	printf("이곳의 끝에는 무엇이 기다리고 있을까.\n");
	_getch();
	printf("당신은 호기심에 이끌려 발걸음을 옮겼다.\n");
	_getch();
	startingRoom(player, enemy);
}


void preparingRoom(Charstats* player, Enemystats* enemy) {//준비방(쉼터)
	battle = 0;
	strcpy(player->room, "preparing");
	saveGame(player);
	char input[10];
	printf("---------------------------------\n");
	printf("당신은 편안한 분위기의 장소에 도착했다.\n");
	_getch();
	printf("이곳에서 잠시 휴식을 취하는 것도 좋을 것 같다.\n");
	_getch();
	printf("이곳에서 휴식을 취하거나 전투를 준비할 수 있습니다.\n\n");
	_getch();
	printf("---------------------------------\n");
	readEquipments(player);
	//장비 영향을 받은 플레이어 최대 체력, 마나 설정
	int maxHP = player->HP + player->weapon->weaponHP + player->armor->armorHP;
	int maxMP = player->MP + player->weapon->weaponMP + player->armor->armorMP;
	while (1) {
		printf("전투\n\n");
		printf("회복\n\n");
		printf("상점\n\n");
		printf("제작\n\n");
		if (player->battleCount < 5) {
			printf("보스 %s(전투 %d회 후 해금)%s\n", RED, 5 - player->battleCount, RESET);
		}
		else {
			printf("보스 %s(입장가능)%s\n", YELLOW, RESET);
		}
		printf("\n상태창\n");
		printf("---------------------------------\n");
		printf("입력 : ");
		scanf("%s", input);
		if (!strcmp(input, "전투")) {
			battleRoom(player, enemy);
		}
		else if (!strcmp(input, "회복")) {
			char temp[10];
			printf("\n%d골드를 지불하고 체력과 마나를 회복시키겠습니까?\n", player->stage * 50);
			printf("현재 골드 : %d | HP : %d/%d | MP : %d/%d \n", player->gold, player->currentHP, maxHP, player->currentMP, maxMP);
			printf("예 / 아니오\n");
			scanf("%s", temp);
			if (!strcmp(temp, "예")) {
				if (player->gold >= player->stage * 50) {
					if (player->currentHP < maxHP || player->currentMP < maxMP) {
						printf("당신의 몸에는 생기가 돌아왔다.\n");
						player->currentHP = maxHP;
						player->currentMP = maxMP;
						player->gold -= player->stage * 50;

					}
					else {
						printf("당신은 이미 건강합니다.\n");
					}
				}
				else {
					printf("골드가 부족합니다.\n");
				}
			}
			else if (!strcmp(temp, "아니요")) {
			}
			else {
				printf("잘못된 입력입니다.\n");

			}
		}
		else if (!strcmp(input, "상점")) {
			store(player);
		}
		else if (!strcmp(input, "제작")) {
			crafting(player);
		}
		else if (!strcmp(input, "보스")) {
			if (player->battleCount < 5) {
				printf("전투 %d회를 더 승리하면 입장할 수 있습니다.\n", 5 - player->battleCount);
			}
			else {
				char temp[10];
				while (1) {
					printf("%s보스전을 진행하시겠습니까? (예/아니오)%s\n", YELLOW, RESET);
					scanf("%s", temp);
					if (!strcmp(temp, "예")) {
						bossRoom(player, enemy);
					}
					else if (strcmp(temp, "아니오")) {
						break;
					}
				}
			}
		}
		else if (!strcmp(input, "상태창")) {
			int equip;
			printf("---------------------------------\n");
			printf("플레이어 %s | 스테이지 %d | %dG\n", player->charname, player->stage, player->gold);
			healthBar(player->currentHP, maxHP, 0);
			ManaBar(player->currentMP, maxMP);
			printf("\n\n무기 : %s\n%s\n%s\n\n방어구 : %s\n%s\n%s\n\n", player->weapon->weaponName, player->weapon->weaponInfo, player->weapon->weaponskill, player->armor->armorName, player->armor->armorInfo, player->armor->armorskill);
			printf("인벤토리 :\n");
			for (int i = 0; i < player->invenCount; i++) {
				printf("%d. %s %d개\n", i + 1, player->Inventory[i]->itemName, player->Inventory[i]->quantity);
			}
			printf("---------------------------------\n");
			equip = _getch() - '1';
			char temp[30];
			if (equip >= 0 && equip < player->invenCount) {
				if (!strcmp(player->Inventory[equip], "맨주먹") || !strcmp(player->Inventory[equip], "슬라임 배트") || !strcmp(player->Inventory[equip], "돌 메이스") || !strcmp(player->Inventory[equip], "의식용 뼈칼") || !strcmp(player->Inventory[equip], "조잡한 에너지 소드")) {
					strcpy(temp, player->weapon->weaponName);
					strcpy(player->weapon->weaponName, player->Inventory[equip]);
					strcpy(player->Inventory[equip], temp);
					printf("%s을 장비했습니다.\n", player->weapon->weaponName);
				}
				else if(!strcmp(player->Inventory[equip], "슬라임 해머") || !strcmp(player->Inventory[equip], "마법 문양 돌 메이스") || !strcmp(player->Inventory[equip], "척수검") || !strcmp(player->Inventory[equip], "빛나는 에너지 소드")){
					strcpy(temp, player->weapon->weaponName);
					strcpy(player->weapon->weaponName, player->Inventory[equip]);
					strcpy(player->Inventory[equip], temp);
					printf("%s을 장비했습니다.\n", player->weapon->weaponName);
				}
				else if(!strcmp(player->Inventory[equip], "맨몸") || !strcmp(player->Inventory[equip], "점액 보호대") || !strcmp(player->Inventory[equip], "슬라임 갑옷") || !strcmp(player->Inventory[equip], "돌 갑옷") || !strcmp(player->Inventory[equip], "마법 문양 돌 갑옷")){
					strcpy(temp, player->armor->armorName);
					strcpy(player->armor->armorName, player->Inventory[equip]);
					strcpy(player->Inventory[equip], temp);
					printf("%s을 장비했습니다.", player->armor->armorName);
				}
				else if (!strcmp(player->Inventory[equip], "뼈 투구") || !strcmp(player->Inventory[equip], "정교한 뼈 갑옷") || !strcmp(player->Inventory[equip], "기계 방패") || !strcmp(player->Inventory[equip], "고대 에너지 방패")) {
					strcpy(temp, player->armor->armorName);
					strcpy(player->armor->armorName, player->Inventory[equip]);
					strcpy(player->Inventory[equip], temp);
					printf("%s을 장비했습니다.", player->armor->armorName);
				}
				strcpy(temp, "");
				readEquipments(player);
			}
		}
		else {
			printf("잘못된 입력입니다.\n");
		}
	}
}

void store(Charstats* player) {

	printf("여기는 물약 상점이라네!\n");
	printf("골라보시게나!\n");
	//스테이지 시작 때마다 초기화되는 전역변수 4개
	int input;

	if (player->storeItem1 > 0) {
		printf("1. 체력회복 물약 %d G 잔고 : %d\n체력을 40 회복한다.\n\n", 50 + (player->stage-1) *20 , player->storeItem1);
	}
	else {
		printf("1. 체력회복 물약(품절)\n\n");
	}
	if (player->storeItem2 > 0) {
		printf("2. 마나회복 물약 %d G 잔고 : %d\n마나를 20 회복한다.\n\n", 30 + (player->stage - 1) * 10, player->storeItem2);
	}
	else {
		printf("2. 마나회복 물약(품절)\n\n");
	}
	if (player->storeItem3 > 0) {
		printf("3. 재생 물약 %d G 잔고 : %d\n4턴간 지속적으로 체력을 회복한다.\n\n", 75 + (player->stage - 1) * 30, player->storeItem3);
	}
	else {
		printf("3. 재생 물약(품절)\n\n");
	}
	if (player->storeItem4 > 0) {
		printf("4. AP 물약 %d G 잔고 : %d\n이번 턴 동안 행동력 1을 얻는다.\n\n", 120 + (player->stage - 1) * 40, player->storeItem4);
	}
	else {
		printf("4. AP 물약(품절)\n\n");
	}
	printf("보유 골드: %d G", player->gold);
	while (1) {
		printf("번호 입력 | 나가기=0:");
		scanf("%d", &input);

		if (input == 1) {
			if (player->gold >= 50 * 50 + (player->stage - 1) * 20) {
				player->gold -= 50 + (player->stage - 1) * 20;
				addItem(player, "체력회복 물약", 1);
				player->storeItem1--;
				printf("구매해 주셔서 고맙네!\n");
			}
			else if (player->storeItem1 == 0) {
				printf("아쉽지만 다 팔렸다네!\n");
			}
			else {
				printf("돈이 부족하다네!\n");
			}
		}
		else if (input==2) {
			if (player->gold >= 30 + (player->stage - 1) * 10) {
				player->gold -= 30 + (player->stage - 1) * 10;
				addItem(player, "마나회복 물약", 1);
				player->storeItem1--;
				printf("구매해 주셔서 고맙네!\n");
			}
			else if (player->storeItem1 == 0) {
				printf("아쉽지만 다 팔렸다네!\n");
			}
			else {
				printf("돈이 부족하다네!\n");
			}
		}
		else if (input==3) {
			if (player->gold >= 75 + (player->stage - 1) * 30) {
				player->gold -= 75 + (player->stage - 1) * 30;
				addItem(player, "재생 물약", 1);
				player->storeItem1--;
				printf("구매해 주셔서 고맙네!\n");
			}
			else if (player->storeItem1 == 0) {
				printf("아쉽지만 다 팔렸다네!\n");
			}
			else {
				printf("돈이 부족하다네!\n");
			}
		}
		else if (input==4) {
			if (player->gold >= 120 + (player->stage - 1) * 40)
			{
				player->gold -= 120 + (player->stage - 1) * 40;
				addItem(player, "AP 물약", 1);
				player->storeItem1--;
				printf("구매해 주셔서 고맙네!\n");
			}
			else if (player->storeItem1 == 0) {
				printf("아쉽지만 다 팔렸다네!\n");
			}
			else {
				printf("돈이 부족하다네!\n");
			}
		}
		else if (input==0) {
			printf("좋은 하루 되시길!\n\n");
			break;
		}
	}
}

void crafting(Charstats* player) {
	//장비들 진열해놓고 입력받기, 재료가 충분하다면 재료 소모 후 현재 장비를 인벤토리에 가져다두고 새 장비 데이터를 파일로부터 입력받음
	printf("누가 봐도 뭔가를 만들어야 할 것만 같은 테이블이 놓여 있다.\n");
	printf("이곳에서 전리품으로 장비들을 제작할 수 있을 것 같다.\n");
	printf("---------------------------------\n제작대\n");
	char input[10];
	int equipment;
	char equipment_made[50] = "";
	char material1[30] = "";
	char material2[30] = "";
	while (1) {
		printf("무기 | 방어구 | 그만두기\n입력 : ");
		scanf("%s", input);
		if (!strcmp(input, "무기")) {
				printf("---------------------------------\n");
				printf("1. 슬라임 배트 | 슬라임 점액질 *3\n\n2. 돌 메이스 | 문양이 새겨진 돌 *3\n\n");
				printf("3. 의식용 뼈칼 | 언데드 뼛조각 *3\n\n4. 조잡한 에너지 소드 | 고대 톱니바퀴 *3\n\n");
				if (player->stage > 1) {
					printf("5. 슬라임 해머 | 슬라임 배트 + 슬라임 핵 *2\n또는 슬라임 핵 *4\n\n");
					printf("6. 마법 문양 돌 메이스 | 돌 메이스 + 마법이 흐르는 돌 *2\n또는 마법이 흐르는 돌 *4\n\n");
					printf("7. 척수검 | 의식용 뼈칼 + 저주가 깃든 뼈 *2\n또는 저주가 깃든 뼈 *4\n\n");
					printf("8. 빛나는 에너지 소드 | 조잡한 에너지 소드 + 고대 엔진 *2\n또는 고대 엔진 *4\n");
				}
				while (1){
				printf("---------------------------------\n보유 재료\n\n");
				printf("슬라임 점액질 : %d | ", showItemCount(player, "슬라임 점액질"));
				printf("문양이 새겨진 돌: %d \n", showItemCount(player, "문양이 새겨진 돌"));
				printf("언데드 뼛조각 : %d | ", showItemCount(player, "언데드 뼛조각"));
				printf("고대 톱니바퀴: %d \n", showItemCount(player, "고대 톱니바퀴"));
				if (player->stage > 1) {
					printf("슬라임 핵 : %d | ", showItemCount(player, "슬라임 핵"));
					printf("마법이 흐르는 돌: %d \n", showItemCount(player, "마법이 흐르는 돌"));
					printf("언데드 뼛조각 : %d | ", showItemCount(player, "저주가 깃든 뼈"));
					printf("고대 톱니바퀴: %d \n", showItemCount(player, "고대 엔진"));
				}
				printf("0. 취소\n");
				printf("---------------------------------\n입력 | :");
				scanf("%d", &equipment);

				if (equipment == 0) {
					break;
				}

				if (equipment == 1) {
					strcpy(equipment_made, "슬라임 배트");
					strcpy(material1, "슬라임 점액질");
				}
				else if (equipment == 2) {
					strcpy(equipment_made, "돌 메이스");
					strcpy(material1, "문양이 새겨진 돌");
				}
				else if (equipment == 3) {
					strcpy(equipment_made, "의식용 뼈칼");
					strcpy(material1, "언데드 뼛조각");
				}
				else if (equipment == 4) {
					strcpy(equipment_made, "조잡한 에너지 소드");
					strcpy(material1, "고대 톱니바퀴");
				}
				else if (player->stage > 1) {  // 2단계 이상의 무기만 처리
					if (equipment == 5) {
						strcpy(equipment_made, "슬라임 해머");
						strcpy(material1, "슬라임 핵");
						strcpy(material2, "슬라임 배트");
					}
					else if (equipment == 6) {
						strcpy(equipment_made, "마법 문양 돌 메이스");
						strcpy(material1, "마법이 흐르는 돌");
						strcpy(material2, "돌 메이스");
					}
					else if (equipment == 7) {
						strcpy(equipment_made, "척수검");
						strcpy(material1, "저주가 깃든 뼈");
						strcpy(material2, "의식용 뼈칼");
					}
					else if (equipment == 8) {
						strcpy(equipment_made, "빛나는 에너지 소드");
						strcpy(material1, "고대 엔진");
						strcpy(material2, "조잡한 에너지 소드");
					}
				}

				if (player->stage > 1 && material2[0] != '\0') {
					if (showItemCount(player, material1) >= 4) {
						useItem(player, material1, 4);
						addItem(player, equipment_made, 1);
						printf("%s를 제작했다!\n", equipment_made);
					}
					else if (showItemCount(player, material2) >= 1 && showItemCount(player, material1) >= 2) {
						if (!strcmp(player->weapon->weaponName, material2)) {
							strcpy(player->weapon->weaponName, "맨주먹");
						}
						else {
							useItem(player, material2, 1);
						}
						useItem(player, material1, 2);
						addItem(player, equipment_made, 1);
						printf("%s를 제작했다!\n", equipment_made);
					}
					else {
						printf("재료가 부족합니다!\n\n");
					}
				}
				else {
					if (showItemCount(player, material1) >= 3) {
						useItem(player, material1, 3);
						addItem(player, equipment_made, 1);
						printf("%s를 제작했다!\n", equipment_made);
						break;
					}
					else {
						printf("%s", material1);
						printf("%s", equipment_made);
						printf("%d", showItemCount(player, material1));
						printf("재료가 부족합니다!\n\n");
					}
				}
			}
		}
		else if (!strcmp(input, "방어구")) {
				printf("---------------------------------\n");
				printf("1. 점액 보호대 | 슬라임 점액질 *3\n\n2. 돌 갑옷 | 문양이 새겨진 돌 *3\n\n");
				printf("3. 뼈 투구 | 언데드 뼛조각 *3\n\n4. 기계 방패 | 고대 톱니바퀴 *3\n\n");
				if (player->stage > 1) {
					printf("5. 슬라임 갑옷 | 점액 보호대 + 슬라임 핵 *2\n또는 슬라임 핵 *4\n\n");
					printf("6. 마법 문양 돌 갑옷 | 돌 갑옷 + 마법이 흐르는 돌 *2\n또는 마법이 흐르는 돌 *4\n\n");
					printf("7. 정교한 뼈 갑옷 | 뼈 투구 + 저주가 깃든 뼈 *2\n또는 저주가 깃든 뼈 *4\n\n");
					printf("8. 고대 에너지 방패 | 기계 방패 + 고대 엔진 *2\n또는 고대 엔진 *4\n");
				}
				while (1) {
				printf("---------------------------------\n보유 재료\n\n");
				printf("슬라임 점액질 : %d | ", showItemCount(player, "슬라임 점액질"));
				printf("문양이 새겨진 돌: %d \n", showItemCount(player, "문양이 새겨진 돌"));
				printf("언데드 뼛조각 : %d | ", showItemCount(player, "언데드 뼛조각"));
				printf("고대 톱니바퀴: %d \n", showItemCount(player, "고대 톱니바퀴"));
				if (player->stage > 1) {
					printf("슬라임 핵 : %d | ", showItemCount(player, "슬라임 핵"));
					printf("문양이 새겨진 돌: %d \n", showItemCount(player, "마법이 흐르는 돌"));
					printf("언데드 뼛조각 : %d | ", showItemCount(player, "저주가 깃든 뼈"));
					printf("고대 톱니바퀴: %d \n", showItemCount(player, "고대 엔진"));
				}
				printf("0. 취소\n");
				printf("---------------------------------\n입력 | :");
				scanf("%d", &equipment);

				if (equipment == 0) {
					break;
				}

				if (equipment == 1) {
					strcpy(equipment_made, "점액 보호대");
					strcpy(material1, "슬라임 점액질");
				}
				else if (equipment == 2) {
					strcpy(equipment_made, "돌 갑옷");
					strcpy(material1, "문양이 새겨진 돌");
				}
				else if (equipment == 3) {
					strcpy(equipment_made, "뼈 투구");
					strcpy(material1, "언데드 뼛조각");
				}
				else if (equipment == 4) {
					strcpy(equipment_made, "기계 방패");
					strcpy(material1, "고대 톱니바퀴");
				}
				else if (player->stage > 1) {  // 2단계 이상의 무기만 처리
					if (equipment == 5) {
						strcpy(equipment_made, "슬라임 갑옷");
						strcpy(material1, "슬라임 핵");
						strcpy(material2, "점액 보호대");
					}
					else if (equipment == 6) {
						strcpy(equipment_made, "마법 문양 돌 갑옷");
						strcpy(material1, "마법이 흐르는 돌");
						strcpy(material2, "돌 갑옷");
					}
					else if (equipment == 7) {
						strcpy(equipment_made, "정교한 뼈 갑옷");
						strcpy(material1, "저주가 깃든 뼈");
						strcpy(material2, "뼈 투구");
					}
					else if (equipment == 8) {
						strcpy(equipment_made, "고대 에너지 방패");
						strcpy(material1, "고대 엔진");
						strcpy(material2, "기계 방패");
					}
				}

				if (player->stage > 1 && material2[0] != '\0') {
					if (showItemCount(player, material1) >= 4) {
						useItem(player, material1, 4);
						addItem(player, equipment_made, 1);
						printf("%s를 제작했다!\n", equipment_made);
					}
					else if (showItemCount(player, material2) >= 1 && showItemCount(player, material1) >= 2) {
						if (!strcmp(player->armor->armorName, material2)) {
							strcpy(player->armor->armorName, "맨몸");
						}
						else {
							useItem(player, material2, 1);
						}
						useItem(player, material1, 2);
						addItem(player, equipment_made, 1);
						printf("%s를 제작했다!\n", equipment_made);
					}
					else {
						printf("재료가 부족합니다!\n\n");
					}
				}
				else {
					if (showItemCount(player, material1) >= 3) {
						useItem(player, material1, 3);
						addItem(player, equipment_made, 1);
						printf("%s를 제작했다!\n", equipment_made);
						break;
					}
					else {
						printf("%s", material1);
						printf("%s", equipment_made);
						printf("%d", showItemCount(player, material1));
						printf("재료가 부족합니다!\n\n");
					}
				}
			}
		}
		else if (!strcmp(input, "그만두기")) {
			printf("그만두었다.\n\n\n");
			break;
		}
	}


}

int showItemCount(Charstats* player, char name[30]) {
	int i;
	// 플레이어의 인벤토리를 순회하며 이름이 일치하는 아이템을 찾습니다.
	for (i = 0; i < player->invenCount; i++) {
		if (player->Inventory[i] != NULL && !strcmp(player->Inventory[i]->itemName, name)) {
			// 아이템이 인벤토리에 있으며 장비로 착용 중인 경우 +1 반환
			if (!strcmp(player->weapon->weaponName, name) || !strcmp(player->armor->armorName, name)) {
				return player->Inventory[i]->quantity + 1;
			}
			// 인벤토리에만 있는 경우 수량 반환
			else {
				return player->Inventory[i]->quantity;
			}
		}
	}
	// 현재 장비 중인 무기나 갑옷의 이름과 일치할 경우 1 반환
	if (!strcmp(player->weapon->weaponName, name) || !strcmp(player->armor->armorName, name)) {
		return 1;
	}
	// 인벤토리에도 장착 중인 장비에도 없는 경우 0 반환
	return 0;
}


void startMenu(Charstats* player, Enemystats* enemy) {
	char input[10];
	printf("Text RPG GAME\n\n---------------------------------\n");
		if (GAMEOVER == 1) {
			printf("새로하기\n\n%s이어하기%s\n\n튜토리얼\n", YELLOW, RESET);
		}
		else {
			printf("새로하기\n\n이어하기\n\n튜토리얼\n");
		}
	printf("---------------------------------\n");
	while (1) {
		printf("입력 : ");
		scanf("%s", input);
		if (!strcmp(input, "새로하기")) {
			newGame(player, enemy);
		}
		else if (!strcmp(input, "이어하기")) {
			loadGame(player, enemy);
		}
		else if (!strcmp(input, "튜토리얼")) {
			HowTo(player);
		}
		else {
			printf("잘못 입력했습니다.\n");
		}
	}
}
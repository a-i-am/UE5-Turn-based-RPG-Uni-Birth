# Uni Birth

[프로젝트 PDF](docs/UniBirth.pdf)

> 이 저장소는 포트폴리오용 C++ 소스 아카이브입니다. 원본 Content와 `.uproject`가 없어 단독 실행할 수 없습니다.

'클레르 옵스큐어: 33원정대(Clair Obscur: Expedition 33)'의 실시간 방어 및 반응형 전투 시스템을 모티브로 개발한 Unreal Engine 5 기반 턴제 RPG 팀 프로젝트입니다.

### 프로젝트 상태

장비 사양(MSI GF63 Thin 9SC)으로 인해 Unreal Engine 에디터 렌더링 실행은 제외하였으나, C++ 소스코드 전반에 대한 언리얼 엔진 5 표준 코딩 컨벤션(UE5 Coding Standard) 적용 및 소스코드 리팩터링을 완료하였습니다.

README 업데이트: 2026-08-09

### 실행 빌드

- [Windows 디버그 메시지 비표시 빌드](https://github.com/a-i-am/UE5-Turn-based-RPG-Uni-Birth/releases/tag/debug-free-build-v1)
- 압축 해제 후 `Windows/UniBirth.exe` 실행

### 영상

| 포트폴리오 소개 영상 | 기능 흐름 시연 영상 |
| --- | --- |
| [![포트폴리오 소개 영상](images/portfolio-overview-preview.jpg)](https://github.com/a-i-am/UE5-Turn-based-RPG-Uni-Birth/releases/download/portfolio-videos-v1/UniBirth-portfolio-overview-no-captions.mp4) | [![기능 흐름 시연 영상](images/feature-flow-preview.jpg)](https://github.com/a-i-am/UE5-Turn-based-RPG-Uni-Birth/releases/download/portfolio-videos-v1/UniBirth-feature-flow-no-captions.mp4) |
| 전투, 탐색, UI 흐름을 중심으로 Uni Birth의 전체 플레이 분위기를 정리한 영상입니다. | 보상 획득, 인벤토리 조작/착용, 드론 착용 후 바실리스크 전투, 분해·조합 흐름을 보여줍니다. |

GitHub에서 큰 MP4 파일은 미리보기가 제한될 수 있어, 영상은 [Release asset](https://github.com/a-i-am/UE5-Turn-based-RPG-Uni-Birth/releases/tag/portfolio-videos-v1)으로 제공합니다. 썸네일을 클릭하면 브라우저 설정에 따라 영상이 바로 재생되거나 다운로드됩니다.

### 프로젝트 정보

| 항목 | 내용 |
| --- | --- |
| 개발 기간 | 2025-10 - 2025-12 |
| 리팩터링 상태 | C++ 소스코드 컨벤션 리팩터링 완료 (엔진 에디터 실행 제외) |
| 인원 | 기획 5인, 아트 5인, 개발 3인 |
| 엔진 | Unreal Engine 5.5.4 |
| 협업 | TortoiseSVN, Notion, Discord |


### 기술 스택
<p>
  <img src="https://img.shields.io/badge/Unreal Engine 5-0E1128?style=flat-square&logo=unreal-engine&logoColor=white"/>
  <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=flat-square&logo=c%2B%2B&logoColor=white"/>
  <img src="https://img.shields.io/badge/UMG-000000?style=flat-square"/>
</p>

### 프로젝트 구조

<p>
  <img src="https://img.shields.io/badge/⭐_본인_담당_작업-FF5722?style=flat-square"/>
  <img src="https://img.shields.io/badge/기반_시스템-00599C?style=flat-square"/>
</p>

프로젝트의 핵심 C++ 소스코드는 `Source/UniBirth/` 아래에 모듈별로 구조화되어 있습니다. 아래 파일 링크를 클릭하면 GitHub의 해당 소스코드 파일로 바로 이동하여 확인하실 수 있습니다.

| 모듈 구분 | 파일 / 경로 | 담당 및 핵심 로직 설명 |
| :--- | :--- | :--- |
| <img src="https://img.shields.io/badge/⭐_담당_작업-FF5722?style=flat-square"/> **Component** | [`UBComboBuffComponent.h`](Source/UniBirth/Component/UBComboBuffComponent.h) / [`.cpp`](Source/UniBirth/Component/UBComboBuffComponent.cpp) | **동일 등급 버프의 연쇄 합성 (C➔B➔A➔MAX) 및 슬롯 동기화 시스템** |
| <img src="https://img.shields.io/badge/⭐_담당_작업-FF5722?style=flat-square"/> **Component** | [`UBBuffComponent.h`](Source/UniBirth/Component/UBBuffComponent.h) / [`.cpp`](Source/UniBirth/Component/UBBuffComponent.cpp) | **스킬 버프/디버프 적용, 지속 시간 계산 및 해제(`EDelCase`) 관리** |
| <img src="https://img.shields.io/badge/⭐_담당_작업-FF5722?style=flat-square"/> **Battle / Core** | [`SampleCharacter.h`](Source/UniBirth/Battle/SampleCharacter.h) / [`.cpp`](Source/UniBirth/Battle/SampleCharacter.cpp) | **플레이어 회피·패링 QTE 판정 처리, 반격(Counter) 및 데미지 연동** |
| <img src="https://img.shields.io/badge/⭐_담당_작업-FF5722?style=flat-square"/> **UI / QTE** | [`UBDodgeUI.h`](Source/UniBirth/UI/Combat/Combo/UBDodgeUI.h) / [`.cpp`](Source/UniBirth/UI/Combat/Combo/UBDodgeUI.cpp) | **회피 QTE 커맨드 방향키 UI 및 슬롯 입력 상태 동기화** |
| <img src="https://img.shields.io/badge/⭐_담당_작업-FF5722?style=flat-square"/> **UI / QTE** | [`UBParryUI.h`](Source/UniBirth/UI/Combat/Combo/UBParryUI.h) / [`.cpp`](Source/UniBirth/UI/Combat/Combo/UBParryUI.cpp) | **패링 QTE 게이지 프로그레스 및 서클 타이밍 판정 UI** |
| <img src="https://img.shields.io/badge/⭐_담당_작업-FF5722?style=flat-square"/> **UI / QTE** | [`UBDodgeKeySlot.h`](Source/UniBirth/UI/Combat/Combo/UBDodgeKeySlot.h) / [`.cpp`](Source/UniBirth/UI/Combat/Combo/UBDodgeKeySlot.cpp) | **QTE 입력 슬롯 키 바인딩 및 애니메이션 연동** |
| <img src="https://img.shields.io/badge/⭐_담당_작업-FF5722?style=flat-square"/> **Common / Data** | [`UBBuffData.h`](Source/UniBirth/Common/Data/UBBuffData.h) | **데이터 테이블(CSV) 기반 버프/QTE 스탯 및 콤보 데이터 구조** |
| **Component** | [`HealthComponent.h`](Source/UniBirth/Component/HealthComponent.h) / [`.cpp`](Source/UniBirth/Component/HealthComponent.cpp) | 체력, 실드 차감 및 사망 처리, 나이아가라 이펙트 스폰 |
| **Battle** | [`BattleManager.h`](Source/UniBirth/Battle/BattleManager.h) / [`.cpp`](Source/UniBirth/Battle/BattleManager.cpp) | 턴 순서 정렬, 몬스터 공격 의도 브로드캐스트 및 턴 진행 제어 |
| **Character** | [`UBPlayer.h`](Source/UniBirth/Character/UBPlayer.h) / [`.cpp`](Source/UniBirth/Character/UBPlayer.cpp) | 플레이어 캐릭터 기본 액터, 스프링암/카메라, 총기/드론 에셋 |
| **Character** | [`UBBaseMonster.h`](Source/UniBirth/Character/Enemy/UBBaseMonster.h) / [`.cpp`](Source/UniBirth/Character/Enemy/UBBaseMonster.cpp) | 몬스터/보스 페이즈 공격 시퀀스 및 회전/이동 타이머 |
| **Common** | [`UBdefine.h`](Source/UniBirth/Common/UBdefine.h) / [`.cpp`](Source/UniBirth/Common/UBdefine.cpp) | 프로젝트 전체 열거형(Enum) 및 글로벌 상수 관리 |
| **Common** | [`UBGameInstance.h`](Source/UniBirth/Common/System/UBGameInstance.h) / [`.cpp`](Source/UniBirth/Common/System/UBGameInstance.cpp) | 데이터 테이블 로딩, 스탯 및 아이템/스킬 메인 게이트웨이 |

#### 디렉토리 트리 구조

```text
Source/UniBirth/
├── 📂 Component/
│   ├── ⭐ UBComboBuffComponent.h / .cpp   (버프 연쇄 합성 & 콤보 슬롯 관리)
│   ├── ⭐ UBBuffComponent.h / .cpp        (스킬 버프/디버프 적용 및 해제)
│   └── HealthComponent.h / .cpp         (체력/실드 계산 및 사망 처리)
├── 📂 Battle/
│   ├── ⭐ SampleCharacter.h / .cpp        (QTE 판정 결과 처리 & 반격 로직)
│   └── BattleManager.h / .cpp           (턴 순서 정렬 & 턴제 진행 관리)
├── 📂 UI/Combat/Combo/
│   ├── ⭐ UBDodgeUI.h / .cpp              (회피 QTE 커맨드 UI)
│   ├── ⭐ UBParryUI.h / .cpp              (패링 QTE 타이밍 게이지 UI)
│   └── ⭐ UBDodgeKeySlot.h / .cpp         (QTE 키 슬롯 표시 및 동기화)
├── 📂 Character/
│   ├── UBPlayer.h / .cpp                (플레이어 캐릭터 기본 클래스)
│   └── Enemy/UBBaseMonster.h / .cpp     (보스/몬스터 패턴 시퀀스)
└── 📂 Common/
    ├── ⭐ Data/UBBuffData.h               (CSV 데이터 테이블 버프 구조체)
    ├── UBdefine.h / .cpp                (프로젝트 핵심 Enum 및 상수)
    └── System/UBGameInstance.h / .cpp   (게임 인스턴스 & 데이터 매니저)
```

---

### 플레이 및 조작 방법

Uni Birth는 **실시간 방어 반응(QTE)** 요소가 결합된 턴제 RPG 시스템입니다.

#### 1. 전투 진입 및 턴 시스템 (Battle Encounter & Turn Order)
- 필드에서 몬스터와 조우 시 전투 모드로 진입합니다.
- 각 캐릭터 및 몬스터의 `AttackSpeed(공격 속도)` 스탯에 따라 **턴 순서(Turn Order)**가 자동으로 정렬되어 화면 상단 프로필 리스트에 표시됩니다.
- 본인 턴이 오면 **일반 공격**, **액티브 스킬**, **궁극기**, **아이템 사용**, **방어(Guard)** 중 행동을 선택합니다.

#### 2. 실시간 반응형 방어 시스템 (Dodge & Parry QTE)
몬스터가 공격 시퀀스를 개시하면 슬로우 모션(Time Dilation)과 함께 방어 QTE 타이밍이 발생합니다.

- 🛡️ **패링 (Parry QTE)**:
  - **조작**: 몬스터 공격 게이지가 지정된 패링 존(Parry Zone)에 도달했을 때 **[Space Bar]**를 정확한 타이밍에 입력합니다.
  - **효과**: 피격 데미지 100% 무효화 + 몬스터 공격 직후 **강한 반격(Counter Attack)** 수행 + 패링 콤보 버프 획득.
- ⚡ **회피 (Dodge QTE)**:
  - **조작**: 화면에 제시되는 방향키 커맨드(**[W], [A], [S], [D]**)를 제한 시간 내에 순서대로 모두 입력합니다.
  - **효과**: 피격 데미지 100% 무효화 + 회피 콤보 버프 획득.
- 🔰 **방어 (Guard) 및 실패 판정**:
  - 타이밍을 놓치거나 오입력 시 QTE가 실패하며, 기본 방어(Guard) 상태로 전환되어 데미지를 부분 차단하거나 피격됩니다.
  - QTE 실패 시 기존에 쌓여있던 콤보 버프가 초기화됩니다 (`EDelCase::ComboFailure`).

#### 3. 버프 연쇄 합성 시스템 (Chain Synthesis)
- QTE 성공 시 **C등급 콤보 버프**를 슬롯에 획득합니다.
- 동일한 종류의 동일 등급 버프 2개가 슬롯에 모이면 자동으로 상위 등급으로 연쇄 합성됩니다:
  $$\text{C 등급} + \text{C 등급} \longrightarrow \text{B 등급} \longrightarrow \text{A 등급} \longrightarrow \text{MAX 등급}$$
- 등급이 상승할수록 공격력 배율, 치명타 확률, 방어력 등 CSV 데이터 테이블 기반 스탯이 기하급수적으로 강화됩니다.

---

### 담당 작업

- 플레이어 회피·패링 QTE 시스템 로직 설계
- 스킬 버프와 디버프 적용 및 해제 구현
- 동일 등급 버프의 연쇄 합성 시스템
- 콤보 성공·실패와 버프 슬롯 상태 동기화
- 데이터 테이블(CSV) 기반 QTE 및 스탯 수치 연동

### 업데이트 계획

- 사용한 에셋 출처 표기 예정


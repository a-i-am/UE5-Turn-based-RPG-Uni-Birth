# Uni Birth

[한국어](#한국어) | [English](#english)

[프로젝트 PDF](https://github.com/user-attachments/files/25696755/UniBirth.pdf)

> 이 저장소는 포트폴리오용 C++ 소스 아카이브입니다. 원본 Content와 `.uproject`가 없어 단독 실행할 수 없습니다.

## 한국어

`33원정대`의 실시간 반응 요소를 참고한 Unreal Engine 턴제 RPG 팀 프로젝트입니다.

최종 업데이트: 2026-06-30

### 프로젝트 정보

| 항목 | 내용 |
| --- | --- |
| 개발 기간 | 2025-10 - 2025-12 |
| 리팩터링 기간 | 별도 기록 없음 |
| 인원 | 기획 5인, 아트 5인, 개발 3인 |
| 엔진 | Unreal Engine 5.5.4 |
| 협업 | TortoiseSVN, Notion, Discord |

### 담당 작업

- 플레이어 회피·패링 QTE
- 스킬 버프와 디버프 적용·해제
- 동일 등급 버프의 연쇄 합성 시스템
- 콤보 성공·실패와 버프 슬롯 상태 연결
- 데이터 테이블 기반 QTE 및 버프 수치 연동

### 핵심 구현과 선택

- 회피 입력에서 첫 키의 반대 방향을 제외한 패턴을 생성해 스킬별 난이도를 데이터로 조절했습니다.
- 패링 판정 영역을 UMG 캔버스 좌표와 스탯 데이터에 연결했습니다.
- 버프 추가와 제거를 같은 연산 흐름으로 처리해 반대 로직의 중복을 줄였습니다.
- 연속 합성 가능한 버프를 재귀적으로 처리하고, 제거될 이전 버프 배율을 모아 최종 스탯에 한 번 반영했습니다.

### 기술 스택

`Unreal Engine 5` `C++` `UMG` `DataTable` `CSV` `TMap` `TArray`

### 영상 및 업데이트

- 보유한 회피, 패링, 콤보 버프 영상을 업로드한 뒤 이 섹션에 연결합니다.
- 영상 파일은 저장소에 직접 커밋하지 않고 링크와 썸네일만 사용합니다.

### 배운 점

UI 입력, 전투 상태, 데이터 테이블, 스탯 갱신이 동시에 바뀌는 시스템에서는 상태 적용 순서와 제거 시점이 기능 정확도를 결정한다는 점을 배웠습니다.

### 브랜치 및 커밋 정리

- 리팩터링 커밋은 `refactor` / `refactoring` 키워드가 있는 경우만 분리해서 읽을 수 있게 정리합니다.
- 플레이 흐름과 전투 시스템 변경을 README에서 먼저 확인할 수 있게 정리했습니다.

### 업데이트 계획

- 회피, 패링, 콤보 버프 영상 링크를 추가합니다.
- 영상이 준비되면 썸네일과 함께 외부 링크만 유지합니다.

## English

Uni Birth is a team turn-based RPG inspired by the real-time defensive interactions of Clair Obscur: Expedition 33.

Last updated: 2026-06-30

### Project

- Development: 2025-10 - 2025-12
- Team: Five planners, five artists, and three programmers
- Engine: Unreal Engine 5.5.4
- Repository scope: C++ portfolio archive; not a standalone build

### My Contribution

- Dodge and parry QTEs
- Skill buffs, debuffs, and data-driven stat application
- Recursive combo-buff merging
- Correct stat reconciliation after merged buffs are removed

### Implementation Decisions

- Dodge patterns exclude the direction opposite the first key and use data-driven skill difficulty.
- Parry zones connect UMG canvas coordinates to combat-stat data.
- One signed operation path handles both buff application and removal.
- Recursive merging handles consecutive upgrades, while removed multipliers are reconciled once after the merge completes.

### Stack and Assets

`Unreal Engine 5` `C++` `UMG` `DataTable` `CSV` `TMap` `TArray`

This repository contains portfolio C++ source only. Original team Content assets and the `.uproject` are not redistributed; third-party material remains subject to the original team project's licenses.

### Video and Updates

Dodge, parry, and combo-buff footage will be uploaded as an external video. Only its link and thumbnail will be added to this repository.

### Lessons

When UI input, combat state, data tables, and stat updates change together, the order of state application and removal is part of the feature's correctness.

### Branch and Commit Notes

- Refactoring commits are readable by the `refactor` / `refactoring` keywords.
- The README keeps player flow, combat system, and implementation notes together.

### Update Plan

- Add dodge, parry, and combo-buff video links.
- Keep only external links and thumbnails for video delivery.

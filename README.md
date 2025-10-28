# SOS Rhythm Game Project – SVN Revision History Portfolio

본 리포지토리는 내부 SVN 기반으로 관리된 **SOS 프로젝트**의 주요 개발 이력과 커밋 로그를 정리한 아카이브입니다.  
리비전 메시지는 2024년 8월 5일부터 8월 7일까지의 주요 개발 내역을 기반으로 하며,  
핵심적인 기능 추가, 버그 수정, 핫픽스, 에디터 개선 등 프로젝트 전반의 개발 흐름을 담고 있습니다.

---

## 🛠️ 프로젝트 개요
- **프로젝트명**: SOS (리듬 액션 기반 프로젝트)
- **기간**: 2024년 8월
- **형태**: 팀 프로젝트 (SVN 기반 협업)
- **주요 기여자**:  
  - 박영웅 – 엔진 및 시스템 로직, 빌드 파이프라인, CSV 로더, 노트 매니저  
  - 김시우 – 타임라인 시스템, CurrTimeBlock 모듈, CameraActor, 월드 관리  
  - 우세철 – 오디오 시스템, SoundManager, Note Actor  

---

## 💡 핵심 기여 내용
- **NoteManager 및 판정 시스템 개선**
  - 노트 관리 로직 및 CSVWriter 기능 수정
  - 판정 세분화 로직 추가 및 핫픽스 처리
- **메타데이터 및 툴 체인**
  - GIF → LottieJson 변환 기능 추가 (Python 스크립트 연동)
  - MetadataEditor 및 SLN 필터 구조 개선
- **타임라인 시스템 및 UI 개선**
  - CurrTimeBlock 신규 시스템 추가
  - CameraActor 업데이트 및 월드 탐색 함수 개선
- **빌드 자동화 및 리소스 관리**
  - QA용 Final Demonstrated Build 배포
  - 사운드 매니저, BGM 매니저 수정 및 리소스 구조 정리

---

## 📄 구조

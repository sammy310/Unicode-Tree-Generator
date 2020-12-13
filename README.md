# Unicode Tree Generator

특정 폴더의 Tree를 유니코드 txt 파일로 생성해주는 프로그램입니다

It is a program that creates a tree in a specific folder as a Unicode txt file

---

### 사용법
1. 트리를 생성하고 싶은 폴더를 지정
2. 생성될 트리 파일이 저장될 위치를 지정
3. 생성될 트리 파일의 이름을 지정
4. 생성 버튼을 클릭

### How To Use
1. Specify the folder where you want to generate the tree
2. Specify the location to save the tree file to be created
3. Specifies the name of the tree file to be created
4. Click the 생성(Generate) button

---

### 테스트 결과 (TEST Result)
- [테스트 폴더 (Test Folders)](https://github.com/sammy310/Tree-Creater/tree/master/Tree%20Test)
- [생성된 트리 (Generated Tree)](https://github.com/sammy310/Tree-Creater/blob/master/Tree_Result.txt)
```
Tree Test
├─aaa
│  │  file1.txt
│  │  file2.txt
│  │  file3.txt
│  │  
│  ├─DIr1
│  │  │  file4.txt
│  │  │  file5.txt
│  │  │  
│  │  └─Dir2
│  │      │  file6.txt
│  │      │  
│  │      └─Dir3
│  │              file7.txt
│  │              
│  └─Dir4
├─English Test
│  │  file1.txt
│  │  file2.txt
│  │  
│  ├─Folder1
│  │      file3.txt
│  │      
│  └─Folder2
│          file4.txt
│          
├─中文测试
│  │  文件.txt
│  │  
│  └─文件夹
├─日本語テスト
│  │  ファイル1.txt
│  │  ファイル2.txt
│  │  
│  ├─フォルダ1
│  │      ファイル3.txt
│  │      
│  └─フォルダ2
│          ファイル4.txt
│          
└─한국어 테스트
    │  파일1.txt
    │  파일2.txt
    │  
    ├─폴더1
    │  │  파일3.txt
    │  │  파일4.txt
    │  │  
    │  ├─폴더3
    │  │      파일5.txt
    │  │      
    │  └─폴더4
    │          파일6.txt
    │          
    └─폴더2
```

### Why???

- 윈도우 CMD의 tree 명령어로는 일본어 등이 제대로 표시되지 않는 경우가 존재하여 제작하게 되었습니다
- There are cases where Japanese etc. are not displayed properly with the tree command of Windows CMD

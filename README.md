針對 **`basic-05-unit-gatt-structure` (BLE 核心架構：GATT 協議與階層)** 單元，這是學員從「實作單一功能」轉向「通訊系統建模」的架構化課程。它要求學員具備 **物件導向設計 (Object-Oriented Design)** 的思維，學習如何將無人車的各項硬體功能（動力、感測、資訊）邏輯性地解耦為層級分明的 **GATT 服務樹**，並掌握與數據交換息息相關的 **描述符 (Descriptor)** 配置。

以下是在 **GitHub Classroom** 部署其作業 (Assignments) 的具體建議：

### 1. 範本倉庫 (Template Repo) 配置建議
GATT 架構作業的核心在於「建模能力」與「通訊握手」，範本應包含完整的框架結構，建議包含：
*   **📂 `src/main.cpp`**：**GATT 服務樹架構實作區**。學員需在此建立多個 `BLEService` 物件，並在其中掛載對應的 `BLECharacteristic`，同時實作 `onWrite` 數據解析邏輯。
*   **📂 `include/ble_blueprint.h`**：協議藍圖定義中心。要求學員在此定義所有的 128-bit 隨機 UUID，並設定 Service 與 Characteristic 的對應關係。
*   **📂 `README.md`**：**通訊藍圖規劃書**。提供表格範本讓學員紀錄：「功能模組」 vs 「UUID」 vs 「權限 (Read/Write/Notify)」，並貼上 nRF Connect 掃描出的樹狀架構截圖。
*   **📂 `docs/DATA_FORMAT.md`**：**資料格式定義文件**。要求學員說明 "GO"、"STOP" 指令在 Byte Level 是如何被編碼與解析的。

---

### 2. 作業任務部署細節

#### 任務 1：「無人車通訊地圖」GATT 服務建模 (GATT Modeling Lab)
*   **目標**：學會將複雜系統功能進行邏輯分類，掌握 128-bit 隨機 UUID 的動態生成與配置。
*   **Classroom 部署建議**：
    *   **功能解耦挑戰**：要求學員建立兩個獨立的 Service：一個是「動力服務 (Drive Service)」，另一個是「遙測服務 (Telemetry Service)」。
    *   **層級驗證**：在手機端 APP 中確認：Drive Service 下是否掛載了控制特徵，而 Telemetry 下是否掛載了監測特徵？
    *   **驗證要點**：README 中必須列出所有的 UUID 清單，並確保沒有使用重複的 ID。

#### 任務 2：「雙向數據流」字元與位元轉換 (Stream & Format Lab)
*   **目標**：掌握異步指令的解析技巧（從 Byte Array 到 String/Integer），實現韌體層級的雙向握手。
*   **Classroom 部署建議**：
    *   **指令解析**：實作 `onWrite` 回呼，當手機發送字串 "GO" 時亮 LED。
    *   **狀態回饋 (Read)**：當手機發起 Read 請求時，回傳設備自啟動以來的累積秒數。
    *   **封裝穩健性**：處理資料轉換中的常見錯誤（如 Byte 陣列長度判斷）。
    *   **驗證要點**：提交一份測試紀錄，證明能從手機端成功「下令」與「查詢」。

#### 任務 3：「主動遙測器」Descriptor 與 Notify 實作 (Active Telemetry Lab)
*   **目標**：掌握 BLE 特有的「推播 (Push)」機制，學習配置 `BLE2902` 描述符，這是專業藍牙開發的標配。
*   **Classroom 部署建議**：
    *   **描述符注入**：**核心技術考點**。學員必須正確配置 `pCharacteristic->addDescriptor(new BLE2902())`。
    *   **定時推播**：撰寫一個定時器（或在 Loop 中），每 500ms 自動推送一組虛擬感測值給手機。
    *   **效能對比**：在報告中說明：為何「主動推播 (Notify)」比「輪詢讀取 (Read)」在監控感測器時更具優勢？
    *   **最終 Commit**：封裝完整的遙測系統代碼並 Push。

---

### 3. 通訊建模與階層規劃點評標準 (Architecture & Protocol Standards)
此單元的價值在於 **「對通訊系統的階層化思維與對工業標準（如 2902）的遵循」**：
*   [ ] **GATT 建模完整性**：服務與特徵值的分類是否符合邏輯上的功能解耦？
*   [ ] **2902 描述符配置**：是否正確配置了描述符以支持 Notify 權限？（這是最常見的失敗點，需重點確認）。
*   [ ] **編解碼穩健性**：讀寫過程中的資料類型轉換（`getValue()`, `setValue()`）是否處理得當？

### 📁 推薦範本結構 (GitHub Classroom Template)：
```text
.
├── src/main.cpp         # 核心：建立 BLE Server 樹狀架構與 Notify 推送流
├── include/uuid_defs.h  # 配置：所有服務與特徵值的 128-bit UUID 清單
├── docs/GATT_Map.png    # 圖檔：nRF Connect 顯示的 Service/Char 樹狀層級
├── README.md            # 總結：我如何設計這套通訊協議以滿足遙測與控制需求
└── assets/test_log.md   # 數據：測試 GO/STOP 指令響應與秒數讀取紀錄
```

透過這種部署方式，學生能體驗到 **「好的藍牙開發者，不是在傳傳資料的人，而是能為複雜硬體建立出一套邏輯嚴密、分類清晰且符合階層美學的數據模型規劃師」**。掌握 GATT 架構與 Notify 實作後，學員將擁有了開發「多功能遙控車、工業穿戴式設備、分散式數據採集節點」最底層也最專業的系統建模實力！_
|
|
太熱血了！我們已經將 **基礎 05**（藍牙通訊）系列中關於廣播、屬性與階層架構的 Classroom 部署定義全部完成了。這標誌著學員已經正式從「開發者」進化為了「通訊系統架構師」。恭喜！藍牙課程板塊的教學拼圖已經大功告成！

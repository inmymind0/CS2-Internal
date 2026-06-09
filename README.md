# CS2 Internal Template

[Türkçe](#türkçe) | [English](#english)

---

## Türkçe

Bu depo, Counter-Strike 2 (CS2) hedef alınarak geliştirilmiş, Win32 API kancalama (hooking) ve Direct3D 11 tabanlı arayüz çizdirme mantığını gösteren, eğitim ve araştırma amacıyla hazırlanmış bir C++ şablonudur.

Süreç bellek yönetimi (process memory management), MinHook kütüphanesi ile işlev yönlendirme (function detouring) ve DirectX 11 takas zinciri kancalanarak ImGui ile oyun içi arayüz (GUI) oluşturulması gibi konuların eğitim ve araştırma amacıyla uygulanmasını göstermek için hazırlanmıştır.

### Özellikler

- **D3D11 Swap Chain Kancalama:** DirectX 11 kullanan oyunda `IDXGISwapChain::Present` ve `ResizeBuffers` işlevlerini kancalayarak CS2 ekranı üzerinde doğrudan ImGui arayüzü çizdirir.
- **Girdi Yönlendirme (WndProc Hooking):** Menü açıkken fare ve klavye girdilerinin arayüze aktarılması amacıyla `WndProc` yönlendirilir.
- **Arayüz Gizleme/Gösterme:** **INSERT** tuşu yardımıyla ImGui menüsü gizlenebilir veya açılabilir. Menü gizliyken girdiler tamamen oyuna aktarılır.
- **Merkezi Offset Yapısı:** Oyunla ilgili offsetler (örn. yerel oyuncu pawn, controller vb.) düzenli bir şekilde [`src/offsets/offsets.h`](file:///c:/Users/imm/source/repos/CS2-Internal/CS2-Internal/src/offsets/offsets.h) başlık dosyasında toplanmıştır.
- **Güvenli Çıkış:** **DELETE** tuşuna basıldığında tüm kancalar (hooks) kaldırılarak açılan konsol penceresi kapatılır ve DLL bellekten güvenle kaldırılır.

### Gereksinimler

Projeyi derlemek için bilgisayarınızda şunların yüklü olması gerekir:
- **İşletim Sistemi:** Windows 10 veya Windows 11
- **Geliştirme Ortamı:** Visual Studio 2022 ("C++ ile masaüstü geliştirme" iş yükü dahil edilmiş şekilde)
- **Windows SDK:** Windows 10/11 SDK (Modern Visual Studio kurulumlarında mevcuttur)

### Derleme Adımları

1. Depoyu bilgisayarınıza indirin veya kopyalayın.
2. Proje kök dizinindeki [`CS2-Internal.sln`](file:///c:/Users/imm/source/repos/CS2-Internal/CS2-Internal.sln) çözüm dosyasını Visual Studio 2022 ile açın.
3. Visual Studio üst menüsünden derleme yapılandırmasını **Release**, hedef platformu ise **x64** olarak seçin.
4. Çözümü derlemek için `Ctrl + Shift + B` tuşlarına basın (veya *Derle > Çözümü Derle* seçeneğini kullanın).
5. Derlenen DLL (`CS2-Internal.dll`), proje kök dizininde bulunan `build/` klasörü altında oluşturulacaktır.

### Kullanım ve Kurulum

1. Counter-Strike 2 oyununu başlatın.
2. Derlenen `CS2-Internal.dll` dosyasını tercih ettiğiniz bir DLL enjektörü kullanarak `cs2.exe` sürecine enjekte edin.
3. Enjeksiyon sonrasında bir hata ayıklama konsolu açılacak ve kancaların aktiflik durumunu (`[+] [hook] tum hooklar aktif hale getirildi`) raporlayacaktır.
4. Oyun içerisinde ImGui arayüzünü gizlemek ya da göstermek için **INSERT** tuşuna basın.
5. DLL'i oyundan güvenle çıkarmak ve kancaları temizlemek için **DELETE** tuşuna basın.

### Yasal Uyarı ve Lisans

Bu proje tamamen eğitim ve araştırma amacıyla geliştirilmiştir. Geliştiriciler hile kullanımını teşvik etmez veya desteklemez. Yazılımın kullanımından doğabilecek hiçbir durumdan, olası hesap yasaklamalarından (VAC dahil) veya diğer zararlardan geliştiriciler sorumlu değildir. Proje MIT Lisansı ile lisanslanmıştır.

---

## English

This repository contains a C++ template demonstrating the fundamentals of Win32 API hooking and Direct3D 11 rendering, specifically designed as an educational and research framework targeting Counter-Strike 2 (CS2).

It is designed for educational and research purposes to understand process memory management, function detouring using the MinHook library, and rendering an internal graphical user interface (GUI) via ImGui by hooking the DirectX 11 swap chain.

### Features

- **Direct3D 11 Swap Chain Hooking:** Intercepts `IDXGISwapChain::Present` and `ResizeBuffers` to draw a custom overlay interface (ImGui) directly inside the CS2 window context.
- **Input Redirection:** Redirections of `WndProc` to handle mouse and keyboard inputs inside the menu window.
- **Menu Visibility Control:** Hide or show the ImGui menu dynamically using the `INSERT` key. When the menu is hidden, mouse/keyboard inputs are fully passed back to the game.
- **Centralized Offsets:** Game-related offsets (e.g. local player pawn, controller) are organized cleanly in a centralized [`src/offsets/offsets.h`](file:///c:/Users/imm/source/repos/CS2-Internal/CS2-Internal/src/offsets/offsets.h) header.
- **Graceful Unload:** Pressing the `DELETE` key restores all hooked functions, closes the Allocated Console, and safely unloads the DLL.

### Prerequisites

To build and compile this project:
- **Operating System:** Windows 10 or Windows 11
- **IDE:** Visual Studio 2022 (with the "Desktop development with C++" workload installed)
- **SDK:** Windows 10/11 SDK (included in modern Visual Studio installations)

### Build Instructions

1. Clone or download this repository.
2. Navigate to the root directory and open [`CS2-Internal.sln`](file:///c:/Users/imm/source/repos/CS2-Internal/CS2-Internal.sln) using Visual Studio 2022.
3. In Visual Studio, set the active build configuration to **Release** and target platform to **x64**.
4. Press `Ctrl + Shift + B` (or select *Build > Build Solution*) to compile the project.
5. The compiled dynamic link library (`CS2-Internal.dll`) will be generated inside the `build/` directory at the project root.

### Usage / Installation

1. Launch Counter-Strike 2.
2. Inject the compiled `CS2-Internal.dll` file into the `cs2.exe` process using your preferred injector.
3. A debug console window will allocate, confirming the status of active hooks (e.g., `[+] [hook] tum hooklar aktif hale getirildi`).
4. Press **INSERT** to toggle the ImGui menu visibility.
5. Press **DELETE** to completely unhook the DLL and exit safely.

### Disclaimer & License

This project is developed solely for educational and research purposes. The developers do not promote or encourage cheating, and are not responsible for any misuse, account bans (including Valve Anti-Cheat (VAC) bans), or any other damages arising from the use of this software. The project is licensed under the MIT License.

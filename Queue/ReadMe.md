<html>
<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="../assets/EduminD_dark.webp">
    <source media="(prefers-color-scheme: light)" srcset="../assets/EduminD.webp">
    <img src="../assets/EduminD.webp" alt="EduminD Logo" width="250">
  </picture>

</p>

<div align="center">
  <a href="https://t.me/X_MindWorld" target="_blank">
    <img src="https://img.shields.io/badge/Telegram-2CA5E0?style=for-the-badge&logo=telegram&logoColor=white" alt="Telegram" />
  </a>
  <a href="https://instagram.com/x_mindworld" target="_blank">
    <img src="https://img.shields.io/badge/Instagram-E4405F?style=for-the-badge&logo=instagram&logoColor=white" alt="Instagram" />
  </a>
  <a href="mailto:education.xmindworld@gmail.com" target="_blank">
    <img src="https://img.shields.io/badge/Gmail-D14836?style=for-the-badge&logo=gmail&logoColor=white" alt="Gmail" />
  </a>
</div>
</html>





# بررسی صف خطی و صف حلقوی (Linear Queue & Circular Queue)

توی این پروژه دو روش مختلف برای پیاده‌سازی ساختار داده‌ **Queue** رو بررسی کردم: صف خطی و صف حلقوی.

هدف این بود که قبل از استفاده از صف برای مدیریت رویدادهای MQTT در پروژه XNode-Aero، با نحوه عملکرد این دو روش و تفاوت‌هاشون آشنا بشم.

## ساختار پروژه

```
Queue/
├── CircularQueue/
└── LinearQueue/
```

هر پوشه یک پیاده‌سازی مستقل از صف داره و می‌تونین اون‌ها رو جداگانه اجرا و بررسی کنین.

## 1. Linear Queue | صف خطی

در این روش، عناصر به ترتیب داخل آرایه قرار می‌گیرن. با خارج کردن هر عنصر، فضای خالی ایجادشده با جابه‌جایی عناصر باقی‌مونده مدیریت می‌شه.

این روش ساده‌ست، اما جابه‌جایی عناصر باعث افزایش تعداد عملیات در بعضی شرایط می‌شه.

## 2. Circular Queue | صف حلقوی

در این روش، آرایه به شکل یک ساختار حلقوی مدیریت می‌شه. با استفاده از دو اندیس `front` و `rear` می‌تونیم بعد از رسیدن به انتهای آرایه، دوباره از ابتدای اون استفاده کنیم.

در نتیجه، برای حذف و اضافه کردن عناصر نیازی به جابه‌جایی داده‌ها نداریم.

## شرایط پیاده‌سازی

برای مقایسه عملکرد دو روش، هر دو رو با مشخصات یکسان پیاده‌سازی کردم:

- ظرفیت ثابت: ۴ عنصر
- استفاده از آرایه استاتیک
- رعایت ترتیب FIFO (First In, First Out)
- در صورت پر شدن صف، قدیمی‌ترین عنصر حذف و عنصر جدید جایگزین می‌شه.
- تعداد عناصر حذف‌شده در شمارنده‌ی `droppedCount` نگهداری می‌شه.
- با استفاده از `peek()` می‌تونیم اولین عنصر صف رو بدون حذف کردن مشاهده کنیم.
- با `confirmDequeue()` بعد از پردازش موفق، عنصر از صف خارج می‌شه.

## روال تست‌ها

برای بررسی عملکرد صف، سناریوهای زیر رو در نظر گرفتم.

### 1. تست اولیه و پر شدن صف (Queue Initialization & Overflow)

**روال تست:**

1. چهار مقدار `10`، `20`، `30` و `40` وارد صف می‌شن.
2. وضعیت صف با استفاده از `printQueue()` نمایش داده می‌شه.
3. مقدار پنجم (`50`) وارد صف می‌شه.
4. وضعیت صف مجدداً بررسی می‌شه.

**خروجی مورد انتظار:**

```
--- Queue Test ---
Size: 4 | Full: Yes | Empty: No | Dropped: 0

--- Overflow Test ---
Size: 4 | Full: Yes | Empty: No | Dropped: 1
```

**نتیجه:** با اضافه شدن مقدار پنجم، قدیمی‌ترین عنصر (`10`) حذف و مقدار جدید (`50`) جایگزین می‌شه. ظرفیت صف همچنان ۴ باقی می‌مونه و شمارنده حذف یک واحد افزایش پیدا می‌کنه.

### 2. تست حذف عنصر (Dequeue Test)

**روال تست:**

1. اولین عنصر صف با `peek()` مشاهده می‌شه.
2. مقدار مشاهده‌شده برای پردازش نمایش داده می‌شه.
3. پردازش موفق در نظر گرفته می‌شه.
4. با اجرای `confirmDequeue()` عنصر پردازش‌شده از صف خارج می‌شه.
5. وضعیت صف مجدداً نمایش داده می‌شه.

**خروجی مورد انتظار:**

```
--- Dequeue Test ---
Processing value: 20
Size: 3 | Full: No | Empty: No | Dropped: 1
```

**نتیجه:** مقدار `20` با موفقیت پردازش و از صف خارج می‌شه. اولین عنصر باقی‌مانده `30` خواهد بود.

### 3. تست پردازش ناموفق (Failed Processing Test)

**روال تست:**

1. اولین عنصر صف با `peek()` مشاهده می‌شه.
2. پردازش ناموفق در نظر گرفته می‌شه.
3. چون پردازش موفق نبوده، `confirmDequeue()` اجرا نمی‌شه.
4. وضعیت صف بررسی می‌شه.

**خروجی مورد انتظار:**

```
--- Failed Processing Test ---
Processing value: 30
Processing failed. Event remains in queue.
Size: 3 | Full: No | Empty: No | Dropped: 1
```

**نتیجه:** چون پردازش موفق نبوده، عنصر `30`  در صف باقی می‌مونه و برای تلاش مجدد قابل دسترسه.

### 4. تست بازگشت اندیس‌ها (Wrap-around Test)

این تست برای بررسی نحوه استفاده مجدد از خانه‌های آزادشده در **صف حلقوی** در نظر گرفته شده.

**روال تست:**

1. یک صف جدید با چهار مقدار `10`، `20`، `30` و `40` پر می‌شه.
2. دو عنصر اول با `confirmDequeue()` از صف خارج می‌شن.
3. دو مقدار جدید `50` و `60` وارد صف می‌شن.
4. با استفاده از `peek()` و `confirmDequeue()` تمام عناصر به ترتیب FIFO خوانده و خارج می‌شن.

**خروجی مورد انتظار:**

```
--- Wrap-around Test ---
Value: 30
Value: 40
Value: 50
Value: 60
```

**نتیجه:** اندیس `rear` پس از رسیدن به انتهای آرایه به ابتدای اون برمی‌گرده و از خانه‌های آزادشده استفاده می‌کنه؛ بدون اینکه نیازی به جابه‌جایی عناصر باشه.

### 5. تست چند چرخه متوالی (Multi-cycle Queue Test)

در این بخش چند چرخه متوالی از اضافه کردن، حذف کردن و پر شدن صف رو بررسی می‌کنیم.

#### **چرخه اول:**

1. مقادیر `1` تا `4` وارد صف می‌شن.
2. تمام عناصر به ترتیب از صف خارج می‌شن.
3. تعداد عناصر باقی‌مانده بررسی می‌شه.

**خروجی مورد انتظار:**

```
--- Multi-cycle Queue Test ---

Cycle 1:
Value: 1
Value: 2
Value: 3
Value: 4
Size: 0
```

#### **چرخه دوم:**

1. مقادیر `5` تا `8` وارد صف می‌شن.
2. تمام عناصر به ترتیب از صف خارج می‌شن.

**خروجی مورد انتظار:**

```
Cycle 2:
Value: 5
Value: 6
Value: 7
Value: 8
```

#### **چرخه سوم:**

1. مقادیر `9` تا `14` وارد صف می‌شن.
2. ظرفیت صف ۴ عنصره؛ بنابراین با ورود مقادیر پنجم و ششم، دو عنصر قدیمی‌تر حذف می‌شن.
3. تعداد عناصر و تعداد حذف‌شده‌ها بررسی می‌شه.
4. تمام عناصر باقی‌مانده به ترتیب از صف خارج می‌شن.

**خروجی مورد انتظار:**

```
Cycle 3: Overflow
Size: 4 | Dropped: 2
Value: 11
Value: 12
Value: 13
Value: 14
```

#### **چرخه چهارم:**

1. پس از خالی شدن صف، مقادیر `15` و `16` وارد می‌شن.
2. هر دو مقدار از صف خارج می‌شن.
3. در پایان تعداد عناصر باقی‌مانده و مجموع عناصر حذف‌شده بررسی می‌شه.

**خروجی مورد انتظار:**

```
Cycle 4:
Value: 15
Value: 16
Final Size: 0 | Total Dropped: 2
```

**نتیجه:** با اجرای چند چرخه متوالی، عملکرد اندیس‌ها، استفاده مجدد از فضای آرایه، حفظ ترتیب FIFO و مدیریت ظرفیت صف بررسی می‌شه.

### 6. تست تلاش مجدد برای پردازش (Processing Failure & Retry)

در این تست بررسی می‌کنیم که اگه پردازش یک عنصر ناموفق باشه، آیا در تلاش بعدی هنوز همون عنصر در دسترس قرار می‌گیره یا نه.

**روال تست:**

1. مقادیر `100`، `200` و `300` وارد صف می‌شن.
2. در تلاش اول، پردازش مقدار اول (`100`) ناموفق در نظر گرفته می‌شه.
3. در تلاش دوم، مجدداً اولین عنصر صف مشاهده و پردازش موفق در نظر گرفته می‌شه.
4. پس از حذف عنصر پردازش‌شده، مقدار بعدی صف مشاهده می‌شه.
5. تعداد عناصر باقی‌مانده بررسی می‌شه.

**خروجی مورد انتظار:**

```
--- Processing Failure Test ---
First attempt: 100
Second attempt: 100
Next item: 200
Remaining size: 2
```

**نتیجه:** در صورت ناموفق بودن پردازش، عنصر از صف خارج نمی‌شه و در تلاش بعدی مجدداً قابل دسترسه. تنها پس از پردازش موفق، با اجرای `confirmDequeue()` از صف خارج می‌شه.

## مقایسه عملکرد

|عملیات|صف خطی|صف حلقوی|
|---|---|---|
|اضافه کردن عنصر|O(1)|O(1)|
|اضافه کردن در حالت پر|O(n)|O(1)|
|مشاهده اولین عنصر|O(1)|O(1)|
|حذف اولین عنصر|O(n)|O(1)|

تفاوت اصلی این دو روش در نحوه مدیریت فضای خالی آرایه و جابه‌جایی عناصره.

## نتیجه

هر دو روش می‌تونن برای پیاده‌سازی صف استفاده بشن؛ اما در صف حلقوی، با استفاده از اندیس‌های `front` و `rear`، عملیات اضافه و حذف بدون جابه‌جایی عناصر انجام می‌شه.

به همین دلیل، صف حلقوی برای شرایطی که تعداد عملیات حذف و اضافه زیاده، انتخاب بهینه‌تریه.

---

**Platform:** ESP32 DevKit V1  
**Framework:** Arduino  
**Language:** C++




<html>
<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="../assets/XminD_logo_dark.webp">
    <source media="(prefers-color-scheme: light)" srcset="../assets/XminD_logo.webp">
    <img src="../assets/XminD_logo.webp" alt="XminD Logo" width="250">
  </picture>

</p>

<div align="center">
  <a href="https://t.me/X_MindWorld" target="_blank">
    <img src="https://img.shields.io/badge/Telegram-2CA5E0?style=for-the-badge&logo=telegram&logoColor=white" alt="Telegram" />
  </a>
  <a href="https://instagram.com/x_mindworld" target="_blank">
    <img src="https://img.shields.io/badge/Instagram-E4405F?style=for-the-badge&logo=instagram&logoColor=white" alt="Instagram" />
  </a>
  <a href="mailto:education.xmindworld@gmail.com" target="_blank">
    <img src="https://img.shields.io/badge/Gmail-D14836?style=for-the-badge&logo=gmail&logoColor=white" alt="Gmail" />
  </a>
</div>
</html>

<div dir="rtl" lang="he">

# מספרים עם יחידות - שלב א

בשנת 1999, לוויין של נאס"א בשווי של 125 מיליון דולר התרסק בגלל אי-התאמה ביחידות - אחד הצוותים שפיתחו את הלוויין עבד ביחידות מטריות והצוות השני עבד ביחידות בריטיות (ראו דוגמאות נוספות 
[כאן](http://mentalfloss.com/article/25845/quick-6-six-unit-conversion-disasters)).

כדי שזה לא יקרה שוב, הם שכרו אתכם וביקשו מכם לכתוב מחלקה המייצגת מספר עם יחידות. במחלקה הזאת אפשר, למשל, לייצג את המספר "3 מטר" ואת המספר "40 סנטימטר", והסכום שלהם לא יהיה 43 אלא 3.4 מטר - המחלקה תדאג לבצע את ההמרה המתאימה. בנוסף, המחלקה לא תאפשר לחבר מספרים עם מימדים לא תואמים, למשל, חיבור של "3 מטר" עם "5 שניות" יגרום לזריקת חריגה.

הגדירו מחלקה בשם `NumberWithUnits` עם הפעולות הבאות (ראו בקובץ המצורף [Demo.cpp](Demo.cpp)):

* פונקציה בשם `read_units`, הקוראת את היחידות מתוך קובץ טקסט. קובץ טקסט לדוגמה נמצא [כאן](units.txt). 
* שישה אופרטורים חשבוניים: חיבור (+) הוספה (+=) פלוס אונרי (+), ושלושת האופרטורים המקבילים לחיסור (-). כאמור, חיבור של שני מספרים מאותו מימד יתבצע תוך המרת היחידה של המספר השני ליחידה של המספר הראשון; חיבור של שני מספרים ממימדים שונים יגרום לחריגה.
* שישה אופרטורי השוואה: גדול, גדול-או-שווה, קטן, קטן-או-שווה, שווה, לא-שווה, לפי אותם כללים של האופרטורים החשבוניים.
* הגדלה ב-1 (++) והקטנה ב-1 (--) לפני ואחרי המספר.
* הכפלה במספר ממשי (`double`). שימו לב: אין צורך לממש הכפלה של שני עצמים מסוג `NumberWithUnits`,
אלא רק `NumberWithUnits` כפול `double`. ההכפלה לא משנה את היחידות.
* אופרטור קלט ואופרטור פלט.

הערות לגבי קובץ המרת היחידות:

* אין חשיבות לרווח-לבן (אפשר לדלג על רווחים).
* הקובץ כולל מספר פקודות של המרת-יחידות. כל פקודה מתחילה ב"1", אחריה שם של יחידה, אחריה סימן "=", אחריה מספר כלשהו, ואחריה שם של יחידה אחרת. ראו דוגמה בקובץ [units.txt](units.txt).
* יחידות שאינן נמצאות בקובץ-היחידות נחשבות לא חוקיות: ניסיון לאתחל מספר עם יחידות כאלו יביא לזריקת חריגה.
* יחידות המוגדרות בקובץ-היחידות נחשבות חוקיות: אין צורך לבדוק שהנתונים בקובץ מתאימים למציאות.
* יש הבדל בין אותיות גדולות לקטנות. לדוגמה, אם בקובץ מוגדר "km" אז רק "km" נחשב חוקי - לא "KM" או "Km".

הערות לגבי קלט ופלט:

* פורמט הפלט של מספר עם יחידות הוא: המספר, אחריו (בלי רווח) סוגריים מרובעים, ובתוכן היחידות. ראו דוגמה בקובץ Demo.cpp.
* פורמט הקלט הוא דומה, פרט לכך שמותר שיהיו רווחים לבנים (מותר לדלג על רווחים בקריאה).


בשלב א עליכם לכתוב: 

* קובץ כותרת הכולל את כל הפונקציות הדרושות (ללא מימוש). שימו לב: הכותרות צריכות להיות נכונות בהתאם למה שנלמד בהרצאות - מומלץ לחזור על החומר לפני שמתחילים לכתוב.
* בדיקות מקיפות לכל הפונקציות הדרושות.
   * אין צורך לבדוק קריאה של קובץ-יחידות עם פורמט שגוי.



כיתבו את כל הקבצים הדרושים כך שהפקודות הבאות יעבדו ללא שגיאות:

<div dir='ltr'>

    make demo && ./demo
	make test && ./test

</div>

מומלץ גם להריץ `make tidy`.

אין לשנות את הקבצים הנתונים, אלא רק להוסיף קבצים חדשים.

יש לפתור את המטלה באופן עצמאי.

* מותר להתייעץ עם סטודנטים אחרים או לחפש מידע באינטרנט;
אסור להעתיק קטעי-קוד שלמים מסטודנטים אחרים או מהאינטרנט.
* יש לדווח על כל עזרה שקיבלתם, מסטודנטים אחרים או מהאינטרנט, בהתאם ל[תקנון היושר של המחלקה](https://www.ariel.ac.il/wp/cs/wp-content/uploads/sites/88/2020/08/Guidelines-for-Academic-Integrity.pdf).

</div>

dos2unix grade

dos2unix grade_utils

bash grade

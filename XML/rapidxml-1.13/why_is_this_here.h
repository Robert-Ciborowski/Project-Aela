#pragma once

/*
 * You might ask: Why is there a copy of rapidxml inside the source code?
 * Answer: because I had to make a small modification to the library to get it
 * to not freak out if it stumbles upon a control character before it reaches
 * the next '<'.
*/
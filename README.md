# typify

Pretty simple, input like this in C#, or C++, or Java, or any language with this sort of syntax goes in:

    interface IExample{
      Dictionary<string, int>[] dictionary;
      int[] numbers;
    }
    
And you get this typescript out: 

    interface IExample{
      numbers : number[];
      dictionary : Dictionary<string,number>[];
    }
    
## Why are you using C++, why aren't you using regex, you know you could save a lot of time with...?

This project is just for me to get used to using C++ again, that's its only purpose. That is why issues are not resolved for quite a while, and why I chose not to develop like I normally would. If this project was only about creating a working product as fast as possible, I would not be using C++, and I would heavily use regex.

## Known Issues
- C# Property syntax (`{get; set;}`) does not work. Removing the `{get; set;}` fixes the issue.

---
![Logo](/images/logo_full.png)

# Made by Where 1

# typify

Pretty simple, input like this in C#, or C++, or Java, or any language with this sort of syntax goes in:

    interface IExample{
      Dictionary<string, int>[] dictionary;
      {
        int[] numbers;
      }
    }
    
And you get this typescript out: 

    interface IExample{
      {
        numbers : number[];
      }
      dictionary : Dictionary<string,number>[];
    }

## Known Issues
- C# Property syntax (`{get; set;}`) does not work. Removing the `{get; set;}` fixes the issue.

---
![Logo](/images/logo_full.png)

# Made by Where 1

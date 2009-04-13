#ifndef CREDITS_H
#define CREDITS_H

namespace CDH
{

class Credits
{
public:
   Credits();
   ~Credits();

   void queueline(const char* line);

   const char* firstLine() const;

   // returns NULL when end of credits reached
   const char* nextLine() const;
private:
   struct CreditsData;
   CreditsData* m_credits;
};

}





#endif // CREDITS_H
#pragma once

class Location: public Actor
{
public:
   Location(std::string name);
   virtual ~Location();

   // Actor overrides
   virtual void Render() = 0;
   virtual void Update(float dt) = 0;

   enum LocationType
   {
      Invalid = 0,
      Library,
      ArcaneSchool
   };

   LocationType Type;
   bool Functional;  

   const std::string& name() const {return m_name;}
private:
   std::string m_name;
};

%include <factory.i>

%factory(Renderable*, TextActor, ParticleActor, PhysicsActor, Camera, Actor);
%factory(DeveloperLog*, CompoundLog, SystemLog, FileLog, ConsoleLog);
%factory(MessageListener*, TextActor, ParticleActor, PhysicsActor, Camera, Actor);
%factory(Actor*, TextActor, ParticleActor, PhysicsActor, Camera);

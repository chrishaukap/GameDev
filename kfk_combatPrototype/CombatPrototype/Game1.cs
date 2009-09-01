using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;
using Microsoft.Xna.Framework.Net;
using Microsoft.Xna.Framework.Storage;

using CombatPrototype.GameWorld;
using CombatPrototype.Core;
using CombatPrototype.Core.TextRendering;
using CombatPrototype.Core.XMLProperties;

namespace CombatPrototype
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        public GameMouse mGameMouse;
        GameWorldCore mGameWorld;
        private TextOutput mTextOutput;
        private StaticTextOutput mStaticTextOutput;
        private XNA2DCamera mCamera;
        public XNA2DCamera Camera { get { return mCamera; } }
        
        public TextOutput TextOutputter
        { get { return mTextOutput; } }
        public StaticTextOutput StaticTextOutputter
        { get { return mStaticTextOutput; } }

        public ContentManager Content
        {
            get { return mContent; }
        }
        ContentManager mContent;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            mContent = new ContentManager(Services, "Content");
            mContent.RootDirectory = "Content";
            this.Components.Add(mTextOutput = new TextOutput(this));
            this.Components.Add(mStaticTextOutput = new StaticTextOutput(this));


            XMLPropertyReader.ReadXML();
        }

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            // TODO: Add your initialization logic here
            base.Initialize();

            mCamera = new XNA2DCamera(graphics.GraphicsDevice);
            mGameWorld = new GameWorldCore(graphics, mContent, this);
            mGameMouse = new GameMouse(graphics, mContent);
        }

        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        protected override void LoadContent()
        {

            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(graphics.GraphicsDevice);
            

            // TODO: use this.Content to load your game content here
        }

        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// all content.
        /// </summary>
        protected override void UnloadContent()
        {
            // TODO: Unload any non ContentManager content here
        }

        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            // TODO: Add your update logic here
            mGameMouse.Update();
            mGameWorld.Update();

            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            mGameWorld.Draw();
            mGameMouse.Draw();

            base.Draw(gameTime);
        }
    }
}

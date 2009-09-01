using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace CombatPrototype.Core
{
    public class RectangleF
    {
        public RectangleF(float left, float top, float width, float height)
        {
            m_left = left;
            m_top = top;
            m_width = width;
            m_height = height;        
        }

        public float Width { get { return m_width; } set { m_width = value; } }
        public float Height { get { return m_height; } set { m_height = value; } }
        public float Top { get { return m_top; } set { m_top = value; } }
        public float Left { get { return m_left; } set { m_left = value; } }
        private float m_left;
        private float m_top;
        private float m_width;
        private float m_height;
    }

    public class XNA2DCamera 
    {
        private GraphicsDevice m_gfxDevice;
        public XNA2DCamera(GraphicsDevice gfxDevice)
        {
            m_gfxDevice = gfxDevice;
            visibleArea = new RectangleF(0, 0, m_gfxDevice.Viewport.Width, m_gfxDevice.Viewport.Height);
            position = ScreenPosition;
        }

        protected Vector2 position = Vector2.Zero;
        public Vector2 Position
        {
            get { return position; }
            set
            {
                position = value;
                visibleArea.Left = position.X + offset.X - visibleArea.Width / 2;
                visibleArea.Top = position.Y + offset.Y - visibleArea.Height / 2;
            }
        }

        protected Vector2 offset = Vector2.Zero;
        public Vector2 Offset
        {
            get { return offset; }
            set
            {
                offset = value;
                visibleArea.Left = position.X + offset.X - visibleArea.Width / 2;
                visibleArea.Top = position.Y + offset.Y - visibleArea.Height / 2;
            }
        }

        protected RectangleF visibleArea;
        public RectangleF VisibleArea
        {
            get { return visibleArea; }
        }

        public float ViewingWidth
        {
            get { return visibleArea.Width; }
            set { visibleArea.Width = value; }
        }
        public float ViewingHeight
        {
            get { return visibleArea.Height; }
            set { visibleArea.Height = value; }
        }

        protected float rotation = 0.0f;
        public float Rotation
        {
            get { return rotation; }
            set { rotation = value; }
        }

        // 0 - <1 = Zoom Out
        // >1 = Zoom In
        // <0 = Funky (flips axis)
        protected Vector2 zoom = Vector2.One;
        public Vector2 Zoom
        {
            get { return zoom; }
            set 
            { 
                zoom = value;
                if (zoom.X < 0)
                    zoom.X = 0;
                if (zoom.Y < 0)
                    zoom.Y = 0;
            }
        }

        public Vector2 ScreenPosition
        {
            get { return new Vector2(m_gfxDevice.Viewport.Width / 2, m_gfxDevice.Viewport.Height / 2); }
        }
        
        /// Returns a transformation matrix based on the camera’s position, rotation, and zoom.
        /// Best used as a parameter for the SpriteBatch.Begin() call.
        public virtual Matrix ViewTransformationMatrix()
        {
            Vector3 matrixRotOrigin = new Vector3(Position + Offset, 0);
            Vector3 matrixScreenPos = new Vector3(ScreenPosition, 0.0f);

            // Translate back to the origin based on the camera’s offset position, since we’re rotating around the camera
            // Then, we scale and rotate around the origin
            // Finally, we translate to SCREEN coordinates, so translation is based on the ScreenCenter
            return Matrix.CreateTranslation(-matrixRotOrigin) *
                Matrix.CreateScale(Zoom.X, Zoom.Y, 1.0f) *
                Matrix.CreateRotationZ(Rotation) *
                Matrix.CreateTranslation(matrixScreenPos);
        }
    }
}
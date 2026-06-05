

    static void RB_DrawText( const char* text, const idVec3& origin, float scale, const idVec4& color, const idMat3& viewAxis, const int align )
    {

    // <snip/>

    idVec3 org, p1, p2;

    // <snip/>

    for( i = 0; i < len; i++ )
    {
      if( i == 0 || text[i] == '\n' )
      {
        org = origin - viewAxis[2] * ( line * 36.0f * scale );
        // <snip/>
      }

      org -= viewAxis[1] * ( spacing * scale );
    }



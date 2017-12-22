#ifndef LOCATION_H_
#define LOCATION_H_

enum FWDirection {
  N,  // Norte, arriba
  E,  // Este, derecha
  S,  // Sur, abajo
  W,  // Oeste, izquierda
  IMPOSSIBLE, // Imposible, error, no hay dirección...
};

FWDirection FDIRECTIONS[] = {N, E, S, W};

/*
    struct for representing locations in the grid.
*/
struct Location
{
  int row, col;

  Location()
  {
    row = col = 0;
  };

  Location(int r, int c)
  {
    row = r;
    col = c;
  };

  Location(const int *posFromArray)
  {
    this->col = posFromArray[0];
    this->row = posFromArray[1];
  }

  Location(const int *pos, const int *dir)
  {
    this->col = pos[0] + dir[0];
    this->row = pos[1] + dir[1];
  }

  Location(const int *pos, const FWDirection dir)
  {
    if (dir != IMPOSSIBLE)
    {
      this->col = pos[0] + DIRECTIONS[dir][0];
      this->row = pos[1] + DIRECTIONS[dir][1];
    }
    else
    {
      this->col = -1;
      this->row = -1;
    }
  }

  Location(const Location &pos, const FWDirection dir)
  {
    if (dir != IMPOSSIBLE)
    {
      this->col = pos.col + DIRECTIONS[dir][0];
      this->row = pos.row + DIRECTIONS[dir][1];
    }
    else
    {
      this->col = -1;
      this->row = -1;
    }
  }

  void wrap(const State &s)
  {
    col = (col + s.cols) % s.cols;
    row = (row + s.rows) % s.rows;
  }

  void wrap(size_t cols, size_t rows)
  {
    col = (col + s.cols) % cols;
    row = (row + s.rows) % rows;
  }

  Location operator+(const Location &other) const
  {
    Location result(*this);
    result.col += other.col;
    result.row += other.row;
    return result;
  }
};

#endif //LOCATION_H_

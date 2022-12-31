CREATE TABLE Actions (
  Action text NOT NULL,
  Data text DEFAULT NULL,
  PRIMARY KEY (Action)
);

CREATE TABLE Extensions (
  Extension text NOT NULL,
  Instruction text NOT NULL,
  IsFile boolean NOT NULL,
  PRIMARY KEY (Extension)
);

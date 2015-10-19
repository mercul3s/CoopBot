from sqlalchemy import Column, Integer, String, Time, create_engine
from sqlalchemy.ext.declarative import declarative_base

engine = create_engine('sqlite:///:memory:', echo=True)

Base = declarative_base()

class Chicken(Base):
	__tablename__ = "chickens"
	id = Column(Integer)
	chicken_id_tag = Column(Integer)
	name = Column(String)

	def __repr__(self):
        return "<Chicken(name='%s', chicken_id_tag='%s')>" % (
                             self.name, self.chicken_id_tag)

class DoorEvents(Base):
	__tablename__ = "door_events"
	id = Column(Integer)
	chicken_id_tag = Column(Integer)
	event = Column(String)
	timestamp = Column(Time)

class NestEvents(Base):
	__tablename__ = "nesting_box_events"
	id = Column(Integer)
	chicken_id_tag = Column(Integer)
	timestamp = Column(Time)
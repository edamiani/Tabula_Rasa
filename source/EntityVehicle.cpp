#include "../include/EntityVehicle.h"

namespace TR
{
	namespace Physics
	{
		// GearBox code
		GearBox::GearBox() 
		{ 
			TR::Physics::Gear gear;

			gear.ratio = -1.0;
			gear.minRpm = 0.0;
			gear.maxRpm = 0.0;
			mGears.push_back(gear); 

			gear.ratio = 0.0;
			gear.minRpm = 0.0;
			gear.maxRpm = 0.0;
			mGears.push_back(gear); 
			mCurrentGear = 0; 
			mGearsIterator = mGears.begin(); 
			mGearsIterator++;
			mNumberOfGears = 0;
		}

		void GearBox::gearDown() 
		{ 
			if(mCurrentGear > -1) 
			{ 
				mGearsIterator--; 
				mCurrentGear--; 
			} 
		}
			
		void GearBox::gearUp() 
		{ 
			mGearsIterator++; 
			if(mGearsIterator != mGears.end()) 
				mCurrentGear++;
			else
				mGearsIterator--;
		}

		int GearBox::getCurrentGear() 
		{ 
			return mCurrentGear; 
		}

		TR::Real GearBox::getCurrentGearRatio() 
		{ 
			return (*mGearsIterator).ratio; 
		}

		TR::Real GearBox::getCurrentGearMaxRpm() 
		{ 
			return (*mGearsIterator).maxRpm; 
		}

		TR::Real GearBox::getCurrentGearMinRpm() 
		{ 
			return (*mGearsIterator).minRpm; 
		}

		int GearBox::getNumberOfGears() 
		{ 
			return mNumberOfGears; 
		}

		void GearBox::insertGear(TR::Physics::Gear gear) 
		{ 
			mGears.push_back(gear); mNumberOfGears++; 
		}

		void GearBox::setReverseGear(TR::Physics::Gear gear) 
		{ 
			mGears.front() = gear; 
		}

		void GearBox::_setCurrentGear(int gear) 
		{ 
			mCurrentGear = gear; 
			mGearsIterator = mGears.begin();
			int i = -1;
			while (i < gear)
			{
				i++;
				mGearsIterator++;
			}
		}



		// Engine code
		Engine::Engine(TR::Physics::EngineDesc &engineDesc, TR::Physics::GearBox *gearBox) 
			: mGearBox(gearBox) 
		{
			mCurrentRpm = 0.0;
			mDifferentialRatio = engineDesc.differentialRatio;

			std::list<TR::Physics::TorquePoint *>::iterator it = engineDesc.torqueCurve.begin();
			for( ; it != engineDesc.torqueCurve.end(); it++)
				insertTorquePoint((*it)->rpm, (*it)->torque);
		}

		Engine::~Engine()
		{
			std::list<TR::Physics::TorquePoint *>::iterator it = mTorqueCurve.begin();
			for( ; it != mTorqueCurve.end(); it++)
				delete (*it);
		}

		TR::Real Engine::getCurrentRpm() 
		{ 
			return mCurrentRpm; 
		}
		
		TR::Real Engine::getDifferentialRatio() 
		{ 
			return mDifferentialRatio; 
		}

		TR::Real Engine::getMaxRpm() 
		{ 
			return mTorqueCurve.back()->rpm; 
		}

		TR::Real Engine::getMinRpm() 
		{ 
			return mTorqueCurve.front()->rpm; 
		}

		TR::Real Engine::getTorque(TR::Real rpm)
		{
			// First of all, find the real RPM
			// RPM is below the engine minimum RPM
			if(rpm < getMinRpm()) 
				mCurrentRpm = (getMinRpm() + 1.0);
			// Current RPM is higher than the maximum RPM
			else if(rpm > getMaxRpm() && mGearBox->getCurrentGear() < mGearBox->getNumberOfGears()) 
				mCurrentRpm = (getMaxRpm() - 1.0);
			else
				mCurrentRpm = rpm;

			std::list<TR::Physics::TorquePoint *>::iterator it1, it2;
			it1 = mTorqueCurve.begin();
			it2 = it1; it2++;

			for( ; it2 != mTorqueCurve.end(); it1++, it2++)
			{
				if(mCurrentRpm >= (*it1)->rpm && mCurrentRpm <= (*it2)->rpm)
				{
					TR::Real torque = (*it1)->torque + ((mCurrentRpm - (*it1)->rpm) * (*it1)->ratio);
					return torque;
				}
			}

			// RPM is above the maximum, return the maximum torque allowed
			it2--;
			return (*it2)->torque;
		}

		void Engine::insertTorquePoint(TR::Real rpm, TR::Real torque)
		{
			TR::Physics::TorquePoint *tp = new TR::Physics::TorquePoint(rpm, torque);
			tp->ratio = 0;

			if(mTorqueCurve.empty())
				mTorqueCurve.push_back(tp);
			else
			{
				mTorqueCurve.push_back(tp);
				std::list<TR::Physics::TorquePoint *>::iterator it1, it2;
				it1 = it2 = --(mTorqueCurve.end());
				it1--;
				(*it1)->ratio = ((*it2)->torque - (*it1)->torque) / ((*it2)->rpm - (*it1)->rpm);
			}
		}
	}
}
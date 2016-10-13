class ServerPinball {
public:
	static ServerPinball* instance(); 
	static void destroy(); 
	~ServerPinball();
	//остальные методы...

protected:
	ServerPinball(); 

private:
	//остальные методы...

private:
	static ServerPinball* _instance; 
	//остальные поля...
};
 

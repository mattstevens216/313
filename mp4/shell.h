class shell{
	private:
	std::vector<String> commands;
    std::vector<Queue> procs;
	
	public:
	shell();
	~shell();
	void update_prompt();
	void parse_Commands(int argc, char** argv)
	void tokenize_Commands();
	void run();
}
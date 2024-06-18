//
// Created by oleg on 31.3.24.
//
#include "Connection.h"

void processAuthentication(Client& obj,char* buffer, std::string& commandType, std::string& username, std::string& displayname, std::string& secret, int& counterForBye) {
    std::string extra; // Used to check for extra unwanted arguments in the input
    bool isValid = false; // Flag to track if the current input command is valid
    do {
        std::cin.getline(buffer, BUFFER_SIZE); // Read a line of input into the buffer
        if(cin.eof()){
            obj.sendClient("BYE\r\n");
            exit(EXIT_SUCCESS);
        }
        std::istringstream iss(buffer); // Use input string stream to parse the buffer content

        // Clear previous values to prepare for new input
        commandType.clear();
        username.clear();
        secret.clear();
        displayname.clear();
        extra.clear();

        iss >> commandType; // Extract the command type from the input

        // Process the "/auth" command for authentication
        if (commandType == "/auth") {
            iss >> username >> secret >> displayname >> extra; // Extract the necessary parameters and any extra input

            // Validate the extracted parameters and ensure no extra input was provided
            isValid = isValidUsername(username) && isValidDisplayName(displayname) && isValidSecret(secret) && extra.empty();

            if (!isValid) {
                // If validation fails, output an error and prompt for input again
                std::cerr << "ERR: Invalid input. Please try again.\n";
            }
        } else if (commandType == "/help") { // Process the "/help" command
            iss >> extra; // Check for extra input beyond the "/help" command
            if (extra.empty()) {
                // If no extra input, display help information
                std::cout << "HELP" << std::endl;
                std::cout << "/auth\t{Username} {Secret} {DisplayName}" << std::endl;
                std::cout << "/join\t{ChannelID}" << std::endl;
                std::cout << "/rename\t{DisplayName}" << std::endl;
                std::cout << "/help" << std::endl;
            } else {
                // If extra input is found, output an error
                std::cerr << "ERR: Invalid help instruction. Please write a correct one again" << std::endl;
            }
            isValid = true; // Mark input as valid to prevent re-prompting
        } else if (commandType == "BYE" && counterForBye != 0) { // Process the "BYE" command with conditions
            iss >> extra; // Check for extra input with the "BYE" command
            isValid = extra.empty(); // Validate that no extra arguments were provided
            if (!isValid) {
                // If extra arguments were found, output an error
                std::cerr << "ERR: Command BYE should be without arguments. Please try again." << std::endl;
            }
        } else {
            // If an unrecognized command is received, output an error
            std::cerr << "ERR: Command type should be /auth or /help. Please try again." << std::endl;
        }
    } while (!isValid); // Repeat the process until a valid command is entered

    // Once a valid "/auth" command is processed, construct the authentication message and increment the bye counter
    if (commandType == "/auth") {
        constructAuth(buffer, username, displayname, secret); // Construct the auth message in the buffer
        counterForBye++; // Increment the counter used for tracking "BYE" command usage
    }
}
bool joinChannelProcess(Client& obj,char* buffer, string& commandType, string& channelId, string& displayname){
    string extra; // Used to check for extra unwanted arguments in the input
    bool isValidJoinMessage = false; // Flag to track if the current input command is valid
    do{
        std::cin.getline(buffer, BUFFER_SIZE); // Read a line of input into the buffer
        if(cin.eof()){
            obj.sendClient("BYE\r\n");
            exit(EXIT_SUCCESS);
        }
        std::istringstream iss(buffer); // Use input string stream to parse the buffer content

        // Clear previous values to prepare for new input
        commandType.clear();
        channelId.clear();
        extra.clear();

        iss >> commandType; // Extract the command type from the input
        // Process the "/join" command
        if (commandType == "/join") {
            iss >> channelId >> extra; // Extract the channel ID and any extra input
            // Validate the channelId and ensure no extra input was provided
            if (!isValidChannelId(channelId) || !extra.empty() || channelId.empty()) {
                std::cerr << "ERR: Invalid input. Please try again.\n";
                return false; // Invalid input, exit function with false
            } else {
                constructJoin(buffer, channelId, displayname); // Construct join message if valid
                isValidJoinMessage = true; // Mark message as valid
            }
        } else if (commandType != "/join" && commandType != "BYE" && commandType != "/rename" && commandType != "/help") {
            // Process general messages that don't match special commands
            if (!isValidMessageContent(buffer)) {
                std::cerr << "ERR: Invalid input message. Please try again.\n";
                return false; // Invalid message content, exit function with false
            } else {
                constructMessage(buffer, displayname); // Construct message with display name if valid
                isValidJoinMessage = true; // Mark message as valid
            }
        } else if (commandType == "/help") {
            iss >> channelId; // Attempt to extract additional input after "/help"
            if (channelId.empty()) {
                // Display help information if no extra input is provided
                std::cout << "HELP" << std::endl;
                std::cout << "/auth\t{Username} {Secret} {DisplayName}" << std::endl;
                std::cout << "/join\t{ChannelID}" << std::endl;
                std::cout << "/rename\t{DisplayName}" << std::endl;
                std::cout << "/help" << std::endl;
            } else {
                std::cerr << "ERR: Invalid help instruction. Please write a correct one again" << std::endl;
            }
            isValidJoinMessage = true; // Mark help request as processed
        } else if (commandType == "/rename") {
            // Process the "/rename" command to change the display name
            iss >> channelId >> extra; // Reuse channelId to temporarily hold the new display name and check for extra input
            // Validate the new display name and ensure no extra input was provided
            if (!isValidDisplayName(channelId) || !extra.empty() || channelId.empty()) {
                std::cerr << "Invalid display name. Please write a correct one again.\n";
            } else {
                displayname = channelId; // Update display name if valid
            }
            isValidJoinMessage = true; // Mark rename request as processed
        }
    } while (!isValidJoinMessage); // Repeat until a valid command is processed

    return isValidJoinMessage; // Return true if a valid join message or other command was processed
}

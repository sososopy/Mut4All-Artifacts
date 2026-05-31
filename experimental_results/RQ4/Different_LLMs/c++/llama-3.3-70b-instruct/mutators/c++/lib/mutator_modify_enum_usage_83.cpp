//source file
#include "../include/Modify_Enum_Usage_83.h"
#include <random>

// ========================================================================================================
#define MUT83_OUTPUT 1

std::string getRandomBitwiseOperator() {
    std::string operators[] = {"|", "&", "^"};
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 2);
    return operators[dis(gen)];
}

std::string getRandomVariableOrConstant() {
    std::string variables[] = {"var1", "var2", "var3"};
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 2);
    return variables[dis(gen)];
}

void MutatorFrontendAction_83::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::UsingEnumDecl>("UsingEnum")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto enumName = MT->getEnumDecl()->getNameAsString();
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Find all occurrences of enum values used in bitwise operations within parentheses
      std::regex pattern("\\(([A-Za-z_][A-Za-z0-9_]*)\\s*([|&^])\\s*([A-Za-z_][A-Za-z0-9_]*)\\)");
      auto words_begin = std::sregex_iterator(declaration.begin(), declaration.end(), pattern);
      auto words_end = std::sregex_iterator();
      for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        // Check if the matched enum values are from the same enum
        if (match[1].str() == enumName || match[3].str() == enumName) {
          // Apply mutation by removing parentheses, changing bitwise operation, or replacing enum values
          int choice = rand() % 3;
          if (choice == 0) {
            // Remove parentheses
            declaration = std::regex_replace(declaration, std::regex("\\(" + match[1].str() + "\\s*" + match[2].str() + "\\s*" + match[3].str() + "\\)"), match[1].str() + " " + match[2].str() + " " + match[3].str());
          } else if (choice == 1) {
            // Change bitwise operation
            declaration = std::regex_replace(declaration, std::regex(match[2].str()), getRandomBitwiseOperator());
          } else {
            // Replace enum values with existing variables or constants
            declaration = std::regex_replace(declaration, std::regex(match[1].str()), getRandomVariableOrConstant());
          }
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_83::MutatorASTConsumer_83::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = usingEnumDecl().bind("UsingEnum");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
//source file
#include "../include/Mismatch_Template_Args_In_Parameter_Pack_Call_382.h"

// ========================================================================================================
#define MUT382_OUTPUT 1

void MutatorFrontendAction_382::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      
      const clang::FunctionDecl *FD = CE->getDirectCallee();
      if (!FD || !FD->isTemplateInstantiation()) return;
      
      const clang::TemplateDecl *TD = FD->getTemplateInstantiationPattern()->getDescribedTemplate();
      if (!TD) return;
      
      const clang::TemplateParameterList *TPL = TD->getTemplateParameters();
      if (!TPL || !TPL->containsUnexpandedParameterPack()) return;
      
      //Get the source code text of target node
      std::string callText = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
      
      //Find template argument list in call
      size_t templateStart = callText.find('<');
      if (templateStart == std::string::npos) return;
      
      size_t templateEnd = callText.find('>', templateStart);
      if (templateEnd == std::string::npos) return;
      
      std::string templateArgs = callText.substr(templateStart + 1, templateEnd - templateStart - 1);
      
      //Count existing template arguments
      std::vector<std::string> argList;
      size_t pos = 0;
      while (pos < templateArgs.length()) {
        size_t commaPos = templateArgs.find(',', pos);
        if (commaPos == std::string::npos) {
          argList.push_back(templateArgs.substr(pos));
          break;
        }
        argList.push_back(templateArgs.substr(pos, commaPos - pos));
        pos = commaPos + 1;
      }
      
      //Perform mutation on the source code text by applying string replacement
      int mutationType = getrandom::getRandomIndex(2); // 0: add extra, 1: remove some, 2: change type
      std::string newTemplateArgs;
      
      if (mutationType == 0) { // Add extra template argument
        for (const std::string &arg : argList) {
          newTemplateArgs += arg + ", ";
        }
        //Add a random fundamental type
        std::string extraType;
        int typeChoice = getrandom::getRandomIndex(4);
        switch(typeChoice) {
          case 0: extraType = "int"; break;
          case 1: extraType = "float"; break;
          case 2: extraType = "char"; break;
          case 3: extraType = "bool"; break;
          case 4: extraType = "double"; break;
        }
        newTemplateArgs += extraType;
      } else if (mutationType == 1 && argList.size() > 1) { // Remove some template arguments
        int removeCount = getrandom::getRandomIndex(argList.size() - 1);
        for (size_t i = 0; i < argList.size() - removeCount; ++i) {
          newTemplateArgs += argList[i];
          if (i < argList.size() - removeCount - 1) newTemplateArgs += ", ";
        }
      } else { // Change type of one template argument
        int changeIndex = getrandom::getRandomIndex(argList.size() - 1);
        for (size_t i = 0; i < argList.size(); ++i) {
          if (i == changeIndex) {
            std::string newType;
            int typeChoice = getrandom::getRandomIndex(4);
            switch(typeChoice) {
              case 0: newType = "int"; break;
              case 1: newType = "float"; break;
              case 2: newType = "char"; break;
              case 3: newType = "bool"; break;
              case 4: newType = "double"; break;
            }
            newTemplateArgs += newType;
          } else {
            newTemplateArgs += argList[i];
          }
          if (i < argList.size() - 1) newTemplateArgs += ", ";
        }
      }
      
      std::string mutatedCall = callText.substr(0, templateStart + 1) + newTemplateArgs + callText.substr(templateEnd);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedCall);
    }
}
  
void MutatorFrontendAction_382::MutatorASTConsumer_382::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(hasAncestor(functionDecl()), callee(functionDecl(isTemplateInstantiation()))).bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
//source file
#include "../include/modify_function_parameter_to_enum_18.h"

// ========================================================================================================
#define MUT18_OUTPUT 1

void MutatorFrontendAction_18::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      if (FD->getNumParams() == 0)
        return;

      //Get the source code text of target node
      auto &SM = *Result.SourceManager;
      auto &LangOpts = Result.Context->getLangOpts();
      auto FuncRange = FD->getSourceRange();
      std::string FuncText = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(FuncRange), SM, LangOpts).str();

      //Perform mutation on the source code text by applying string replacement
      std::string EnumDecl = "enum NewEnum { val1, val2 }; ";
      std::string ParamType = "NewEnum";
      auto FirstParam = FD->parameters()[0];
      auto ParamRange = FirstParam->getSourceRange();
      std::string ParamText = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(ParamRange), SM, LangOpts).str();

      size_t Pos = FuncText.find(ParamText);
      if (Pos != std::string::npos) {
          FuncText.replace(Pos, ParamText.length(), ParamType + " " + FirstParam->getNameAsString());
          //Insert the enum declaration at the beginning of the class definition
          if (auto *CD = FD->getParent()) {
              if (CD->isCompleteDefinition()) {
                  auto ClassStart = CD->getBraceRange().getBegin();
                  Rewrite.InsertTextAfterToken(ClassStart, "\n" + EnumDecl);
              }
          }
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(FuncRange, FuncText);
    }
}
  
void MutatorFrontendAction_18::MutatorASTConsumer_18::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isDefinition()).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
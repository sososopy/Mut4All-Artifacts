//source file
#include "../include/Mutator_Remove_Friend_Function_Default_Argument_219.h"

// ========================================================================================================
#define MUT219_OUTPUT 1

void MutatorFrontendAction_219::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FriendDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if it's a friend declaration inside a templated class/struct
      if (!FD->getFriendObjectKind())
        return;
      const clang::DeclContext *DC = FD->getDeclContext();
      if (!DC)
        return;
      const clang::CXXRecordDecl *RD = dyn_cast<clang::CXXRecordDecl>(DC);
      if (!RD || !RD->getDescribedClassTemplate())
        return;
      //Check if function has default arguments
      unsigned numParams = FD->getNumParams();
      if (numParams == 0)
        return;
      //Find the last parameter with a default argument
      int lastDefaultIdx = -1;
      for (unsigned i = 0; i < numParams; ++i) {
        const clang::ParmVarDecl *PVD = FD->getParamDecl(i);
        if (PVD && PVD->hasDefaultArg()) {
          lastDefaultIdx = i;
        }
      }
      if (lastDefaultIdx == -1)
        return;
      //Get the source code text of target node
      std::string funcDeclStr = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
      //Find the parameter with default argument in the source text
      const clang::ParmVarDecl *targetPVD = FD->getParamDecl(lastDefaultIdx);
      if (!targetPVD)
        return;
      std::string paramStr = stringutils::rangetoStr(*(Result.SourceManager), targetPVD->getSourceRange());
      //Remove the default argument part
      size_t eqPos = paramStr.find('=');
      if (eqPos == std::string::npos)
        return;
      std::string newParamStr = paramStr.substr(0, eqPos);
      //Trim trailing spaces
      while (newParamStr.back() == ' ')
        newParamStr.pop_back();
      //Perform mutation on the source code text by applying string replacement
      size_t paramPos = funcDeclStr.find(paramStr);
      if (paramPos == std::string::npos)
        return;
      funcDeclStr.replace(paramPos, paramStr.length(), newParamStr);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcDeclStr);
    }
}
  
void MutatorFrontendAction_219::MutatorASTConsumer_219::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasParent(cxxRecordDecl(isTemplateInstantiation()))).bind("FriendDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
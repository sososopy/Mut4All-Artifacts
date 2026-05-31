//source file
#include "../include/Reference_Init_Change_157.h"

// ========================================================================================================
#define MUT157_OUTPUT 1

void MutatorFrontendAction_157::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecls")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   VD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (VD->getType()->isReferenceType()) {
        std::string typeStr = VD->getType().getAsString();
        size_t pos = typeStr.find('&');
        if (pos != std::string::npos) {
          typeStr.erase(pos, 1);
          declaration.replace(declaration.find(VD->getType().getAsString()), 
                             VD->getType().getAsString().length(), typeStr);
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_157::MutatorASTConsumer_157::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(referenceType())).bind("VarDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
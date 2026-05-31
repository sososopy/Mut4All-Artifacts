//source file
#include "../include/invalid_pointer_dereference_in_initialization_8.h"

// ========================================================================================================
#define MUT8_OUTPUT 1

void MutatorFrontendAction_8::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (VD->hasInit() && VD->getInit()->isIntegerConstantExpr(*Result.Context)) {
        //Get the source code text of target node
        QualType type = VD->getType();
        std::string typeName = type.getAsString();
        std::string mutatedInit = "*" + ("reinterpret_cast<" + typeName + "*>(nullptr)");
        //Perform mutation on the source code text by applying string replacement
        Rewrite.ReplaceText(VD->getInit()->getSourceRange(), mutatedInit);
      }
    }
}
  
void MutatorFrontendAction_8::MutatorASTConsumer_8::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasInitializer(integerLiteral())).bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
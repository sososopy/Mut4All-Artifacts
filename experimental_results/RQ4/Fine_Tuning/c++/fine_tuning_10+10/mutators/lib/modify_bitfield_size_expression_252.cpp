//source file
#include "../include/modify_bitfield_size_expression_252.h"

// ========================================================================================================
#define MUT252_OUTPUT 1

void MutatorFrontendAction_252::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BD = Result.Nodes.getNodeAs<clang::FieldDecl>("bitfieldDecl")) {
      //Filter nodes in header files
      if (!BD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BD->getLocation()))
        return;

      if (BD->isBitField()) {
        //Get the source code text of target node
        auto bitWidthExpr = BD->getBitWidth();
        if (!bitWidthExpr)
          return;
        
        //Perform mutation on the source code text by applying string replacement
        std::string newExpr = "1 + (unsigned long long)0xffffffff";
        Rewrite.ReplaceText(bitWidthExpr->getSourceRange(), newExpr);
      }
    }
}
  
void MutatorFrontendAction_252::MutatorASTConsumer_252::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl(isBitField()).bind("bitfieldDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
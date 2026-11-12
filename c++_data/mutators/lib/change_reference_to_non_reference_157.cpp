//source file
#include "../include/change_reference_to_non_reference_157.h"

// ========================================================================================================
#define MUT157_OUTPUT 1

void MutatorFrontendAction_157::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Decl = Result.Nodes.getNodeAs<clang::VarDecl>("refVarDecl")) {
      //Filter nodes in header files
      if (!Decl || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Decl->getLocation()))
        return;
      
      // Get the source code text of target node
      auto type = Decl->getType().getNonReferenceType();
      auto expr = Decl->getInit();
      if (!expr)
        return;

      // Perform mutation on the source code text by applying string replacement
      std::string newDecl = type.getAsString() + " " + Decl->getNameAsString() + " = " + Lexer::getSourceText(CharSourceRange::getTokenRange(expr->getSourceRange()), *Result.SourceManager, Result.Context->getLangOpts()).str() + ";";
      
      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Decl->getSourceRange()), newDecl);
    }
}
  
void MutatorFrontendAction_157::MutatorASTConsumer_157::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(references(qualType())), hasInitializer(expr())).bind("refVarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
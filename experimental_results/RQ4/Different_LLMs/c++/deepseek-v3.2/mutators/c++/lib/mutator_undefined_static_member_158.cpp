//source file
#include "../include/Mutator_Undefined_Static_Member_158.h"

// ========================================================================================================
#define MUT158_OUTPUT 1

void MutatorFrontendAction_158::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      declaration.insert(declaration.rfind("}"), "\nstatic constexpr bool bar_undefined;");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), declaration);
    }
}
  
void MutatorFrontendAction_158::MutatorASTConsumer_158::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isClass(), unless(hasAttr(attr::Annotate))).bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
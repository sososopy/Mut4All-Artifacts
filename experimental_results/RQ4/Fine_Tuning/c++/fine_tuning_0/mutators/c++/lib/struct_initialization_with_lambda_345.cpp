//source file
#include "../include/Struct_Initialization_With_Lambda_345.h"

// ========================================================================================================
#define MUT345_OUTPUT 1

void MutatorFrontendAction_345::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FieldDecl>("Field")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto fieldname = stringutils::rangetoStr(*(Result.SourceManager),
                                               MT->getSourceRange());
      auto init = MT->getInClassInitializer();
      if (init == nullptr)
        return;
      auto init_str =
          stringutils::rangetoStr(*(Result.SourceManager), init->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto lambda_init = "[=](){ return " + init_str + "; }()";
      fieldname.replace(fieldname.find(init_str), init_str.size(), lambda_init);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          fieldname);
    }
}
  
void MutatorFrontendAction_345::MutatorASTConsumer_345::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl().bind("Field");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
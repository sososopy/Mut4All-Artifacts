//source file
#include "../include/Alter_Class_Template_Specialization_147.h"

// ========================================================================================================
#define MUT147_OUTPUT 1

void MutatorFrontendAction_147::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto name = MT->getNameAsString();
      auto specs = MT->specializations();
      if (specs.empty())
        return;
      auto partial_specs = MT->specializations();
      if (partial_specs.empty())
        return;
      llvm::outs() << name << '\n';
      for (auto spec : partial_specs) {
        //Perform mutation on the source code text by applying string replacement
        auto partial_spec = stringutils::rangetoStr(*(Result.SourceManager),
                                                    spec->getSourceRange());
        partial_spec = "/*mut147*/" + partial_spec;
        llvm::outs() << partial_spec << '\n';
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(spec->getEndLoc(), 0, partial_spec);
      }
    }
}
  
void MutatorFrontendAction_147::MutatorASTConsumer_147::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
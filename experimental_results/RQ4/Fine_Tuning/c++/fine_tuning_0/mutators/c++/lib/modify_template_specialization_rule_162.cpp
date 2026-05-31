//source file
#include "../include/Modify_Template_Specialization_Rule_162.h"

// ========================================================================================================
#define MUT162_OUTPUT 1

void MutatorFrontendAction_162::Callback::run(const MatchFinder::MatchResult &Result) {
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
      auto spec = specs.begin();
      auto spec_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              (*spec)->getSourceRange());
      llvm::outs() << spec_str << '\n';
      auto pos = spec_str.find('<');
      if (pos == string::npos)
        return;
      //Perform mutation on the source code text by applying string replacement
      spec_str.erase(pos);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText((*spec)->getSourceRange(), spec_str);
    }
}
  
void MutatorFrontendAction_162::MutatorASTConsumer_162::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
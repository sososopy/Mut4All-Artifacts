//source file
#include "../include/Add_Nested_Template_With_NTTP_454.h"

// ========================================================================================================
#define MUT454_OUTPUT 1

void MutatorFrontendAction_454::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Template")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto name = MT->getNameAsString();
      llvm::outs() << name << '\n';
      if (MT->getTemplateParameters()->size() == 0)
        return;
      if (MT->getTemplateParameters()->getParam(0)->getKind() !=
          Decl::Kind::NonTypeTemplateParm)
        return;
      //Perform mutation on the source code text by applying string replacement
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      if (content.rfind('}') != string::npos) {
        auto ins = "template <typename U, char C> struct Inner {};";
        content.insert(content.rfind('}'), ins);
      }
      llvm::outs() << content << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_454::MutatorASTConsumer_454::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
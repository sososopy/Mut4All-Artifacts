//source file
#include "../include/Target_Templated_TypeAlias_Attribute_392.h"

// ========================================================================================================
#define MUT392_OUTPUT 1

void MutatorFrontendAction_392::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("TemplatedAlias")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      if (!TD->isInIdentifierNamespace(Decl::IDNS_Ordinary))
        return;

      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             TD->getSourceRange());
      auto TAD = TD->getTemplatedDecl();
      auto aliased_type = TAD->getUnderlyingType();
      if (!aliased_type->isPointerType())
        return;
      auto N = TD->getTemplateParameters()->size();
      llvm::outs() << "N: " << N << '\n';
      auto alias_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                   TAD->getSourceRange());
      llvm::outs() << alias_content << '\n';
      if (alias_content.find("[[") == string::npos)
        return;
      auto varname = "mut_392_var";
      auto ins_content = content + " " + alias_content + " " + varname;
      llvm::outs() << ins_content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()),
                          ins_content);
    }
}
  
void MutatorFrontendAction_392::MutatorASTConsumer_392::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("TemplatedAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
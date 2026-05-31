//source file
#include "../include/Invalid_Alias_Template_Declaration_In_Class_Template_Context_131.h"

// ========================================================================================================
#define MUT131_OUTPUT 1

void MutatorFrontendAction_131::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *AT = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("AliasTemplate")) {
      if (!AT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AT->getLocation()))
        return;
      if (AT->isInvalidDecl())
        return;
      if (AT->getDeclContext() != class_template->getTemplatedDecl())
        return;
      auto alias_name = AT->getNameAsString();
      auto alias_type = stringutils::rangetoStr(*(Result.SourceManager),
                                                AT->getTypeSourceInfo()->getTypeLoc().getSourceRange());
      if (alias_name == "" || alias_type == "")
        return;
      auto alias_decl = "/*mut131*/template<typename K> using " + alias_name +
                        "=" + alias_type + ";";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(AT->getSourceRange()),
                          alias_decl);
    } else if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                   "ClassTemplate")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      class_template = CT;
    }
}

void MutatorFrontendAction_131::MutatorASTConsumer_131::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto alias_template_matcher = typeAliasDecl().bind("AliasTemplate");
    auto class_template_matcher = classTemplateDecl().bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(alias_template_matcher, &callback);
    matchFinder.addMatcher(class_template_matcher, &callback);
    matchFinder.matchAST(Context);
}
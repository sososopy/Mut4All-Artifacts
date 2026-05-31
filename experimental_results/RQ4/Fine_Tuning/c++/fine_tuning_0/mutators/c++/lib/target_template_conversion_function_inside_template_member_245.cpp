//source file
#include "../include/Target_Template_Conversion_Function_Inside_Template_Member_245.h"

// ========================================================================================================
#define MUT245_OUTPUT 1

void MutatorFrontendAction_245::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Templates")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      if ((!TD->isStruct() && !TD->isClass()) || TD->isLambda())
        return;
      if (!TD->isCompleteDefinition())
        return;
      if (TD->getDescribedClassTemplate() == nullptr)
        return;

      auto methods = TD->methods();
      bool has_conversion_operator_template = false;
      for (auto method : methods) {
        if (method->getDescribedFunctionTemplate() == nullptr)
          continue;
        if (llvm::dyn_cast<CXXConversionDecl>(method) == nullptr)
          continue;
        has_conversion_operator_template = true;
      }
      if (!has_conversion_operator_template)
        return;

      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
      auto insert_pos = content.find_last_of('}');
      if (insert_pos == string::npos)
        return;
      auto insert_content = "void f();";
      content.insert(insert_pos, insert_content);
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), content);
    } else if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                   "TemplateDecl")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      auto TDI = TD->getTemplatedDecl();
      auto methods = TDI->methods();
      bool has_conversion_operator_template = false;
      for (auto method : methods) {
        if (method->getDescribedFunctionTemplate() == nullptr)
          continue;
        if (llvm::dyn_cast<CXXConversionDecl>(method) == nullptr)
          continue;
        has_conversion_operator_template = true;
      }
      if (!has_conversion_operator_template)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
      auto insert_pos = content.find_last_of('}');
      if (insert_pos == string::npos)
        return;
      auto insert_content = "void A::f(){0,operator int();}";
      content.insert(insert_pos, insert_content);
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_245::MutatorASTConsumer_245::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Templates");
    auto template_matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.matchAST(Context);
}
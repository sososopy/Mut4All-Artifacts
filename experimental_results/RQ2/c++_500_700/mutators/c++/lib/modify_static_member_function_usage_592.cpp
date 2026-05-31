//source file
#include "../include/modify_static_member_function_usage_592.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

// ========================================================================================================
#define MUT592_OUTPUT 1

void MutatorFrontendAction_592::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("StaticMethod")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      if (MT->isStatic() && MT->doesThisDeclarationHaveABody()) {
        if (MT->hasAttr<clang::NoThrowAttr>()) {
          std::string modifiedNoexcept = "noexcept(true)";
          auto attr = MT->getAttr<clang::NoThrowAttr>();
          Rewrite.ReplaceText(attr->getRange(), modifiedNoexcept);
        }
      }
    }
}
  
void MutatorFrontendAction_592::MutatorASTConsumer_592::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(isStaticStorageClass(), isTemplateInstantiation()).bind("StaticMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
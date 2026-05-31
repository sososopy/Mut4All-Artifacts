//source file
#include "../include/modify_explicit_object_parameter_2.h"

// ========================================================================================================
#define MUT2_OUTPUT 1

void MutatorFrontendAction_2::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->getNumParams() > 0 && !MT->isVirtual()) {
        auto param = MT->getParamDecl(0);
        if (param->getType()->isReferenceType()) {
          auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                     MT->getSourceRange());
          auto paramTypeStr = param->getType().getAsString();
          std::string newParamType = "this const int&";
          std::string oldParamType = paramTypeStr + " " + param->getNameAsString();
          std::string newParam = newParamType + " " + param->getNameAsString();
          size_t pos = declaration.find(oldParamType);
          if (pos != std::string::npos) {
            declaration.replace(pos, oldParamType.length(), newParam);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_2::MutatorASTConsumer_2::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(unless(cxxConstructorDecl())).bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
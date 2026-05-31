//source file
#include "../include/Add_Invalid_Constraints_In_Template_Struct_Ctors_316.h"

// ========================================================================================================
#define MUT316_OUTPUT 1

void MutatorFrontendAction_316::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Ctors")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (!DL->isInheritingConstructor() && DL->isExplicit() == false)
        return;
      if (DL->getNumParams() == 0)
        return;
      if (DL->getTemplateSpecializationArgs() != nullptr)
        return;
      if (!DL->getDescribedFunctionTemplate())
        return;
      if (!DL->getType()->isDependentType())
        return;
      auto requiresclause = DL->getTrailingRequiresClause();
      if (!requiresclause)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             requiresclause->getSourceRange());
      if (content.find("requires") != string::npos)
        content = "/*mut316*/requires requires(T t) { []<typename Y>(Y&){}(z); }";
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(requiresclause->getSourceRange()),
          content);
    }
  }

void MutatorFrontendAction_316::MutatorASTConsumer_316::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl().bind("Ctors");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
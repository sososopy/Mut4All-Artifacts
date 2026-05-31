//source file
#include "../include/array_initialization_with_complex_type_418.h"

// ========================================================================================================
#define MUT418_OUTPUT 1

void MutatorFrontendAction_418::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("ArrayDecl")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->hasInit() == false)
      return;
    auto type = DL->getType();
    if (type->isArrayType() == false)
      return;
    auto arraytype = type->getAsArrayTypeUnsafe();
    if (arraytype == nullptr)
      return;
    auto elemtype = arraytype->getElementType();
    if (elemtype->isStructureOrClassType() == false)
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    if (content.rfind(';') != string::npos)
      content.insert(content.rfind(';'), "/*mut418*/={}");
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                        content);
  }
}

void MutatorFrontendAction_418::MutatorASTConsumer_418::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl().bind("ArrayDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
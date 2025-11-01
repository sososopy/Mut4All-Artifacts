//source file
#include "../include/Struct_With_Nested_Lambdas_345.h"

// ========================================================================================================
#define MUT345_OUTPUT 1

void MutatorFrontendAction_345::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("memberInit")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (auto *IL = dyn_cast<clang::InitListExpr>(VD->getInit())) {
        if (IL->getNumInits() == 1) {
          if (auto *LE = dyn_cast<clang::LambdaExpr>(IL->getInit(0))) {
            auto structDecl = dyn_cast<clang::CXXRecordDecl>(VD->getDeclContext());
            if (!structDecl) return;

            std::string structMemberName;
            for (auto *field : structDecl->fields()) {
              if (field->getType()->isIntegerType()) {
                structMemberName = field->getNameAsString();
                break;
              }
            }
            if (structMemberName.empty()) return;

            std::string lambdaText = stringutils::rangetoStr(*(Result.SourceManager), LE->getSourceRange());
            std::string nestedLambda = "(10, [=]() { return [=]() { return " + structMemberName + "; }(); }())";

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(IL->getSourceRange()), nestedLambda);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_345::MutatorASTConsumer_345::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasInitializer(initListExpr(has(lambdaExpr())))).bind("memberInit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
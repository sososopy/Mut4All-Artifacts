//source file
#include "../include/Modify_Array_Declaration_With_Negative_Size_348.h"

// ========================================================================================================
#define MUT348_OUTPUT 1

void MutatorFrontendAction_348::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        if (VD->getType()->isConstantArrayType()) {
            const auto *CAT = Result.Context->getAsConstantArrayType(VD->getType());
            if (CAT) {
                auto sizeExpr = CAT->getSizeExpr();
                if (sizeExpr) {
                    std::string newSize = "-5";
                    auto sizeRange = sizeExpr->getSourceRange();
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(sizeRange), newSize);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_348::MutatorASTConsumer_348::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(constantArrayType())).bind("arrayDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
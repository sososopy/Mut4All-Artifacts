//source file
#include "../include/template_function_pointer_dereference_625.h"

// ========================================================================================================
#define MUT625_OUTPUT 1

void MutatorFrontendAction_625::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("TemplateFunction")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       MT->getLocation()))
            return;

        if (MT->isTemplateInstantiation()) {
            auto body = MT->getBody();
            for (auto &stmt : body->children()) {
                if (auto *castExpr = llvm::dyn_cast<clang::CStyleCastExpr>(stmt)) {
                    auto castType = castExpr->getTypeAsWritten();
                    if (castType->isPointerType()) {
                        std::string replacement = "*(" + castExpr->getSubExprAsWritten()->getSourceRange().getBegin().printToString(Result.Context->getSourceManager()) + ") = 0;";
                        Rewrite.ReplaceText(castExpr->getSourceRange(), replacement);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_625::MutatorASTConsumer_625::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(isTemplateInstantiation()).bind("TemplateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
//source file
#include "../include/misuse_template_keywords_nested_instantiation_233.h"

// ========================================================================================================
#define MUT233_OUTPUT 1

void MutatorFrontendAction_233::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                FD->getLocation()))
            return;

        for (auto *Body : FD->getBody()->children()) {
            if (auto *Expr = dyn_cast<clang::UnaryOperator>(Body)) {
                if (Expr->getOpcode() == clang::UO_AddrOf) {
                    auto *SubExpr = Expr->getSubExpr()->IgnoreParenCasts();
                    if (auto *MemExpr = dyn_cast<clang::MemberExpr>(SubExpr)) {
                        auto SourceText = stringutils::rangetoStr(
                            *(Result.SourceManager), MemExpr->getSourceRange());
                        size_t pos = SourceText.find("template ");
                        if (pos != std::string::npos) {
                            SourceText.erase(pos, 9); // Remove 'template ' keyword
                            Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                MemExpr->getSourceRange()), SourceText);
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_233::MutatorASTConsumer_233::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasDescendant(
        unaryOperator(hasOperatorName("&"),
                      hasUnaryOperand(memberExpr(hasDescendant(templateSpecializationType()))))
    )).bind("FuncDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
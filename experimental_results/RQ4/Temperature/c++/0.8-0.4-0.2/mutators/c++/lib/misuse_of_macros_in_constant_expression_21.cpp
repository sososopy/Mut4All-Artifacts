//source file
#include "../include/misuse_of_macros_in_constant_expression_21.h"

// ========================================================================================================
#define MUT21_OUTPUT 1

void MutatorFrontendAction_21::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Macros")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (FD->isDefined() && FD->getNumParams() > 0) {
            macros.push_back(FD);
        }
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::VarDecl>("ConstExpr")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getLocation()))
            return;
        if (CE->isConstexpr()) {
            if (!macros.empty()) {
                const clang::FunctionDecl *macro = macros.back();
                std::string macroName = macro->getNameAsString();
                std::string mutatedExpr = macroName + "(10, 5)"; // Example usage
                Rewrite.ReplaceText(CE->getInit()->getSourceRange(), mutatedExpr);
            }
        }
    }
}

void MutatorFrontendAction_21::MutatorASTConsumer_21::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher macroMatcher = functionDecl(isExpansionInMainFile()).bind("Macros");
    DeclarationMatcher constExprMatcher = varDecl(hasInitializer(integerLiteral())).bind("ConstExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(macroMatcher, &callback);
    matchFinder.addMatcher(constExprMatcher, &callback);
    matchFinder.matchAST(Context);
}
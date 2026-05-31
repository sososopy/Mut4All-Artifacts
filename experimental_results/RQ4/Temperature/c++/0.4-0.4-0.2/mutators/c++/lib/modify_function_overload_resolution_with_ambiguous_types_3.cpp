//source file
#include "../include/modify_function_overload_resolution_with_ambiguous_types_3.h"

// ========================================================================================================
#define MUT3_OUTPUT 1

void MutatorFrontendAction_3::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->isOverloadedOperator() || FD->isTemplateInstantiation())
            return;

        overloadedFunctions.push_back(FD);
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getExprLoc()))
            return;

        for (auto *FD : overloadedFunctions) {
            if (FD->getNameInfo().getName().getAsString() == CE->getDirectCallee()->getNameInfo().getName().getAsString()) {
                auto args = CE->getArgs();
                for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
                    if (args[i]->getType()->isIntegerType()) {
                        std::string replacement = std::to_string(42.0f);
                        Rewrite.ReplaceText(args[i]->getSourceRange(), replacement);
                        break;
                    }
                }
                break;
            }
        }
    }
}

void MutatorFrontendAction_3::MutatorASTConsumer_3::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionMatcher = functionDecl(anyOf(cxxMethodDecl(), functionDecl())).bind("Functions");
    StatementMatcher callExprMatcher = callExpr(callee(functionDecl(anyOf(cxxMethodDecl(), functionDecl())))).bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(callExprMatcher, &callback);
    matchFinder.matchAST(Context);
}
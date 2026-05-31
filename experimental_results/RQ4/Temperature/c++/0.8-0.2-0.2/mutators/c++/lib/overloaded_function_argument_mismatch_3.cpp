//source file
#include "../include/overloaded_function_argument_mismatch_3.h"
#include "clang/ASTMatchers/ASTMatchers.h"

using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT3_OUTPUT 1

void MutatorFrontendAction_3::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("OverloadedFunction")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (!FD->isOverloadedOperator()) {
            overloadedFunctions.push_back(FD);
        }
    } else if (auto *Call = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
        if (!Call || !Result.Context->getSourceManager().isWrittenInMainFile(Call->getBeginLoc()))
            return;

        for (const auto *FD : overloadedFunctions) {
            if (FD->getNameAsString() == Call->getDirectCallee()->getNameAsString()) {
                if (FD->getNumParams() > 0) {
                    const auto *Param = FD->getParamDecl(0);
                    if (Param->getType()->isPointerType()) {
                        auto newType = "std::set<int>";
                        std::string paramStr = Param->getNameAsString();
                        std::string mutation = std::string("/*mut3*/") + newType + " " + paramStr;
                        Rewrite.ReplaceText(Param->getSourceRange(), mutation);
                        break;
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_3::MutatorASTConsumer_3::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto overloadedFunctionMatcher = functionDecl(hasOverloadedOperatorName("")).bind("OverloadedFunction");
    auto functionCallMatcher = callExpr(callee(functionDecl(hasOverloadedOperatorName("")))).bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(overloadedFunctionMatcher, &callback);
    matchFinder.addMatcher(functionCallMatcher, &callback);
    matchFinder.matchAST(Context);
}
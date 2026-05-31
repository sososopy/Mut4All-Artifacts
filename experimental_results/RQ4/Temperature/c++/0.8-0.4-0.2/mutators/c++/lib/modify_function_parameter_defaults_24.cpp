//source file
#include "../include/Modify_Function_Parameter_Defaults_24.h"

// ========================================================================================================
#define MUT24_OUTPUT 1

void MutatorFrontendAction_24::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithDefaults")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        bool hasDefaultArgs = false;
        for (unsigned i = 0; i < FD->getNumParams(); ++i) {
            if (FD->getParamDecl(i)->hasDefaultArg()) {
                hasDefaultArgs = true;
                break;
            }
        }

        if (hasDefaultArgs) {
            std::string funcDecl = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
            for (unsigned i = 0; i < FD->getNumParams(); ++i) {
                if (FD->getParamDecl(i)->hasDefaultArg()) {
                    defaultArgs[FD].push_back(FD->getParamDecl(i)->getDefaultArg());
                    const_cast<ParmVarDecl*>(FD->getParamDecl(i))->setDefaultArg(nullptr);
                }
            }
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcDecl);
        }
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
        const FunctionDecl *FD = CE->getDirectCallee();
        if (FD && defaultArgs.find(FD) != defaultArgs.end()) {
            std::string callText = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
            std::string newArgs;
            for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
                if (i > 0) newArgs += ", ";
                newArgs += stringutils::rangetoStr(*(Result.SourceManager), CE->getArg(i)->getSourceRange());
            }
            for (auto *defaultArg : defaultArgs[FD]) {
                if (!newArgs.empty()) newArgs += ", ";
                newArgs += stringutils::rangetoStr(*(Result.SourceManager), defaultArg->getSourceRange());
            }
            std::string newCall = CE->getDirectCallee()->getNameAsString() + "(" + newArgs + ")";
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), newCall);
        }
    }
}

void MutatorFrontendAction_24::MutatorASTConsumer_24::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionMatcher = functionDecl(hasAnyParameter(parmVarDecl(hasInitializer(expr())))).bind("FunctionWithDefaults");
    StatementMatcher callMatcher = callExpr(callee(functionDecl(hasAnyParameter(parmVarDecl(hasInitializer(expr())))))).bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}
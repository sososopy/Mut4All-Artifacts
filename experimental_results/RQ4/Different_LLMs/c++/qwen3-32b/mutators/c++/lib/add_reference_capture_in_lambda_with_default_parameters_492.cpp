//source file
#include "../include/Add_Reference_Capture_In_Lambda_With_Default_Parameters_492.h"

// ========================================================================================================
#define MUT492_OUTPUT 1

void MutatorFrontendAction_492::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        const ParmVarDecl *defaultParam = nullptr;
        for (const auto *param : FD->parameters()) {
            if (param->hasDefaultArg()) {
                defaultParam = param;
                break;
            }
        }
        if (!defaultParam)
            return;

        const Expr *defaultArg = defaultParam->getDefaultArg();
        std::string varName;
        if (const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(defaultArg->IgnoreImplicit())) {
            varName = DRE->getNameInfo().getName().getAsString();
        } else {
            varName = "some_var";
        }

        std::string newParam = ", int& y = " + varName;
        SourceManager &SM = Result.Context->getSourceManager();
        FullSourceLoc loc(FD->getLocation(), SM);
        SourceLocation startLoc = FD->getBeginLoc();
        SourceLocation endLoc = FD->getTypeSourceInfo()->getTypeLoc().getEndLoc();

        std::string paramList = stringutils::rangetoStr(SM, SourceRange(startLoc, endLoc));
        size_t closeParenPos = paramList.find(')');
        if (closeParenPos != std::string::npos) {
            paramList.insert(closeParenPos, newParam);
            Rewrite.ReplaceText(startLoc, paramList.length(), paramList);
        }

        for (const auto *stmt : FD->getBody()->children()) {
            if (const ReturnStmt *RS = dyn_cast<ReturnStmt>(stmt)) {
                const Expr *retExpr = RS->getRetValue();
                if (const LambdaExpr *LE = dyn_cast<LambdaExpr>(retExpr)) {
                    SourceLocation captureStart = LE->getCaptureDefaultLoc();
                    if (captureStart.isValid()) {
                        std::string captureText = stringutils::rangetoStr(SM, SourceRange(captureStart, captureStart.getLocWithOffset(1)));
                        if (captureText == "&[") {
                            captureText += "y";
                        } else if (captureText == "[") {
                            captureText += "y";
                        } else {
                            captureText += ", y";
                        }
                        Rewrite.ReplaceText(captureStart, captureText.length(), captureText);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_492::MutatorASTConsumer_492::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(
        hasDescendant(returnStmt(hasReturnValue(ignoringImplicit(lambdaExpr())))),
        hasAnyParameter(hasDefaultArgument())
    ).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
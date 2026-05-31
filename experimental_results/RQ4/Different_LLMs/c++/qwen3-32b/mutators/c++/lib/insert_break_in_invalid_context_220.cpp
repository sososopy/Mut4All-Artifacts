//source file
#include "../include/Insert_Break_In_Invalid_Context_220.h"

// ========================================================================================================
#define MUT220_OUTPUT 1

void MutatorFrontendAction_220::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("method")) {
        if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(MD->getLocation()))
            return;

        if (!MD->hasBody())
            return;
        clang::Stmt *Body = MD->getBody();
        if (!Body)
            return;

        clang::ASTContext &Context = *Result.Context;
        clang::SourceManager &SM = Context.getSourceManager();
        for (clang::Stmt *S : Body->children()) {
            if (auto *RS = clang::dyn_cast<clang::ReturnStmt>(S)) {
                clang::Expr *RetVal = RS->getRetValue();
                if (!RetVal)
                    continue;

                if (clang::isa<clang::StmtExpr>(RetVal))
                    continue;

                clang::SourceLocation Start = RetVal->getBeginLoc();
                clang::SourceLocation End = RetVal->getEndLoc();
                if (Start.isInvalid() || End.isInvalid())
                    continue;

                std::string originalExpr = stringutils::rangetoStr(SM, clang::CharSourceRange::getTokenRange(clang::SourceRange(Start, End)).getAsRange());
                std::string newCode = "{ int x; x = " + originalExpr + "; if (x) break; }";
                Rewrite.ReplaceText(RetVal->getSourceRange(), newCode);
            }
        }
    }
}
  
void MutatorFrontendAction_220::MutatorASTConsumer_220::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(
        isConstexpr(),
        hasDescendant(returnStmt(unless(hasParent(stmtExpr()))))
    ).bind("method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
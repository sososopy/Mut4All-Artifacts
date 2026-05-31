//header file
#pragma once
#include "Mutator_base.h"

/**
 * Default_Param_ConstExpr_167
 */ 
class MutatorFrontendAction_167 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(167)
private:
    class MutatorASTConsumer_167 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_167(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::FunctionDecl*> modifiedFunctions;
    };
};

//source file
#include "../include/Default_Param_ConstExpr_167.h"

// ========================================================================================================
#define MUT167_OUTPUT 1

void MutatorFrontendAction_167::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
        if (!Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (FD->isConstexpr())
            return;
        if (FD->getNumParams() == 0)
            return;
        const clang::ParmVarDecl *firstParam = FD->getParamDecl(0);
        if (firstParam->hasDefaultArg())
            return;
        clang::SourceLocation endOfParam = firstParam->getTypeSourceInfo()->getTypeLoc().getSourceRange().getEnd();
        Rewrite.InsertText(endOfParam, " = 0", true, true);
        Rewrite.InsertText(FD->getBeginLoc(), "constexpr ", true, true);
        modifiedFunctions.push_back(FD);
    } else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("ctorDecl")) {
        if (!Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;
        if (CD->isConstexpr())
            return;
        if (CD->getNumParams() == 0)
            return;
        const clang::ParmVarDecl *firstParam = CD->getParamDecl(0);
        if (firstParam->hasDefaultArg())
            return;
        clang::SourceLocation endOfParam = firstParam->getTypeSourceInfo()->getTypeLoc().getSourceRange().getEnd();
        Rewrite.InsertText(endOfParam, " = 0", true, true);
        Rewrite.InsertText(CD->getBeginLoc(), "constexpr ", true, true);
        modifiedFunctions.push_back(CD);
    } else if (auto *call = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        auto *callee = call->getDirectCallee();
        if (!callee || std::find(modifiedFunctions.begin(), modifiedFunctions.end(), callee) == modifiedFunctions.end())
            return;
        if (call->getNumArgs() == 0)
            return;
        clang::SourceLocation lParenLoc = clang::Lexer::getLocForEndOfToken(call->getBeginLoc(), 0, *Result.SourceManager, Result.Context->getLangOpts());
        clang::SourceLocation rParenLoc = call->getRParenLoc();
        if (lParenLoc.isInvalid() || rParenLoc.isInvalid())
            return;
        std::string newArgs;
        for (unsigned i = 1; i < call->getNumArgs(); ++i) {
            if (i > 1)
                newArgs += ", ";
            auto arg = call->getArg(i);
            newArgs += stringutils::rangetoStr(*Result.SourceManager, arg->getSourceRange());
        }
        Rewrite.ReplaceText(clang::SourceRange(lParenLoc, rParenLoc), newArgs);
    } else if (auto *returnStmt = Result.Nodes.getNodeAs<clang::ReturnStmt>("returnStmt")) {
        auto *expr = returnStmt->getRetValue();
        if (!expr)
            return;
        auto *call = dyn_cast<clang::CallExpr>(expr);
        if (!call)
            return;
        auto *callee = call->getDirectCallee();
        if (!callee || std::find(modifiedFunctions.begin(), modifiedFunctions.end(), callee) == modifiedFunctions.end())
            return;
        std::string originalCall = stringutils::rangetoStr(*Result.SourceManager, call->getSourceRange());
        std::string newReturn = "return " + originalCall + " && " + originalCall + ";";
        Rewrite.ReplaceText(clang::SourceRange(returnStmt->getBeginLoc(), returnStmt->getEndLoc()), newReturn);
    }
}
  
void MutatorFrontendAction_167::MutatorASTConsumer_167::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    clang::ast_matchers::DeclarationMatcher funcMatcher = clang::ast_matchers::functionDecl(
        clang::ast_matchers::unless(clang::ast_matchers::isConstexpr()),
        clang::ast_matchers::hasParameter(0, clang::ast_matchers::parmVarDecl(clang::ast_matchers::unless(clang::ast_matchers::hasDefaultArgument()))),
        clang::ast_matchers::unless(clang::ast_matchers::isMain())
    ).bind("funcDecl");
    clang::ast_matchers::DeclarationMatcher constructorMatcher = clang::ast_matchers::cxxConstructorDecl(
        clang::ast_matchers::unless(clang::ast_matchers::isConstexpr()),
        clang::ast_matchers::hasParameter(0, clang::ast_matchers::parmVarDecl(clang::ast_matchers::unless(clang::ast_matchers::hasDefaultArgument())))
    ).bind("ctorDecl");
    clang::ast_matchers::DeclarationMatcher callMatcher = clang::ast_matchers::callExpr(
        clang::ast_matchers::callee(clang::ast_matchers::decl().bind("callee"))
    ).bind("callExpr");
    clang::ast_matchers::StatementMatcher returnMatcher = clang::ast_matchers::returnStmt(
        clang::ast_matchers::hasReturnValue(clang::ast_matchers::ignoringImplicit(clang::ast_matchers::callExpr().bind("callExpr")))
    ).bind("returnStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.addMatcher(constructorMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.addMatcher(returnMatcher, &callback);
    matchFinder.matchAST(Context);
}
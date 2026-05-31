//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Memory_Function_Params_431
 */ 
class MutatorFrontendAction_431 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(431)

private:
    class MutatorASTConsumer_431 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_431(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Modify_Memory_Function_Params_431.h"

// ========================================================================================================
#define MUT431_OUTPUT 1

void MutatorFrontendAction_431::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *callExpr = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        if (!Result.Context->getSourceManager().isWrittenInMainFile(callExpr->getBeginLoc()))
            return;
        
        if (auto *funcDecl = callExpr->getDirectCallee()) {
            std::string funcName = funcDecl->getNameInfo().getName().getAsString();
            if (funcName != "memcpy" && funcName != "memmove")
                return;
        }
        
        // Modify size parameter (third argument) to 0
        if (callExpr->getNumArgs() >= 3) {
            auto sizeArg = callExpr->getArg(2);
            if (auto *intLit = dyn_cast<clang::IntegerLiteral>(sizeArg)) {
                Rewrite.ReplaceText(intLit->getSourceRange(), "0");
                return;
            }
        }
        
        // Modify destination index (first argument) to 0 if it's an array subscript
        if (callExpr->getNumArgs() >= 1) {
            auto destArg = callExpr->getArg(0);
            if (auto *unaryOp = dyn_cast<clang::UnaryOperator>(destArg)) {
                if (unaryOp->getOpcode() == clang::UO_AddrOf) {
                    auto operand = unaryOp->getSubExpr();
                    if (auto *arraySub = dyn_cast<clang::ArraySubscriptExpr>(operand)) {
                        auto index = arraySub->getIndex();
                        if (auto *intLit = dyn_cast<clang::IntegerLiteral>(index)) {
                            Rewrite.ReplaceText(intLit->getSourceRange(), "0");
                            return;
                        }
                    }
                }
            }
        }
        
        // Modify source index (second argument) to 0 if it's an array subscript
        if (callExpr->getNumArgs() >= 2) {
            auto srcArg = callExpr->getArg(1);
            if (auto *unaryOp = dyn_cast<clang::UnaryOperator>(srcArg)) {
                if (unaryOp->getOpcode() == clang::UO_AddrOf) {
                    auto operand = unaryOp->getSubExpr();
                    if (auto *arraySub = dyn_cast<clang::ArraySubscriptExpr>(operand)) {
                        auto index = arraySub->getIndex();
                        if (auto *intLit = dyn_cast<clang::IntegerLiteral>(index)) {
                            Rewrite.ReplaceText(intLit->getSourceRange(), "0");
                            return;
                        }
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_431::MutatorASTConsumer_431::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = 
        stmt(anyOf(ifStmt(), whileStmt(), forStmt(), doStmt()))
            .hasCondition(
                callExpr(
                    callee(functionDecl(hasAnyName("memcpy", "memmove"))
                    )
                ).bind("callExpr")
            ).bind("stmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Lambda_Variadic_Scope_Interaction_41
 */ 
class MutatorFrontendAction_41 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(41)

private:
    class MutatorASTConsumer_41 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_41(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Lambda_Variadic_Scope_Interaction_41.h"

// ========================================================================================================
#define MUT41_OUTPUT 1

void MutatorFrontendAction_41::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("targetFunction")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (!FD->hasBody())
            return;
        const CompoundStmt *Body = FD->getBody();
        for (const Stmt *Stmt : Body->children()) {
            if (const VarDecl *VD = dyn_cast<VarDecl>(Stmt)) {
                std::string varName = VD->getNameAsString();
                std::string packName;
                for (const ParmVarDecl *Param : FD->parameters()) {
                    if (Param->isParameterPack()) {
                        packName = Param->getNameAsString();
                        break;
                    }
                }
                if (packName.empty())
                    return;
                std::string lambdaCode = "(..., [";
                lambdaCode += varName;
                lambdaCode += "](auto a) -> decltype(";
                lambdaCode += varName;
                lambdaCode += ") { return ";
                lambdaCode += varName;
                lambdaCode += "; }(";
                lambdaCode += packName;
                lambdaCode += "));";
                SourceLocation insertLoc = VD->getEndLoc().getLocWithOffset(1);
                Rewrite.InsertText(insertLoc, lambdaCode, true, true);
                break;
            }
        }
    }
}
  
void MutatorFrontendAction_41::MutatorASTConsumer_41::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(
        hasParameterPack(),
        hasBody(compoundStmt(
            hasDescendant(varDecl())
        ))).bind("targetFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_function_parameter_and_external_declaration_44
 */ 
class MutatorFrontendAction_44 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(44)

private:
    class MutatorASTConsumer_44 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_44(Rewriter &R) : TheRewriter(R) {}
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
        std::string newName = "externVar";
    };
};

//source file
#include "../include/modify_function_parameter_and_external_declaration_44.h"

// ========================================================================================================
#define MUT44_OUTPUT 1

void MutatorFrontendAction_44::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        for (auto *param : FD->parameters()) {
            if (auto *ED = Result.Nodes.getNodeAs<clang::VarDecl>("ExternVar")) {
                if (ED->getName() == param->getName()) {
                    SourceLocation startLoc = ED->getBeginLoc();
                    SourceLocation endLoc = ED->getEndLoc();
                    Rewrite.ReplaceText(SourceRange(startLoc, endLoc), "extern int " + newName + ";");
                }
            }
        }
    }
}
  
void MutatorFrontendAction_44::MutatorASTConsumer_44::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionMatcher = functionDecl(hasDescendant(varDecl(hasStorageClass(SC_Extern), hasName("i")).bind("ExternVar"))).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}
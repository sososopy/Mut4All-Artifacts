//header file
#pragma once
#include "Mutator_base.h"

/**
 * CStyleCastAfterInvalidArrayInit_298
 */ 
class MutatorFrontendAction_298 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(298)

private:
    class MutatorASTConsumer_298 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_298(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
        std::vector<FunctionDecl*> functions;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite, const std::vector<FunctionDecl*>& funcs) : Rewrite(Rewrite), functions(funcs) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        const std::vector<FunctionDecl*>& functions;
    };
};

//source file
#include "../include/CStyleCastAfterInvalidArrayInit_298.h"

// ========================================================================================================
#define MUT298_OUTPUT 1

void MutatorFrontendAction_298::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (!FD->getBody())
            return;

        const Stmt* body = FD->getBody();
        SourceRange bodyRange = body->getSourceRange();
        if (bodyRange.isInvalid())
            return;

        if (functions.empty())
            return;

        std::string fooName = functions[0]->getNameAsString();

        std::string newCode = "{\n    int x[] = { " + fooName + "->a };\n    (int)x;\n}";

        Rewrite.ReplaceText(SourceRange(bodyRange.getBegin(), bodyRange.getEnd()), newCode);
    }
}
  
void MutatorFrontendAction_298::MutatorASTConsumer_298::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;

    DeclarationMatcher funcMatcher = functionDecl().bind("func");
    class FuncCollector : public MatchFinder::MatchCallback {
    public:
        FuncCollector(std::vector<FunctionDecl*>& funcs) : funcs(funcs) {}
        virtual void run(const MatchFinder::MatchResult &Result) {
            if (auto* FD = Result.Nodes.getNodeAs<FunctionDecl>("func")) {
                funcs.push_back(FD);
            }
        }
    private:
        std::vector<FunctionDecl*>& funcs;
    };
    FuncCollector collector(functions);
    matchFinder.addMatcher(funcMatcher, &collector);
    matchFinder.matchAST(Context);

    DeclarationMatcher mainMatcher = functionDecl(hasBody(compoundStmt())).bind("FuncDecl");
    Callback callback(TheRewriter, functions);
    matchFinder.addMatcher(mainMatcher, &callback);
    matchFinder.matchAST(Context);
}
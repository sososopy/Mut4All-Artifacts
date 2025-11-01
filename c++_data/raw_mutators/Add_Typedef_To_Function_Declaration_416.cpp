//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Typedef_To_Function_Declaration_416
 */ 
class MutatorFrontendAction_416 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(416)

private:
    class MutatorASTConsumer_416 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_416(Rewriter &R) : TheRewriter(R) {}
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
        std::map<const clang::FunctionDecl*, std::string> typedefMap;
    };
};

//source file
#include "../include/Add_Typedef_To_Function_Declaration_416.h"

// ========================================================================================================
#define MUT416_OUTPUT 1

void MutatorFrontendAction_416::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getNumParams() > 0) {
            auto *param = FD->getParamDecl(0);
            auto paramType = param->getType().getAsString();

            if (typedefMap.find(FD) == typedefMap.end()) {
                std::string typedefName = "Typedef_" + std::to_string(reinterpret_cast<uintptr_t>(FD));
                typedefMap[FD] = typedefName;

                std::string typedefDecl = "typedef " + paramType + " " + typedefName + ";\n";
                Rewrite.InsertTextBefore(FD->getBeginLoc(), "/*mut416*/" + typedefDecl);

                auto paramTypeLoc = param->getTypeSourceInfo()->getTypeLoc();
                Rewrite.ReplaceText(paramTypeLoc.getSourceRange(), typedefName);
            }
        }
    }
}

void MutatorFrontendAction_416::MutatorASTConsumer_416::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
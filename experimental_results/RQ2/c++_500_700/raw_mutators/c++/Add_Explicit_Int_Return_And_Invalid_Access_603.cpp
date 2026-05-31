//header file
#pragma once
#include "Mutator_base.h"

/**
 * add_explicit_int_return_and_invalid_access_603
 */ 
class MutatorFrontendAction_603 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(603)

private:
    class MutatorASTConsumer_603 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_603(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_603.h"

// ========================================================================================================
#define MUT603_OUTPUT 1

void MutatorFrontendAction_603::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        
        if (!FD->getReturnType()->isVoidType())
            return;

        auto declaration = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
        declaration.insert(declaration.find(FD->getNameAsString()), "int ");
        
        std::string mutation = "\nunsigned numberArray[] = {((SomeStruct*)somePointer)->nonExistentMember};\n(unsigned)numberArray;\n";
        declaration.insert(declaration.find("{") + 1, mutation);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}

void MutatorFrontendAction_603::MutatorASTConsumer_603::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(cxxMethodDecl())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
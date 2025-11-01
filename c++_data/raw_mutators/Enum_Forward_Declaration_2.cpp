//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enum_Forward_Declaration_2
 */ 
class MutatorFrontendAction_2 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(2)

private:
    class MutatorASTConsumer_2 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_2(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const EnumDecl*> visitedEnums;
    };
};

//source file
#include "../include/enum_forward_declaration_2.h"

// ========================================================================================================
#define MUT2_OUTPUT 1

void MutatorFrontendAction_2::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(ED->getLocation()))
            return;
        
        if (ED->isComplete() || visitedEnums.find(ED) != visitedEnums.end())
            return;

        visitedEnums.insert(ED);

        std::string enumName = ED->getNameAsString();
        if (enumName.empty())
            return;

        std::string mutation = "template <typename T> void someFunction(typename T::EnumMember); someFunction<" + enumName + ">(0);";
        mutation = "/*mut2*/" + mutation;

        Rewrite.InsertTextAfterToken(ED->getEndLoc(), "\n" + mutation);
    }
}

void MutatorFrontendAction_2::MutatorASTConsumer_2::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = enumDecl(unless(isDefinition())).bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
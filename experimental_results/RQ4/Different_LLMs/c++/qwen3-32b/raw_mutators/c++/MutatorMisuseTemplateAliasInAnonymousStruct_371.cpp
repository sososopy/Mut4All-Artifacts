//header file
#pragma once
#include "Mutator_base.h"

/**
 * MutatorMisuseTemplateAliasInAnonymousStruct_371
 */ 
class MutatorFrontendAction_371 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(371)

private:
    class MutatorASTConsumer_371 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_371(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/MutatorMisuseTemplateAliasInAnonymousStruct_371.h"

// ========================================================================================================
#define MUT371_OUTPUT 1

void MutatorFrontendAction_371::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TAD = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("alias")) {
        if (!TAD || !Result.Context->getSourceManager().isWrittenInMainFile(TAD->getLocation()))
            return;

        std::string aliasName = TAD->getNameAsString();
        std::string misspelledName = aliasName + "_";
        std::string structCode = "struct {\n    " + misspelledName + ";\n};\n";
        SourceLocation endLoc = TAD->getEndLoc();
        Rewrite.InsertTextAfter(endLoc, "/*mut371*/\n" + structCode);
    }
}

void MutatorFrontendAction_371::MutatorASTConsumer_371::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("alias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
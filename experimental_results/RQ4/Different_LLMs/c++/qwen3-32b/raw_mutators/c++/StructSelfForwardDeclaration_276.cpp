//header file
#pragma once
#include "Mutator_base.h"

/**
 * StructSelfForwardDeclaration_276
 */ 
class MutatorFrontendAction_StructSelfForwardDeclaration_276 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(276)

private:
    class MutatorASTConsumer_276 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_276(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_StructSelfForwardDeclaration_276.h"

// ========================================================================================================
#define MUT276_OUTPUT 1

void MutatorFrontendAction_StructSelfForwardDeclaration_276::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        std::string structName = RD->getNameAsString();
        if (structName.empty())
            return;
        SourceLocation openBraceLoc = RD->getBraceRange().getBegin();
        if (openBraceLoc.isInvalid())
            return;
        std::string forwardDecl = "struct " + structName + ";";
        Rewrite.InsertTextAfter(openBraceLoc, forwardDecl);
    }
}

void MutatorFrontendAction_StructSelfForwardDeclaration_276::MutatorASTConsumer_276::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
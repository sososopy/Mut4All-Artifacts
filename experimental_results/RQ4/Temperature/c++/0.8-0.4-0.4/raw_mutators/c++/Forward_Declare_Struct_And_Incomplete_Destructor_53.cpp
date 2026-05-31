//header file
#pragma once
#include "Mutator_base.h"

/**
 * forward_declare_struct_and_incomplete_destructor_53
 */ 
class MutatorFrontendAction_53 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(53)

private:
    class MutatorASTConsumer_53 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_53(Rewriter &R) : TheRewriter(R) {}
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
        const CXXRecordDecl *targetStruct = nullptr;
    };
};

//source file
#include "../include/forward_declare_struct_and_incomplete_destructor_53.h"

// ========================================================================================================
#define MUT53_OUTPUT 1

void MutatorFrontendAction_53::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SR = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
        if (!SR || !Result.Context->getSourceManager().isWrittenInMainFile(SR->getLocation()))
            return;
        if (!SR->isStruct() || !SR->isCompleteDefinition())
            return;

        targetStruct = SR;
        std::string structName = SR->getNameAsString();
        std::string forwardDecl = "struct " + structName + ";";
        std::string incompleteDtor = structName + "::~" + structName + "();";
        
        SourceLocation insertLoc = SR->getBeginLoc();
        Rewrite.InsertTextBefore(insertLoc, "/*mut53*/" + forwardDecl + "\n" + incompleteDtor + "\n");
    }
}

void MutatorFrontendAction_53::MutatorASTConsumer_53::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("Struct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}